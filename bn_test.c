#include <openssl/bn.h>
#include "bn_test_tc.c"
#include "const.h"

void bn_func() {
    BIGNUM* bn[BN_COUNT];
    BN_CTX* bn_ctx = BN_CTX_new();
    BIGNUM* result = BN_new();
    // printf("%d\n", len);
    for (int i = 0; i < BN_COUNT; i++) {
        bn[i] = NULL;
        BN_hex2bn(&bn[i], bn_hex[i][TC_NUM]);
    }

    if (T_ADD)
        BN_add(result, bn[2], bn[1]);
    if (T_MUL)
        BN_mul(result, bn[2], bn[1], bn_ctx);
    if (T_DIV)
        BN_div(result, NULL, bn[1], bn[2], bn_ctx);
    if (T_EXP)
        BN_exp(result, bn[1], bn[2], bn_ctx);
    if (T_MOD_EXP)
        BN_mod_exp(result, bn[1], bn[2], bn[0], bn_ctx);

    BN_CTX_free(bn_ctx);
    BN_free(result);
    for (int i = 0; i < BN_COUNT; i++) {
        BN_free(bn[i]);
    }
}
