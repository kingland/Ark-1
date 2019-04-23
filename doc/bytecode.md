# Ark bytecode

A basic file is composed of those headers:
- magic number, 6386283 ('ark'), then a zero padding
- symbols table
    - number of elements (two bytes, big endian)
    - strings, null terminated
- values table (aka constants table)
    - number of elements (two bytes, big endian)
        - type (1 byte)
            - 0x01 for number
            - 0x02 for string
            - 0x03 for function
        - value
            - number: represented in hexadecimal format (stored as a null terminated string), big endian
            - string: all the characters, plus a \0 at the end (aka null terminated string)
            - function: page number (two bytes, big endian)
- code segments (can have multiple code segments)
    - number of elements (two bytes, big endian), can be equal to 0
    - instructions

## Note on jumps

Jumps are used to jump from a code segment to another, in case of functions. The page number is directly encoded on two bytes (big endian).

## Instructions

| Code | Argument(s) | Job |
| ---- | ----------- | --- |
| `NOP` (0x00) | | Does Nothing |
| `LOAD_SYMBOL` (0x01) | symbol id (two bytes, big endian) | Load a symbol from its id onto the stack |
| `LOAD_CONST` (0x02) | constant id (two bytes, big endian) | Load a constant from its id onto the stack |
| `POP_JUMP_IF_TRUE` (0x03) | relative address to jump to (two bytes, big endian) | Jump to the provided address if the last value on the stack was equal to true. Pop it if true, otherwise, leave it on the stack |
| `STORE` (0x04) | symbol id (two bytes, big endian) | Take the value on top of the stack and put it inside a variable named following the symbol id (cf symbols table), in the nearest scope. Raise an error if it couldn't find a scope where the variable exists |
| `LET` (0x05) | symbol id (two bytes, big endian) | Take the value on top of the stack and create a variable in the current scope, named following the given symbol id (cf symbols table) |
| `POP_JUMP_IF_FALSE` (0x06) | relative address to jump to (two bytes, big endian) | Jump to the provided address if the last value on the stack was equal to false. Pop it if false, otherwise, leave it on the stack |
| `JUMP` (0x07) | relative address to jump to (two byte, big endian) | Jump to the provided address |
| `RET` (0x08) | | If in a code segment other than the main one, quit it, and push the value on top of the stack to the new stack ; should as well delete the current environment. Otherwise, acts as a `HALT` |
| `HALT` (0x09) | | Stop the Virtual Machine |
| `CALL` (0x0a) | number of arguments when calling the function | Call function from its symbol id located on top of the stack. Take the given number of arguments from the top of stack and give them  to the function (the first argument taken from the stack will be the last one of the function). The stack of the function is now composed of its arguments, from the first to the last one |
| `NEW_ENV` (0x0b) | | Create a new environment (a new scope for variables) in the Virtual Machine |