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
    for (int i = 0; i < BN_COUNT; i++) {
        bn[i] = BN_new();
    }
    //modulo, must be bigger than the other
    BN_pseudo_rand(bn[0], bn_size + 2, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ODD);

    //equal size
    BN_pseudo_rand(bn[1], bn_size, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    BN_pseudo_rand(bn[2], bn_size, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);

    //smaller one
    BN_pseudo_rand(bn[3], bn_size/64, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    if (bn_size > 128) {
        int nsize = bn_size - 128;
        BN_pseudo_rand(bn[4], nsize, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    } else
        BN_pseudo_rand(bn[4], bn_size, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);

    //constant size
    BN_pseudo_rand(bn[5], CONST_SIZE_BN, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ANY);
    BN_pseudo_rand(bn[6], CONST_SIZE_BN+2, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ODD);

    // print_opt(opt, 8);

    if (opt[0]){
        BN_add(result, bn[1], bn[2]);
    }
    if (opt[1]){ //mul normal
        BN_mul(result, bn[1], bn[4], bn_ctx);
    }
    if (opt[2]){
        BN_div(result, NULL, bn[1], bn[3], bn_ctx);
    }
    if (opt[3]){
        BN_exp(result, bn[5], bn[1], bn_ctx);
    }
    if (opt[4]){
        BN_mod_mul(result, bn[1], bn[2], bn[6], bn_ctx);
    }
    if (opt[5]){
        // printf("modexp\n");
        BN_mod_exp(result, bn[1], bn[2], bn[0], bn_ctx);
    }
    if (opt[6]){ //mul_recursive
        BN_mul(result, bn[1], bn[2], bn_ctx);
    }
    if (opt[7]){
        BN_sqr(result, bn[1], bn_ctx);
    }

    BN_CTX_free(bn_ctx);
    BN_free(result);
    for (int i = 0; i < BN_COUNT; i++) {
        BN_free(bn[i]);
    }
}
