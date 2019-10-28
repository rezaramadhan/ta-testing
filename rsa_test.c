#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <string.h>

void rsa_test() {
    RSA *rsa = NULL;
    BIGNUM *bne = NULL;

    int bits = 4096;
    unsigned long e = RSA_F4;

    // 1. generate rsa key
    bne = BN_new();
    BN_set_word(bne,e);

    rsa = RSA_new();
    RSA_generate_key_ex(rsa, bits, bne, NULL);

    // save keys
    // rsa_save_keys(rsa);

    // public encrypt
    char *msg = "THIS IS A MESSAGE\0";
    char enc_msg[RSA_size(rsa)];
    char dec_msg[RSA_size(rsa)];
    printf("%d\n", RSA_size(rsa));
    RSA_public_encrypt(strlen(msg), msg, enc_msg, rsa, RSA_PKCS1_OAEP_PADDING);
    RSA_private_decrypt(RSA_size(rsa), enc_msg, dec_msg, rsa, RSA_PKCS1_OAEP_PADDING);
    printf("%s\n", dec_msg);
    // 4. free

    RSA_free(rsa);
    BN_free(bne);
}
