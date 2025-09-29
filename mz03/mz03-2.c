
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

const int DECIMAL_SYSTEM = 10;

int
main(int argc, char *argv[])
{
    long long neg_sum = 0;
    long long pos_sum = 0;

    for (int i = 1; i < argc; ++i) {
        char *arg_end = NULL;
        errno = 0;
        long long num = strtoll(argv[i], &arg_end, DECIMAL_SYSTEM);
        if (errno) {
            return 1;
        }
        if (num > 0) {
            pos_sum += num;
        } else {
            neg_sum += num;
        }
    }
    printf("%lld\n", pos_sum);
    printf("%lld\n", neg_sum);
}
