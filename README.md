MIPSEmulator
============

A simple emulator for a subset of the MIPS instruction set, as used by University of Waterloo. The emulator replicates memory, register and program counter functionality and can output useful debug information. It can load and execute any valid MIPS program that complies to the language spec used by UW. A copy of the language spec is also included in the root directory of the project.

Usage
============

Currently the program only loads a file name "test.mips" located in the exeuction directory. This will hopefully change soon to support better file loading and direction.

When the file is loaded it will emulate the execution of the assembly code inside. It will print out each line of execution if DEBUG is set to 0. If DEBUG is set to 1 it will print out lines and register, memory, and branching descriptions for each instruction as well as memory contents and register contents.

TODO
============

- Proper Command Line Interface and debugging tools - right now everything is done with print statements.
- User Interface
- Handling of special memory addresses - reading input and writing output with 0xffff0004 and 0xffff000c respectively.
