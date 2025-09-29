#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Error: wrong args\n");
        exit(1);
    }

    const char *filename = argv[1];

    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error: cannot open file\n");
        exit(1);
    }

    long long min_val = LLONG_MAX;
    off_t min_pos = -1;
    off_t current_pos = 0;
    long long current_val;

    while (read(fd, &current_val, sizeof(current_val)) == sizeof(current_val)) {
        if (current_val < min_val) {
            min_val = current_val;
            min_pos = current_pos;
        }
        current_pos += sizeof(current_val);
    }

    if (min_pos != -1) {
        unsigned long long new_val = min_val;
        new_val = 1 + (~new_val);
        lseek(fd, min_pos, SEEK_SET);
        write(fd, &new_val, sizeof(new_val));
    }

    close(fd);

    return 0;
}
