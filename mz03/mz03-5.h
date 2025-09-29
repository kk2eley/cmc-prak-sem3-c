struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *dup_elem(struct Elem *head);

