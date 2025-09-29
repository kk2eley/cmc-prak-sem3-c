#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void
write_pensieve_format(int fd, unsigned int num)
{
    uint8_t buffer[4];

    unsigned int high_chunk = (num >> 12) & 0xFFF;
    unsigned int low_chunk = num & 0xFFF;

    buffer[0] = (high_chunk >> 8) & 0x0F;
    buffer[1] = high_chunk & 0xFF;

    buffer[2] = (low_chunk >> 8) & 0x0F;
    buffer[3] = low_chunk & 0xFF;

    write(fd, buffer, sizeof(buffer));
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Error: wrong args");
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        fprintf(stderr, "Error: cannot open file\n");
        return EXIT_FAILURE;
    }

    unsigned int input_num;
    while (scanf("%u", &input_num) == 1) {
        write_pensieve_format(fd, input_num);
    }

    close(fd);

    return EXIT_SUCCESS;
}
