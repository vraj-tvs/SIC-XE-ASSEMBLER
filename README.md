# SIC/XE Assembler

The SIC/XE assembler translates assembly language programs written in SIC/XE format into machine language instructions. It performs two passes - pass 1 for collecting information about symbols and addresses, and pass 2 for generating the actual machine code.

**Course project**
B.Tech. CSE-2nd Year
IIT Roorkee

## Description

This project is a SIC/XE assembler implemented in C++. It consists of four header files and a compilation file to generate an executable. 

**Features included:**

- [x] Literals
- [x] Expressions
- [x] Symbol-defining statements
- [x] Program Blocks
- [ ] Control Sections

### Components

- `tables.hpp`: Contains necessary data structures used in the assembler.
- `utility.hpp`: Includes functions that are commonly used across both passes of the assembler.
- `pass1.hpp`: Implements pass 1 of the assembler.
- `pass2.hpp`: Implements pass 2 of the assembler.
- `compile.cpp`: The compilation file that generates the executable by including `pass2.hpp`.

## Usage

To compile the assembler and generate the executable file (here, `compile`), execute the following command:
>g++ compile.cpp -o compile

To run the executable file genreated:
>./compile 

Provide the input file name:
><input_file.txt>

Successful execution will generate the following output files:
- `intermediate_file.txt`: File generated after pass1.
- `assembly_listing.txt`: Listing file generated after pass2. Also contains the errors (if any).
- `obj_program.txt`: Contains the object program of given SIC-XE code.

## Test programs

```bash
SUM     START   0
FIRST   LDX     #0
        LDA     #0
        +LDB    #TABLE2
        BASE    TABLE2
LOOP    ADD     TABLE, X
        ADD     TABLE2, X
        TIX     COUNT
        JLT     LOOP
        +STA    TOTAL
        RSUB
COUNT   RESW    1
TABLE   RESW    2000
TABLE2  RESW    2000
TOTAL   RESW    1
        END     FIRST
```

```bash
copy    start   0 
first   stl     retadr
cloop   jsub    rdrec
        lda     length 
        comp    #0 
        jeq     endfil
        j       cloop
endfil  J       @retadr
        use     cdata
retadr  resw    1
length  resw    1
        use     cblks
buffer  resb    4096
bufend  equ     *
maxlen  equ     4096
        use
rdrec   clear   x 
        clear   a
        clear   s 
        +ldt    #maxlen
loop    td      input 
        jeq     loop 
 .gkjbhlnl
        rd      input
com     pr      a, s
        jeq     exit 
 
        stch    buffer,x
        tixr    t
        jlt     loop
exit    stx     length
        rsub
        use     cdata
input   byte    x'f3'
        end     first
```