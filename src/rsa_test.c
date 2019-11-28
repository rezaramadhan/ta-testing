#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string.h>

void handle_error() {
    char err_str[2048];
    unsigned long err_code = ERR_get_error();

    ERR_error_string(err_code, err_str);
    printf("!!%s\n", err_str);

    exit(1);
}

void rsa_save_keys(RSA* rsa) {
    FILE *fp_public = NULL, *fp_private = NULL;
    int size_bits = RSA_size(rsa)*8;
    char fpubname[100];
    char fprivname[100];
    sprintf(fpubname, "key/rsa/%d_public.pem", size_bits);
    sprintf(fprivname, "key/rsa/%d_private.pem", size_bits);

    fp_public = fopen(fpubname, "w");
    fp_private = fopen(fprivname, "w");

    // 2. save public key
    if (!PEM_write_RSAPublicKey(fp_public, rsa))
        handle_error();

    // 3. save private key
    if (!PEM_write_RSAPrivateKey(fp_private, rsa, NULL, NULL, 0, NULL, NULL))
        handle_error();

    fclose(fp_public);
    fclose(fp_private);
}

void rsa_enc(int key_size) {
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();
    ERR_load_crypto_strings();

    // load keys
    RSA *rsa_priv = NULL;
    // rsa_pub = RSA_new();
    // rsa_priv = RSA_new();

    FILE *fp_private = NULL;
    char fprivname[100];
    sprintf(fprivname, "key/rsa/%d_private.pem", key_size);

    fp_private = fopen(fprivname, "r");

    if (fp_private == NULL) {
        printf("file err. Key %s doesn't exist.\n", fprivname);
        exit(1);
    }

    // printf("a\n");
    rsa_priv = PEM_read_RSAPrivateKey(fp_private, NULL, NULL, NULL);

    fclose(fp_private);

    if (rsa_priv == NULL)
        handle_error();

    // printf("%d %d\n", RSA_size(rsa_pub), RSA_size(rsa_priv));

    // public encrypt
    unsigned char *msg = malloc(sizeof(char)*key_size);
    RAND_bytes(msg, key_size);

    unsigned char *enc_msg;
    enc_msg = (char*) calloc(RSA_size(rsa_priv), 8);
    unsigned char *dec_msg;
    dec_msg = (char*) calloc(RSA_size(rsa_priv), 8);
    // // printf("%d\n", RSA_size(rsa));
    RSA_public_encrypt(strlen(msg), msg, enc_msg, rsa_priv, RSA_PKCS1_OAEP_PADDING);
    RSA_private_decrypt(RSA_size(rsa_priv), enc_msg, dec_msg, rsa_priv, RSA_PKCS1_OAEP_PADDING);

    // 4. free
    free(msg);
    free(enc_msg);
    free(dec_msg);

    RSA_free(rsa_priv);
}

void rsa_gen_key(int key_size, unsigned long e) {
    RSA *rsa = NULL;
    BIGNUM *bne = NULL;

    bne = BN_new();
    BN_set_word(bne,e);

    if (NULL == (rsa = RSA_new()))
        handle_error();

    if (0 == RSA_generate_key_ex(rsa, key_size, bne, NULL))
        handle_error();

    // save key
    // rsa_save_keys(rsa);

    RSA_free(rsa);
    BN_free(bne);
}
