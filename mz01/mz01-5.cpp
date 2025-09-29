#include <stdio.h>
#include <stdlib.h>

enum AppConstants
{
    SUCCESS_EXIT_CODE = 0,
    FAILURE_EXIT_CODE = 1,

    EXPECTED_SCANF_ARGS = 1,
    MIN_N_VALUE = 0,
    MAX_N_VALUE = 10,

    INFINITE_LOOP_CONDITION = 1,
    SEQUENCE_START_VALUE = 1,
    PIVOT_SEARCH_OFFSET = 2
};

void
print_array(int *array, int n)
{
    for (int i = 0; i < n; ++i) {
        printf("%d", array[i]);
    }
    printf("\n");
}

int
main()
{
    int n = 0;
    if (scanf("%d", &n) != EXPECTED_SCANF_ARGS) {
        fprintf(stderr, "Error: Invalid input.\n");
        return FAILURE_EXIT_CODE;
    }
    if (n <= MIN_N_VALUE || n >= MAX_N_VALUE) {
        fprintf(stderr, "Error: N must be greater than 0 and less than 10.\n");
        return FAILURE_EXIT_CODE;
    }
    int *array = (int *) malloc(n * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Error: Failed to allocate memmory.\n");
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        array[i] = i + SEQUENCE_START_VALUE;
    }

    print_array(array, n);

    while (INFINITE_LOOP_CONDITION) {
        int i = n - PIVOT_SEARCH_OFFSET;
        while (i >= 0 && array[i] >= array[i + 1]) {
            --i;
        }

        if (i < 0) {
            break;
        }

        int j = n - 1;
        while (array[j] <= array[i]) {
            --j;
        }

        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;

        ++i;
        j = n - 1;
        while (i < j) {
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            ++i;
            --j;
        }

        print_array(array, n);
    }

    free(array);

    return SUCCESS_EXIT_CODE;
}
