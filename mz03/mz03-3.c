#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Error: not enought values");
        return EXIT_FAILURE;
    }
    errno = 0;
    char *end_arg;
    long double current_rate = strtod(argv[1], &end_arg);
    if (errno || end_arg == argv[1] || *end_arg != '\0') {
        fprintf(stderr, "Error: cannot read number");
        return EXIT_FAILURE;
    }
    for (int i = 2; i < argc; ++i) {
        errno = 0;
        long double percent_change = strtod(argv[i], &end_arg);
        if (errno || end_arg == argv[i] || *end_arg != '\0') {
            fprintf(stderr, "Error: cannot read number");
            return EXIT_FAILURE;
        }
        current_rate = current_rate * (1.0 + percent_change / 100.0);
        current_rate = round(current_rate * 10000.0) / 10000.0;
    }
    printf("%.4Lf\n", current_rate);
    return EXIT_SUCCESS;
}
