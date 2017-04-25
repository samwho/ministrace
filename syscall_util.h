#ifndef __SYSCALL_UTIL_H
#define __SYSCALL_UTIL_H

#include <sys/reg.h>

#if __WORDSIZE == 64
#define SYSCALL_NUM sizeof(long) * ORIG_RAX
#define SYSCALL_RET sizeof(long) * RAX
#else
#define SYSCALL_NUM sizeof(long) * ORIG_EAX
#define SYSCALL_RET sizeof(long) * EAX
#endif

#define CHECK_SYSCALL(call) exit_on_error(call, __FILE__, __LINE__)

int exit_on_error(int ret, char *file, int lineno);
const char *sysnum2name(int sysnum);

#endif
