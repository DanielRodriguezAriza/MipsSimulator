# Simple MIPS Simulator

## Introduction
This project is a very simple MIPS Simulator built using C++.

## Features
- **Loading code from a text file.**
- **Code Tokenizer.**
- **Token Parser.**
- **Instruction Interpreter / Virtual machine.**
- **Simple code execution:** There are no text and data segments yet, and tags are not supported as of now. All of the code in the input text file is translated into instructions that the virtual machine will execute.
- **Simple Debugging:** The current version has a half baked debugging system that allows you to visualize the generated tokens and instructions before running the code. There is also partial support for tags that does not fully function as of now, and the debugging system is also capable of detecting tag creation.

## Planned features
The following points are planned features that will be implemented in future versions of the MIPS Simulator.
- **Support for adding tags.**
- **Support for adding sections.**
- **Support for all MIPS32 instructions.**
- **Support for a wider variety of system calls.**
- **Support for floating point coprocessor simulation.**
- **Support for macros.**
- **Support for built in debugging tools.**
- **Support for extracting values from the virtual machine's registers and memory for external usage.**
- **Changes to the parser:** The parser currently uses a hard coded instruction structure system to parse the instructions from the generated tokens. In future versions, it will be updated to a more flexible system like an AST (Abstract syntax tree) or a system to detect instructions of the different types that exist in MIPS architectures (R-type, I-type, J-type).

## Usage
To use this simple MIPS Simulator in your C++ project, you can simply include the "Mips.h" header in your project and use the MipsSimulator::loadSource(src) and MipsSimulator::runProgram() functions to load an external text file with the MIPS source code and execute the code.
