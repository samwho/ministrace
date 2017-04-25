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

  /*
   * Because the argv array contains the currently running program as the first
   * argument, we create child argc and argv variables that chop that off. We
   * also need to provide execvp with a null-terminate args list, which isn't
   * guaranteed with the args list passed through main().
   */
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

  /*
   * Main loop: prints out syscalls and their return values.
   */
  while (1) {
    if (ptrace_await_syscall(child_pid) != 0)
      break;
    int sysnum = ptrace_syscall_num(child_pid);
    printf("[%d] %-16s = ", child_pid, sysnum2name(sysnum));

    if (ptrace_await_syscall(child_pid) != 0)
      break;
    int sysret = ptrace_syscall_ret(child_pid);
    printf("%d\n", sysret);

    /*
     * By default, stdout is buffered and won't always print out each line as it
     * happens. Flushing stdout for each syscall and return means we print out
     * the values at the closest point to when the syscall happened in the
     * child.
     */
    fflush(stdout);
  }

  printf("\n");
  return 0;
}
