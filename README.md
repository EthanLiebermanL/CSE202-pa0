# prog1: Binary Representations and Bitwise Operations
## Programming Assignment  Learning Outcomes:
- Manipulate different binary representations of unsigned, - signed, and floating-point numbers
- Use bitwise operations to manipulate binary representations

## Specification

Write a program that includes the implementation of the following functions:

`int any_even_one(unsigned x)` returns 1 if any even bit of x is 1, 0 otherwise 

`unsigned leftmost_one(unsigned x)` returns a mask indicating the leftmost 1 in x

`unsigned rotate_left(unsigned x, int n)` returns x shifted n positions to the left with the n most significant bits of x inserted on the right

`unsigned rotate_right(unsigned x, int n)`  returns x shifted n positions to the right with the n least significant bits of x inserted on the left

`unsigned saturating_add(int x, int y)` returns the sum of x and y if the sum does not overflow, TMAX if a positive overflow occurs, or TMIN if a negative overflow occurs

`unsigned float_twice(unsigned f)` returns the bit representation of f multiplied by 2. If f is infinity or NaN, the function returns f

`unsigned float_half(unsigned f)` returns the bit representation of f divided by 2. If f is infinity or NaN, the function returns f

An empty main function and prototypes of the functions described above are provided in the file `prog1.c`. The assignment consists in writing the definition of the functions and the main function. The main function should accept command-line arguments for the different operations as listed in the test cases. The input string of 8 hex characters should be stored in the provided union value as 8 hex bytes (note that the bytes should be stored in the array `bytes` using little endian). The same union value can then be accessed as an unsigned, signed, or float number using the fields `uval`, `sval`, or `fval` respectively.


A script file is also provided to test your program (`run_tests.bash`). The bash script performs the following operations:
- compiles prog1.c into the executable prog1
- runs prog1 for all the 32 test cases
- compares prog1 output to the reference output `tests.reference`
- displays the final score of your program 

Keep testing your program until you get a score of 80/80 or all the 32 test cases pass.

All the functions should use the bit-level integer or floating-point coding rules listed below.


IMPORTANT NOTE:
"You are required to develop your own functions to work with the bits in memory. As the purpose of this assignment is for you to learn how bits are stored in memory and how they need to be manipulated for use in various operations and for exchanging between computing systems, you are not allowed to use any existing library functions that may do this work for you."
