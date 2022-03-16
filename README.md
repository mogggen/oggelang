# Oggelang
A stupid idea brought to reality.

Things barely work so don't expect a fun and happy time with this language.

## Building
Use cmake to generate a build system of your choice and just hope it works.
The executable is placed in the bin folder.

## Running
Run the executable with the main file of your ogge program as an argument.
You only need to give the main file to the interpreter as any files referenced from said main file will automatically be compiled and run as well.

Windows:

    .\bin\ogge.exe test_programs\new.ogge

Linux and probably MacOS:

    ./bin/ogge test_programs/new.ogge

## Tutorial

### Types
There are only one type in oggelang and that is a 32-bit signed integer. But it can be used as a pointer, char or just a int.

### Expressions
The expressions work like most languages, except oggelang doesn't support parentages.
There are also some other quirks.

- '@' is the dereference symbol.
- '\*' is the get address of this variable symbol.
- '<<' will left shift the expression to the right.
- '>>' will right shift the expression to the right.

### Keywords 

There are currently seven different keywords in oggelang.

- goto
- var
- if
- alloc
- print
- printc
- scan

#### goto
Goto immidiatly jumps to a different line in the program. If no filename is specified it jumps to given line in the current file.

    goto [:linenumber]

    goto [filename:linenumber]

Example:

    goto :12 
Jumps to line 12 in the current file.

    goto file2.ogge:28
Jumps to line 28 in file2.ogge.

#### var
Var is used to declare a variable and can be used with or without assignment.

    var my_variable
    var my_variable = 4

#### if
If statements doesn't have an entire block after it, instead it only has a single statement following the expression. For example:

    if [expression] [statement]
Example:

    if 2 < 4 goto :12

If the expression (2 < 4) is true or greater than 0 the statatement (goto :12) executed whitch jumps to line 12 in the same file

#### alloc
Allocates an variable on the head and returns the pointer. There is currently no way to free the allocated memory.

#### print
Prints the value of the expression to stdout.

    print [expression]

Example:

    print my_variable
    print 3+65*my_variable

#### printc
Same as print but prints the value of the expression as an ASCII character.

#### scan
Reads a input from stdin and stores it in the given variable. All inputs are handled as integers.

    scan [variable]
Example:

    var input_var
    scan input_var

