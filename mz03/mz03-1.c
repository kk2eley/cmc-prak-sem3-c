#include <stdio.h>

typedef int STYPE;
typedef unsigned int UTYPE;

STYPE
bit_reverse(STYPE value)
{
    UTYPE count = 0;
    count = ~count;
    UTYPE uvalue = value;
    UTYPE result = 0;
    while (count > 0) {
        result = result << 1;
        result += uvalue & 1;
        uvalue = uvalue >> 1;
        count = count >> 1;
    }
    return (STYPE) result;
}

int
main()
{
    STYPE n = 8;
    printf("%d", bit_reverse(n));
}
