#ifndef __PTRACE_UTIL_H
#define __PTRACE_UTIL_H

void ptrace_init();
int ptrace_await_syscall(pid_t child_pid);
pid_t ptrace_spawn(void (*f)());
int ptrace_syscall_num(pid_t child_pid);
int ptrace_syscall_ret(pid_t child_pid);

#endif
