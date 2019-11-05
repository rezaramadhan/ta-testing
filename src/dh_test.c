#include <openssl/bn.h>
#include <openssl/dh.h>

void handleErrors() {
    char err_str[2048];
    unsigned long err_code = ERR_get_error();

    ERR_error_string(err_code, err_str);
    printf("!!%s\n", err_str);

    exit(1);
}

void dh_test(int key_size) {
    DH *dh_alice, *dh_bob;
    int codes;
    int secret_size;

    /* Generate the parameters to be used */
    if(NULL == (dh_alice = DH_new())) handleErrors();
    if(NULL == (dh_bob = DH_new())) handleErrors();

    if(1 != DH_generate_parameters_ex(dh_alice, key_size, DH_GENERATOR_2, NULL)) handleErrors();
    //
    // // if(1 != DH_check(dh_alice, &codes)) handleErrors();
    // // if(codes != 0)
    // // {
    // //     printf("DH_check failed\n");
    // //     abort();
    // // }
    //
    // /* Generate the public and private key pair */
    if(1 != DH_generate_key(dh_alice)) handleErrors();
    //
    // /* Receive the public key from the peer.*/
    BIGNUM *p, *q, *g;
    p = BN_dup(DH_get0_p(dh_alice));
    q = BN_dup(DH_get0_q(dh_alice));
    g = BN_dup(DH_get0_g(dh_alice));
    //
    DH_set0_pqg(dh_bob, p, q, g);
    //
    if(1 != DH_generate_key(dh_bob)) handleErrors();
    const BIGNUM *pubkey = DH_get0_pub_key(dh_bob);

    /* Compute the shared secret */
    unsigned char *secret;
    if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dh_alice))))) handleErrors();

    if(0 > (secret_size = DH_compute_key(secret, pubkey, dh_alice))) handleErrors();

    // /* Do something with the shared secret */
    // // printf("The shared secret is:\n");
    // // BIO_dump_fp(stdout, secret, secret_size);
    // // BIGNUM* bnsecret = NULL;
    // // bnsecret = BN_bin2bn(secret, strlen(secret), NULL);
    // // printf("secret_length %d bit\n", BN_num_bits(bnsecret));
    // /* Clean up */
    OPENSSL_free(secret);
    DH_free(dh_alice);
    DH_free(dh_bob);
}
