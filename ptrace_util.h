#ifndef __PTRACE_UTIL_H
#define __PTRACE_UTIL_H

#include <unistd.h>

/*
 * ptrace_init: checks to see that the current process can make ptrace calls,
 * exits unsuccessfully if we can't.
 */
void ptrace_init();

/*
 * ptrace_spawn_execvp: spawn a subprocess with a given program and args list in
 * a state where it is ready to be traced.
 *
 * Note that programs will not begin running immediately. You'll need to call
 * other ptrace functions to resume execution.
 */
pid_t ptrace_spawn_execvp(const char *path, char *const argv[]);

/*
 * ptrace_spawn_func: see ptrace_spawn_execvp, the only difference is that
 * instead of running another program, it will execute a given function.
 */
pid_t ptrace_spawn_func(void (*f)());

/*
 * ptrace_await_syscall: resumes a traced child process but will stop the
 * process where it makes a syscall.
 *
 * See man ptrace for more information, especially the PTRACE_SYSCALL symbol.
 */
int ptrace_await_syscall(pid_t child_pid);

/*
 * ptrace_syscall_num: use this function to retrieve what syscall was just
 * entered after first calling ptrace_await_syscall.
 */
int ptrace_syscall_num(pid_t child_pid);

/*
 * ptrace_syscall_ret: use this function to retrieve the return value of the
 * syscall that has just been returned from after calling ptrace_await_syscall a
 * second time.
 */
int ptrace_syscall_ret(pid_t child_pid);

#endif
