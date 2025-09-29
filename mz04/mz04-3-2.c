#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);

    long long current_val;
    off_t current_pos = 0;

    off_t min_pos = -1;
    long long min_val = 0;

    while (read(fd, &current_val, sizeof(long long)) == sizeof(long long)) {
        if (min_pos == -1 || current_val < min_val) {
            min_val = current_val;
            min_pos = current_pos;
        }
        current_pos += sizeof(long long);
    }

    if (min_pos != -1) {
        if (min_val != LLONG_MIN) {
            min_val = -min_val;
        }

        lseek(fd, min_pos, SEEK_SET);
        write(fd, &min_val, sizeof(long long));
    }

    close(fd);
    return 0;
}
