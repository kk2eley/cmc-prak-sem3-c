#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Error: wrong args\n");
        return EXIT_FAILURE;
    }

    const char *file1_name = argv[1];
    const char *file2_name = argv[2];
    int32_t mod = (int32_t) strtol(argv[3], NULL, 10);

    if (mod <= 0) {
        fprintf(stderr, "Error: wrong mod\n");
        return EXIT_FAILURE;
    }

    int fd_in = open(file1_name, O_RDONLY);
    if (fd_in == -1) {
        fprintf(stderr, "Error: cannot open file");
        return EXIT_FAILURE;
    }

    int fd_out = open(file2_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        fprintf(stderr, "Error: cannot open or read file");
    }

    long long x = 1;
    long long sum_sq = 0;
    unsigned char byte;

    while (read(fd_in, &byte, sizeof(byte)) == sizeof(byte)) {
        for (int i = 0; i < 8; ++i) {
            long long term = ((long long) x * x) % mod;
            sum_sq = (sum_sq + term) % mod;

            if ((byte >> i) & 1) {
                int32_t result = sum_sq;
                if (write(fd_out, &result, sizeof(result)) != sizeof(result)) {
                    close(fd_in);
                    close(fd_out);
                    fprintf(stderr, "Error: cannot write to file\n");
                    return EXIT_FAILURE;
                }
            }

            x++;
        }
    }

    close(fd_in);
    close(fd_out);

    return 0;
}
