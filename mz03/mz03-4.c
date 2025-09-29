#include <stdio.h>

int
satsum(int v1, int v2)
{
    enum
    {
        MY_INT_MAX = (int) ((~(unsigned int) 0) >> 1),
        MY_INT_MIN = ~MY_INT_MAX
    };
    if (v1 > 0 && v2 > 0 && v1 > MY_INT_MAX - v2) {
        return MY_INT_MAX;
    }
    if (v1 < 0 && v2 < 0 && v1 < MY_INT_MIN - v2) {
        return MY_INT_MIN;
    }
    return v1 + v2;
}

int 
main ()
{
    printf("%d\n", satsum(1,2));
    printf("%d\n", satsum((int) ((~(unsigned int) 0) >> 1), 10));
    printf("%d\n", satsum(~((int) ((~(unsigned int) 0) >> 1)), -10));
}
