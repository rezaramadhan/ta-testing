#include <openssl/bn.h>
#include <openssl/dh.h>

void handleErrors(char* msg) {
    printf("%s\n", msg);
    exit(1);
}

void dh_test() {
    DH *dh_alice;
    int codes;
    int secret_size;
    int key_size = 512;

    /* Generate the parameters to be used */
    if(NULL == (dh_alice = DH_new())) handleErrors("err_new");
    if(1 != DH_generate_parameters_ex(dh_alice, key_size, DH_GENERATOR_2, NULL)) handleErrors("err_gen_param");

    if(1 != DH_check(dh_alice, &codes)) handleErrors("err_check");
    if(codes != 0)
    {
        /* Problems have been found with the generated parameters */
        /* Handle these here - we'll just abort for this example */
        printf("DH_check failed\n");
        abort();
    }

    /* Generate the public and private key pair */
    if(1 != DH_generate_key(dh_alice)) handleErrors("err_gen_key");

    /* Receive the public key from the peer. In this example we're just hard coding a value */
    BIGNUM *pubkey = NULL;
    if(0 == (BN_dec2bn(&pubkey, "01234567890123456789012345678901234567890123456789"))) handleErrors("err_bn_conv");

    /* Compute the shared secret */
    unsigned char *secret;
    if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dh_alice))))) handleErrors("err_malloc_secret");

    if(0 > (secret_size = DH_compute_key(secret, pubkey, dh_alice))) handleErrors("err_compute_secret");

    /* Do something with the shared secret */
    /* Note secret_size may be less than DH_size(dh_alice) */
    printf("The shared secret is:\n");
    BIO_dump_fp(stdout, secret, secret_size);

    /* Clean up */
    OPENSSL_free(secret);
    BN_free(pubkey);
    DH_free(dh_alice);
}
