# Compiler Construction Assignment
#### RAMIT SHIVANSH
#### 2018A7PS0204G
<hr></br>

## Summary of project

The assignment is written in ```C```, ```lex``` and ```yacc```, and compiled using
1. ```gcc``` version ```10.2.0``` 
2. ```flex``` version ```2.5.35 Apple(flex-32)``` 
3. ```bison``` version ```3.7.6``` 

## Project Structure
The following code files can be seen in the project directory:

- <code>lexer.l</code> : This is the lexer for the compiler. It tokenizes the input according to regex.
- <code>parser.y</code> : This is the parser that receives tokens from the lexer and gives instructions to the main function according to the grammar.
- <code>main.c</code> : This file contains all the functions required to run the game, print errors and some other utility functions.
- <code>interpreter.h</code> : It contains all the declarations and macros shared between the main file, the lexer and the parser.

## Compiling and Initialization

The entire compilation for the code can be done directly through the makefile and the make command.

To compile, use <code>make</code> or <code>make all</code>

This will run 2 'make' scripts: 
- <code>make compile</code> : Compiles all the files.
- <code>make run</code> : Runs the compiled file.

Hence ```make all``` or ```make``` compiles AND runs the program. </br>

So to run the app, just use command : ```make```.

## Removing build files

Run <code>make clean</code> to clean and remove the temporary build files.

## Exiting the app

Once inside the interactive shell, use ```ctrl + c``` to exit.
</br>

# Assumptions and Implementation

- Suppose a row is 4 2 2 4. Then SUBTRACT LEFT operation will result in 4 4 0 0 as the middle 2 tiles will get eliminated.

- Indexing of the board is done from ```1```. The top left cell is ```[1,1]```, cell to its right is ```[1,2]``` and the bottom right cell is ```[4,4]```. A clearer representation can be seen when you run the program.

- Nested commands can be used in the following way:
```
    ASSIGN VALUE IN 1,2 TO 2,3.
```

- Stderr is printed on every command where state of the 2048 game is changed.

- Stderr format is as follows: ```<Array of numbers representing matrix in row major form separated by space ('\40')> <x1>,<y1>name1,name2,name3 <x2>,<y2>name4,name5``` and so on.

- Every error prints ```-1``` to stderr.

- Empty tiles cannot be assigned a variable name.

- A tile's value can be reassigned any number of times. It can also be assigned to ```0```.

- A non-empty tile can hold multiple variables. If its value becomes ```0``` at any point (by assignment or by subtraction) all of its variable name are de-assigned, i.e, deleted from memory.