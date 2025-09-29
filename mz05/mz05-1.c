#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

enum
{
    KIBIBYTE = 1024
};

int
main(int argc, char **argv)
{
    uint64_t result = 0;
    for (int i = 0; i < argc; ++i) {
        struct stat info;
        if (lstat(argv[i], &info) == -1) {
            fprintf(stderr, "Error: cannot find file\n");
            return 1;
        }
        if (S_ISREG(info.st_mode) && info.st_nlink == 1 && info.st_size % KIBIBYTE == 0) {
            result += info.st_size;
        }
    }
    printf("%llu\n", result);
}
