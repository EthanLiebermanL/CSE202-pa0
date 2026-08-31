#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
// Union to store 4 bytes as an array of bytes, an unsigned, signed, or float number
union value{
    unsigned uval;
    int sval;
    float fval;
    unsigned char bytes[4];
};
// reads 8 hex characters from string input and stores it in the union v
// returns -1 if the hexadecimal number is invalid, 0 otherwise
int read_hex(union value *v, char *input) {
    unsigned result = 0;
    
    for (int i = 0; i < 8; i++) {
        char c = input[i];
        int digit;

        if (c >= '0' && c <= '9') { //0-9
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') { //a-f lowercase
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') { //a-f uppercase
            digit = c - 'A' + 10;
        } else {
            return -1;
        }

        result = (result << 4) | (unsigned)digit; //shift current value to left to make room for new value, append new to old with OR op
    }

    v->uval = result;  // write into the union
    return 0;
}

// converts the ASCII hex character c to binary
// returns the hex value of c if c is a valid hex digit, -1 otherwise
char hexDigit(char c) {
    if (c >= '0' && c <= '9') { //0-9
        return c - '0';
    } else if (c >= 'a' && c <= 'f') { //a-f lowercase
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') { //a-f uppercase
        return c - 'A' + 10;
    } else {
        return -1;
    }
}

// returns true if x has any even bit equal to 1, 0 otherwise
int any_even_one(unsigned x) {
    return (x & 0b01010101010101010101010101010101) != 0; //decided to hard code mask even if messier its a lot easier for me
}
// returns a mask indicating the position of the left most one in x
int leftmost_one(unsigned x) {
    if (x == 0) {
        return 0;
    }

    unsigned mask = 1u << 31;   // start checking from the top bit
    while ((x & mask) == 0) {
        mask >>= 1;
    }
    return mask;
}
// returns x shifted n positions to the left with the n most significant bits of x 
// inserted at the right of x
unsigned rotate_left(unsigned x, int n) {
    if (n == 0) return x;
    return (x << n) | (x >> (32 - n));
}
// returns x shifted n positions to the right with the n least significant bits of x 
// inserted at the left of x
unsigned rotate_right(unsigned x, int n) {
    if (n == 0) return x;
    return (x >> n) | (x << (32 - n));
}
// returns x+y if no overflow occurs
// returns TMAX if a positive overflow occurs
// returns TMIN if a negative overflow occurs
int saturating_add(int x, int y) {
    int sum = x + y;

    // positive overflow: two positives producing a negative result
    if (x > 0 && y > 0 && sum < 0) {
        return INT_MAX;
    }

    // negative overflow: two negatives producing a non-negative result
    if (x < 0 && y < 0 && sum >= 0) {
        return INT_MIN;
    }

    return sum;
}
// multiplies the binary representation of a float number f by 2
unsigned float_twice(unsigned f) { //I would not be able to do this without AI help
    unsigned sign = f & 0x80000000;
    unsigned exp  = (f >> 23) & 0xFF;
    unsigned mant = f & 0x7FFFFF;

    if (exp == 255) {
        return f;              // Inf or NaN: unchanged
    }
    if (exp == 0) {
        // zero or denormalized: shift mantissa left by 1
        mant <<= 1;
        return sign | mant;    // exponent field stays 0
    }
    // normalized: bump the exponent by 1
    exp++;
    if (exp == 255) {
        return sign | 0x7F800000;  // overflowed to infinity
    }
    return sign | (exp << 23) | mant;
}

// divides the binary representation of a float number f by 2
unsigned float_half(unsigned f) { //I would not be able to do this without AI help
    unsigned sign = f & 0x80000000;
    unsigned exp  = (f >> 23) & 0xFF;
    unsigned mant = f & 0x7FFFFF;

    if (exp == 255) {
        return f;   // Inf or NaN: unchanged
    }

    if (exp == 0) {
        // zero or denormalized: shift mantissa right by 1
        // (rounding to even on the bit shifted out, for correctness)
        mant = (mant >> 1) | (mant & 1);   // simple round: keep if odd
        return sign | mant;
    }

    if (exp == 1) {
        // smallest normalized number, halving pushes it into denormalized range
        // need to shift in the implicit leading 1 before shifting right
        mant = (mant | 0x800000) >> 1;
        return sign | mant;    // exponent becomes 0 (denormalized)
    }

    // normalized, exponent stays safely above the denormalized boundary
    exp--;
    return sign | (exp << 23) | mant;
}

//ai generated
int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
        printf("Invalid number of arguments\n");
        exit(0);
    }

    char *command = argv[1];
    union value v;

    // parse the first hex argument for every command
    if (read_hex(&v, argv[2]) == -1) {
        printf("Invalid hex value\n");
        exit(0);
    }

    if (strcmp(command, "even") == 0) {
        if (argc != 3) { printf("Invalid number of arguments\n"); exit(0); }
        printf(any_even_one(v.uval) ? "True\n" : "False\n");

    } else if (strcmp(command, "left") == 0) {
        if (argc != 3) { printf("Invalid number of arguments\n"); exit(0); }
        printf("%08x\n", leftmost_one(v.uval));

    } else if (strcmp(command, "lrotate") == 0 || strcmp(command, "rrotate") == 0) {
        if (argc != 4) { printf("Invalid number of arguments\n"); exit(0); }

        char *endptr;
        long n = strtol(argv[3], &endptr, 10);
        if (*endptr != '\0' || n < 0 || n > 31) {
            printf("Invalid number of shift positions\n");
            exit(0);
        }

        unsigned result = (strcmp(command, "lrotate") == 0)
                             ? rotate_left(v.uval, (int)n)
                             : rotate_right(v.uval, (int)n);
        printf("%08x\n", result);

    } else if (strcmp(command, "saturate") == 0) {
        if (argc != 4) { printf("Invalid number of arguments\n"); exit(0); }

        union value v2;
        if (read_hex(&v2, argv[3]) == -1) {
            printf("Invalid hex value\n");
            exit(0);
        }
        int sat = saturating_add(v.sval, v2.sval);
        printf("%08x %d\n", (unsigned)sat, sat);

    } else if (strcmp(command, "twice") == 0) {
        if (argc != 3) { printf("Invalid number of arguments\n"); exit(0); }
        {
            unsigned res = float_twice(v.uval);
            union value out; out.uval = res;
            printf("%08x ", res);
            unsigned exp = (res >> 23) & 0xFF;
            unsigned mant = res & 0x7FFFFF;
            int sign = (res & 0x80000000) != 0;
            if (exp == 255) {
                if (mant == 0) {
                    printf("%s\n", sign ? "-inf" : "inf");
                } else {
                    printf("%s\n", sign ? "-nan" : "nan");
                }
            } else {
                printf("%e\n", out.fval);
            }
        }

    } else if (strcmp(command, "half") == 0) {
        if (argc != 3) { printf("Invalid number of arguments\n"); exit(0); }
        {
            unsigned res = float_half(v.uval);
            union value out; out.uval = res;
            printf("%08x ", res);
            unsigned exp = (res >> 23) & 0xFF;
            unsigned mant = res & 0x7FFFFF;
            int sign = (res & 0x80000000) != 0;
            if (exp == 255) {
                if (mant == 0) {
                    printf("%s\n", sign ? "-inf" : "inf");
                } else {
                    printf("%s\n", sign ? "-nan" : "nan");
                }
            } else {
                printf("%e\n", out.fval);
            }
        }

    } else {
        printf("Invalid operation\n");
        exit(0);
    }

    return 0;
}