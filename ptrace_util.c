#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <sys/capability.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscall_util.h"

void ptrace_init() {
  pid_t pid = getpid();
  int res = prctl(PR_CAPBSET_READ, CAP_SYS_PTRACE, NULL, NULL, NULL);
  if (res == -1 || res == 0) {
    error_at_line(1, errno, __FILE__, __LINE__,
                  "process %d does not have CAP_SYS_PTRACE capability", pid);
  }
}

pid_t ptrace_spawn(void (*f)()) {
  int pid = fork();
  if (pid == 0) {
    CHECK_SYSCALL(ptrace(PTRACE_TRACEME, 0, 0, 0));
    raise(SIGSTOP);

    f();
    exit(0);
  }

  int status;
  CHECK_SYSCALL(waitpid(pid, &status, 0));
  if (WIFEXITED(status))
    return -1;

  // Make it possible to distinguish when a child process has stopped due to a
  // syscall happening, instead of any SIGTRAP.
  CHECK_SYSCALL(ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD));

  return pid;
}

int ptrace_await_syscall(pid_t child_pid) {
  int status;

  while (1) {
    CHECK_SYSCALL(ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL));
    CHECK_SYSCALL(waitpid(child_pid, &status, 0));

    // The bitwise and against 0x80 depends on us setting PTRACE_O_TRACESYSGOOD
    // on the child process. If we don't, this conditional won't work.
    if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
      return 0;

    if (WIFEXITED(status))
      return 1;
  }
}

int ptrace_syscall_num(pid_t child_pid) {
  return CHECK_SYSCALL(ptrace(PTRACE_PEEKUSER, child_pid, SYSCALL_NUM));
}

int ptrace_syscall_ret(pid_t child_pid) {
  return CHECK_SYSCALL(ptrace(PTRACE_PEEKUSER, child_pid, SYSCALL_RET));
}
