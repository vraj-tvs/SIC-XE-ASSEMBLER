# SIC/XE Assembler

The SIC/XE assembler translates assembly language programs written in SIC/XE format into machine language instructions. It performs two passes - pass 1 for collecting information about symbols and addresses, and pass 2 for generating the actual machine code.

## Description

This project is a SIC/XE assembler implemented in C++. It consists of four header files and a compilation file to generate an executable. 

**Features included:**

- [x] Literals
- [x] Expressions
- [x] Symbol-defining statements
- [x] Program Blocks
- [ ] Control Sections

### Components

- **tables.hpp**: Contains necessary data structures used in the assembler.
- **utility.hpp**: Includes functions that are commonly used across both passes of the assembler.
- **pass1.hpp**: Implements pass 1 of the assembler.
- **pass2.hpp**: Implements pass 2 of the assembler.
- **compile.cpp**: The compilation file that generates the executable by including `pass2.hpp`.

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
