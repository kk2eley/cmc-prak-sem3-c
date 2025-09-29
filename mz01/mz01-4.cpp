#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int
sort_even_odd_compare(const void *a, const void *b)
{
    int int_a = *(const int *) a;
    int int_b = *(const int *) b;

    int is_a_even = (int_a % 2 == 0);
    int is_b_even = (int_b % 2 == 0);

    if (is_a_even && is_b_even) {
        if (int_a < int_b) {
            return -1;
        }
        if (int_a > int_b) {
            return 1;
        }
        return 0;
    }

    if (!is_a_even && !is_b_even) {
        if (int_a > int_b) {
            return -1;
        }
        if (int_a < int_b) {
            return 1;
        }
        return 0;
    }

    if (is_a_even) {
        return -1;
    } else {
        return 1;
    }
}

void
sort_even_odd(size_t count, int *data)
{
    if (count == 0) {
        return;
    }
    qsort(data, count, sizeof(int), sort_even_odd_compare);
}
