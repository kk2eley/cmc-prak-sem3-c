#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>

struct Elem
{
    struct Elem *next;
    char *str;
};

const int DECIMAL_SYSTEM = 10;

struct Elem *
dup_elem(struct Elem *head)
{
    struct Elem dummy = {};
    dummy.next = head;
    struct Elem *prev = &dummy;
    struct Elem *cur = head;
    while (cur) {
        char *num_end = NULL;
        errno = 0;
        long long num = strtoll(cur->str, &num_end, DECIMAL_SYSTEM);
        struct Elem *new_node = (struct Elem *) malloc(sizeof(struct Elem));
        if (!errno && new_node && *num_end == '\0' && (int32_t) num == num) {
            int len = snprintf(NULL, 0, "%lld", num + 1);
            char *snum = (char *) malloc(len + 1);
            if (!snum) {
                fprintf(stderr, "Error: cannot allocate memmory for str");
                return head;
            }
            sprintf(snum, "%lld", num + 1);
            new_node->str = snum;
            new_node->next = cur;
            prev->next = new_node;
        } else if (!new_node) {
            fprintf(stderr, "Error: cannot allocate memmory for new node");
            free(new_node);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }

    return dummy.next;
}
