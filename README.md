MIPSEmulator
============

A simple emulator for a subset of the MIPS instruction set, as used by University of Waterloo. The emulator replicates memory, register and program counter functionality and can output useful debug information. It can load and execute any valid MIPS program that complies to the language spec used by UW. A copy of the language spec is also included in the root directory of the project.

TODO
============

- Proper Command Line Interface and debugging tools - right now everything is done with print statements.
- User Interface
- Handling of special memory addresses - reading input and writing output with 0xffff0004 and 0xffff000c respectively.
