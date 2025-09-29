#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *f = fopen("testdata.bin", "rb");
    if (!f) return 1;

    double val;
    while (fread(&val, sizeof(double), 1, f) == 1) {
        printf("%.1f ", val);
    }
    printf("\n");

    fclose(f);
    return 0;
}

