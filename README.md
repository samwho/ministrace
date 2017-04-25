# ministrace

Based on an initial curiosity for how strace works, an afternoon of programming
and subsequently cribbing heavily from [this blog post][1], this repo contains a
miniature strace implementation.

# Building

This project uses [Bazel][2] as its build system. You'll need to install Bazel
and then run:

    $ bazel build :ministrace

To build the project.

# Running

    $ build run :ministrace -- ls `pwd`

[1]: https://blog.nelhage.com/2010/08/write-yourself-an-strace-in-70-lines-of-code/
[2]: http://bazel.io
