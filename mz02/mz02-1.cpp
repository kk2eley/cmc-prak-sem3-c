#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 130

int
main()
{
    char s1[MAX_BUFFER_SIZE];
    char s2[MAX_BUFFER_SIZE];
    char s3[MAX_BUFFER_SIZE];

    if (fgets(s1, sizeof(s1), stdin) == NULL || fgets(s2, sizeof(s2), stdin) == NULL ||
        fgets(s3, sizeof(s3), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }

    s1[strcspn(s1, "\n")] = '\0';
    s2[strcspn(s2, "\n")] = '\0';
    s3[strcspn(s3, "\n")] = '\0';

    printf("[Host:%s,Login:%s,Password:%s]\n", s1, s2, s3);

    return 0;
}
