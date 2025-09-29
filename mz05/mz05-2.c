#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

char BASE_RIGHTS_STR[] = "rwxrwxrwx";
enum
{
    LENGTH = sizeof(BASE_RIGHTS_STR) - 1,
    UNVALID_BITS = ~((1u << LENGTH) - 1),
    FIRST_BIT = 1u << (LENGTH - 1),
    OCTAL_BASE = 8,
};

void
handle_error(char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

int
main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        char result[LENGTH + 1];
        errno = 0;
        char *endptr = NULL;
        long num = 0;
        num = strtol(argv[i], &endptr, OCTAL_BASE);
        if (errno || *endptr || endptr == argv[i] || (UNVALID_BITS & num)) {
            handle_error("Invalid rights");
        }
        for (int j = 0; j < LENGTH; ++j) {
            if (num & FIRST_BIT) {
                result[j] = BASE_RIGHTS_STR[j];
            } else {
                result[j] = '-';
            }
            num <<= 1;
        }
        printf("%s\n", result);
    }
}
