#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *f = fopen("testdata.bin", "wb");
    if (!f) return 1;

    for (int i = 1; i <= 9; ++i) {
        double val = (double)i;
        fwrite(&val, sizeof(double), 1, f);
    }

    fclose(f);
    printf("Файл 'testdata.bin' создан.\n");
    return 0;
}

