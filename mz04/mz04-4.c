#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

struct MyNode
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};
enum
{
    NODE_SIZE = sizeof(int32_t) * 3
};

int32_t
convert_from_be(const unsigned char *buf)
{
    uint32_t val =
        ((uint32_t) buf[0] << 24) | ((uint32_t) buf[1] << 16) | ((uint32_t) buf[2] << 8) | ((uint32_t) buf[3]);
    return (int32_t) val;
}

ssize_t
read_fully(int fd, void *buf, size_t count)
{
    char *cbuf = (char *) buf;
    size_t total_read = 0;
    while (total_read < count) {
        ssize_t bytes_read = read(fd, cbuf + total_read, count - total_read);
        if (bytes_read == 0) {
            break;
        }
        if (bytes_read == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        total_read += bytes_read;
    }
    return total_read;
}

void
traverse_tree(int fd, int32_t node_idx)
{
    off_t offset = (off_t) node_idx * NODE_SIZE;
    if (lseek(fd, offset, SEEK_SET) == -1) {
        exit(1);
    }

    unsigned char buffer[NODE_SIZE];

    ssize_t bytes_read = read_fully(fd, buffer, sizeof(buffer));

    if (node_idx == 0 && bytes_read == 0) {
        return;
    }
    if (bytes_read == -1) {
        exit(1);
    }
    if (bytes_read != sizeof(buffer)) {
        exit(1);
    }

    struct MyNode current_node;
    current_node.key = convert_from_be(&buffer[0]);
    current_node.left_idx = convert_from_be(&buffer[4]);
    current_node.right_idx = convert_from_be(&buffer[8]);

    if (current_node.right_idx != 0) {
        traverse_tree(fd, current_node.right_idx);
    }

    printf("%d\n", current_node.key);

    if (current_node.left_idx != 0) {
        traverse_tree(fd, current_node.left_idx);
    }
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }

    traverse_tree(fd, 0);

    close(fd);
    return 0;
}
