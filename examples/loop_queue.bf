[
    FIFO queue - reads characters from input and after each character read 
    displays last characters read in the order they were read. When reading, 
    this program skips new line characters (0x10). Maximum number of displayed 
    characters is two less than cells in the memory.

    This program requires loop memory (decrementing pointer to the first cell
    goes to the last one and vice versa) and at least 3 memory cells.

    It stops reading when it reads null character (0x00). Null is sent by the 
    input operation when end-of-file (EOF) is reached. You can use CTRL+Z in 
    windows PowerShell to send it.

    example input string:
    "AAAAAAAAAA BBBBBBBBBB CCCCCCCCCC DDDDDDDDDD EEEEEEEEEE FFFFFFFFFF GGGGGGGGGG HHHHHHHHHH IIIIIIIIII JJJJJJJJJJ KKKKKKKKKK LLLLLLLLLL MMMMMMMMMM NNNNNNNNNN OOOOOOOOOO PPPPPPPPPP QQQQQQQQQQ RRRRRRRRRR SSSSSSSSSS TTTTTTTTTT UUUUUUUUUU VVVVVVVVVV WWWWWWWWWW XXXXXXXXXX YYYYYYYYYY ZZZZZZZZZZ"
    example Mozkomord BF interpreter options:
    "--mem-type static-loop --mem-size 80"
]

,                                       read first input

[                                       main program loop
    [<] >                               go to the first stored characters
    [.>]                                print all stored characters
    ++++++++++.----------               print a new line char

    > [-]>[-] <<                        reset next two cells to zero
    
    >> +                                start read loop
    [
        [-] < [-]+ <                    reset second and third cell to zero
        ,                               read the input
        ---------- [>-< [->>+<<] ]      do nothing if it is NOT new line
        >> [- << + >>] <<               copy back data after check
        > [> + < -] <                   if it IS new line loop again
        ++++++++++ >>                   fix back the read data
    ] 
    <<                                  move back the data ptr
]
