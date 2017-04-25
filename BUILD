COPTS = [
    "-Wall",
    "-Werror",
    "-Wextra",
    "-g",
]

cc_binary(
    name = "ministrace",
    srcs = ["ministrace.c"],
    copts = COPTS,
    deps = [
        ":ptrace_util",
        ":syscall_util",
    ],
)

cc_library(
    name = "syscall_util",
    srcs = ["syscall_util.c"],
    hdrs = ["syscall_util.h"],
    copts = COPTS,
)

cc_library(
    name = "ptrace_util",
    srcs = ["ptrace_util.c"],
    hdrs = ["ptrace_util.h"],
    copts = COPTS + [
        "-lcap",
    ],
    deps = [":syscall_util"],
)
