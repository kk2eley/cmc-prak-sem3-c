
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input.\n");
        return 1;
    }
    if (n <= 1) {
        fprintf(stderr, "Error: N must be greater than 1.\n");
        return 1;
    }

    int *div = (int *) malloc(n * sizeof(int));
    if (div == NULL) {
        fprintf(stderr, "Error: Failed to allocate memmory.\n");
        return 1;
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            if ((i * j) % n == 1) {
                div[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            printf("%d ", (div[j] * i) % n);
        }
        printf("\n");
    }

    free(div);
}
