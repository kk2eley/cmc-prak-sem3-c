#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

enum
{
    DECIMAL_SYSTEM = 10
};

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
dup_elem(struct Elem *head)
{
    struct Elem dummy = {0};
    dummy.next = head;

    struct Elem *prev = &dummy;
    struct Elem *cur = head;

    while (cur != NULL) {
        if (cur->str == NULL) {
            prev = cur;
            cur = cur->next;
            continue;
        }

        errno = 0;
        char *eptr = NULL;
        long num = strtol(cur->str, &eptr, DECIMAL_SYSTEM);

        bool should_duplicate = true;

        if (errno || *eptr || eptr == cur->str || (int32_t) num != num) {
            should_duplicate = false;
        }

        if (should_duplicate && (INT32_MAX - 1 < num)) {
            should_duplicate = false;
        }

        if (should_duplicate) {
            int len = snprintf(NULL, 0, "%ld", num + 1) + 1;
            char *new_str = calloc(len, sizeof(*new_str));
            if (new_str == NULL) {
                fprintf(stderr, "Error: can't allocate memory for string\n");
                exit(1);
            }
            snprintf(new_str, len, "%ld", num + 1);

            struct Elem *new_node = malloc(sizeof(*new_node));
            if (new_node == NULL) {
                fprintf(stderr, "Error: can't allocate memory for node\n");
                free(new_str);
                exit(1);
            }
            new_node->str = new_str;

            new_node->next = cur;
            prev->next = new_node;
        }

        prev = cur;
        cur = cur->next;
    }

    return dummy.next;
}
