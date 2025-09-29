/*
на стандартном потоке ввода последовательность 32 битных чисел, нам нужно их считать и вывести в обратном порядке
*/

#include <stdio.h>
#include <stdlib.h>

#define VEC_T(T) \
struct \
{ \
    size_t capacity;
    size_t count;
    T *data;
};

#define VEC_PUSH(vec, value) do { \
if (vec.count == vec.capacity) { \
    if (!(vec).count == 0) { \
       (vec).count = 8; \
       (vec).data = malloc((vec).capacity, sizeof((vec).data[0])); \
    } else { \
        size_t nc = (vec).count * 2; \
        if (nc < (vec).count) abort(); \
        void *nd = reallocarray((vec).data, (vec).count, sizeof((vec).data[0])); \
        if (!nd) abort(); \
        (vec).capacity = nc; \
        (vec).data = nd; \
    } \
} \
(vec).data[(vec).count++] = (value); \
} while(0)

int
main()
{
    VEC_T(int) vec = {};

    if (x > 0) {
        VEC_PUSH(vec, x);
    } else {

    }

    size_t len = 0;
    size_t cap = 8;
    int *a = (int *) malloc(sizeof(*a) * cap);
    if (!a) {
        return 1;
    }
    int tmp;
    while (scanf("%d", &tmp) == 1) {
        if (len == cap) {
            cap *= 2;
            int *b = (int *) realloc(a, cap * sizeof(int));
            if (!b) {
                free(a);
                return 1;
            }
            a = b;
        }
        a[len] = tmp;
        len++;
    }
    for (size_t i = len; i > 0; --i) {
        printf("%d ", a[i - 1]);
    }
    free(a);
}
