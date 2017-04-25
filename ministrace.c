#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/capability.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ptrace_util.h"

void child() {
  printf("Hello, from child!\n");
}

int main(void) {
  ptrace_init();
  pid_t child_pid = ptrace_spawn(child);

  if (child_pid == -1) {
    fprintf(stderr, "child exited unexpectedly\n");
    return 1;
  }

  while (1) {
    if (ptrace_await_syscall(child_pid) != 0)
      break;
    int sysnum = ptrace_syscall_num(child_pid);

    if (ptrace_await_syscall(child_pid) != 0)
      break;
    int sysret = ptrace_syscall_ret(child_pid);

    printf("syscall\t%d\treturned\t%d\n", sysnum, sysret);
  }

  return 0;
}
