[
    "Hello world" program in Brainfuck.
    Code source: https://cs.wikipedia.org/w/index.php?title=Brainfuck&oldid=20807516
    Modifications:
    - formatting, comments
    - vertical line characters as breakpoints for Mozkomord debug mode.
]

++++++++++
[
    >+
    >+++
    >+++++++
    >++++++++++
    <<<<-
]
memory after this loop will be (hex) 00 0a 1e 46 64
                            or (dec) 000 010 030 070 100

|                             breakpoint
>>> ++ .                      H
> + .                         e
+++++++ ..                    ll
+++ .                         o
<< ++ .                       space
|                             breakpoint
> +++++++++++++++ .           W
> .                           o
+++ .                         r
------ .                      l
-------- .                    d
<< + .                        !
< .                           new line
