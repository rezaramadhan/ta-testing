#include <openssl/bn.h>
#include "bn_test_tc.c"
#include "const.h"

void bn_func(int tc_num, int* opt) {
    BIGNUM* bn[BN_COUNT];
    BN_CTX* bn_ctx = BN_CTX_new();
    BIGNUM* result = BN_new();
    // printf("%d\n", len);
    for (int i = 0; i < BN_COUNT; i++) {
        bn[i] = NULL;
        BN_hex2bn(&bn[i], bn_hex[i][tc_num]);
    }

    if (opt[0])
        BN_add(result, bn[1], bn[2]);
    if (opt[1])
        BN_mul(result, bn[1], bn[2], bn_ctx);
    if (opt[2])
        BN_div(result, NULL, bn[1], bn[2], bn_ctx);
    if (opt[3])
        BN_exp(result, bn[1], bn[2], bn_ctx);
    if (opt[4])
        BN_mod_exp(result, bn[1], bn[2], bn[0], bn_ctx);

    BN_CTX_free(bn_ctx);
    BN_free(result);
    for (int i = 0; i < BN_COUNT; i++) {
        BN_free(bn[i]);
    }
}
