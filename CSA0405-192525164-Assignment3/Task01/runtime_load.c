#include <stdio.h>
#include <dlfcn.h>
int main(void) {
    void *h = dlopen("./libcrypto_demo.so", RTLD_NOW);
    if (!h) { fprintf(stderr, "Load failed: %s\n", dlerror()); return 1; }
    int (*fn)(int) = (int (*)(int)) dlsym(h, "encrypt_demo");
    if (!fn) { fprintf(stderr, "Symbol missing: %s\n", dlerror()); dlclose(h); return 1; }
    printf("Result: %d\n", fn(7));
    dlclose(h);
    return 0;
}
