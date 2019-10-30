#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <string.h>

void rsa_save_keys(RSA* rsa) {
    BIO *bp_public = NULL, *bp_private = NULL;

    // 2. save public key
    bp_public = BIO_new_file("public.pem", "w+");
    PEM_write_bio_RSAPublicKey(bp_public, rsa);

    // 3. save private key
    bp_private = BIO_new_file("private.pem", "w+");
    PEM_write_bio_RSAPrivateKey(bp_private, rsa, NULL, NULL, 0, NULL, NULL);


    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
}

void rsa_test(int key_size, unsigned long e) {
    RSA *rsa = NULL;
    BIGNUM *bne = NULL;

    // 1. generate rsa key
    bne = BN_new();
    BN_set_word(bne,e);

    rsa = RSA_new();
    RSA_generate_key_ex(rsa, key_size, bne, NULL);

    // save keys
    // rsa_save_keys(rsa);

    // public encrypt
    char *msg = "THIS IS A MESSAGE\0";
    char enc_msg[RSA_size(rsa)];
    char dec_msg[RSA_size(rsa)];
    // printf("%d\n", RSA_size(rsa));
    RSA_public_encrypt(strlen(msg), msg, enc_msg, rsa, RSA_PKCS1_OAEP_PADDING);
    RSA_private_decrypt(RSA_size(rsa), enc_msg, dec_msg, rsa, RSA_PKCS1_OAEP_PADDING);
    // printf("%s\n", dec_msg);
    // 4. free

    RSA_free(rsa);
    BN_free(bne);
}
