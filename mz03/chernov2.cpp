

int
main(int argc, char *argv)
{
    VEC_T(int) vec = {};
    int n; 
    for (int i = 0; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            exit(1);
        }
        fscanf(*argv[i]);
        while (fscanf(f, "%d", &n) == 1) {
            VEC_PUSH(vec, n);
        }
        fclose(f);        
    }

}
