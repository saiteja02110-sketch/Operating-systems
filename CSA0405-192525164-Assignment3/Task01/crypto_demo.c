/* Shared library: exports encrypt_demo used by runtime_load.c */
int encrypt_demo(int value) {
    return value ^ 0x5A;  /* simple demo XOR transform */
}
