#include "ptrace_util.h"
#include "syscall_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s prog args\n", argv[0]);
    exit(1);
  }

  ptrace_init();

  int cargc = argc - 1;
  char **cargv = argv + 1;

  char *args[cargc + 1];
  memcpy(args, cargv, cargc * sizeof(char *));
  args[cargc] = NULL;

  pid_t child_pid = ptrace_spawn_execvp(args[0], args);

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

    printf("[%d] %-16s = %d\n", child_pid, sysnum2name(sysnum), sysret);
    fflush(stdout);
  }

  return 0;
}
