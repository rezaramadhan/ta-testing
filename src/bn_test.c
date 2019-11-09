#include <openssl/bn.h>
// #include "bn_test_tc.c"
#include "const.h"

void print_opt(int* opt, int n) {
    printf("opt ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", opt[i]);
    }
    printf("\n");
}

void bn_func(int bn_size, int* opt) {
    BIGNUM* bn[BN_COUNT];
    BN_CTX* bn_ctx = BN_CTX_new();
    BIGNUM* result = BN_new();
    // printf("%d\n", len);
    for (int i = 1; i < BN_COUNT; i++) {
        bn[i] = BN_new();
        BN_pseudo_rand(bn[i], bn_size, -1, 0);
    }
    bn[0] = BN_new();
    BN_pseudo_rand(bn[0], bn_size/64, -1, 0);

    // print_opt(opt, 6);

    if (opt[0])
        BN_add(result, bn[1], bn[2]);
    if (opt[1])
        BN_mul(result, bn[1], bn[2], bn_ctx);
    if (opt[2])
        BN_div(result, NULL, bn[1], bn[0], bn_ctx);
    if (opt[3])
        BN_exp(result, bn[1], bn[2], bn_ctx);
    if (opt[4])
        BN_mod_mul(result, bn[1], bn[2], bn[0], bn_ctx);
    if (opt[5])
        BN_mod_exp(result, bn[1], bn[0], bn[2], bn_ctx);

    BN_CTX_free(bn_ctx);
    BN_free(result);
    for (int i = 0; i < BN_COUNT; i++) {
        BN_free(bn[i]);
    }
}
