#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Error: wrong args\n");
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    long n_to_process = strtol(argv[2], NULL, 10);

    if (n_to_process <= 1) {
        return 0;
    }

    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        return 0;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 2 * sizeof(double)) {
        close(fd);
        return 0;
    }

    long total_elements = file_size / sizeof(double);

    long limit = (n_to_process < total_elements) ? n_to_process : total_elements;

    lseek(fd, 0, SEEK_SET);

    double prev_out_val;
    double current_in_val;

    read(fd, &prev_out_val, sizeof(double));

    for (long i = 1; i < limit; ++i) {
        read(fd, &current_in_val, sizeof(double));
        double current_out_val = current_in_val - prev_out_val;
        lseek(fd, -sizeof(double), SEEK_CUR);
        write(fd, &current_out_val, sizeof(double));
        prev_out_val = current_out_val;
    }

    close(fd);

    return 0;
}
