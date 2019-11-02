#include <openssl/bn.h>
#include <openssl/dh.h>

void handleErrors(char* msg) {
    printf("%s\n", msg);
    exit(1);
}

void dh_test(int key_size) {
    // DH *dh_alice, *dh_bob;
    // int codes;
    // int secret_size;
    //
    // /* Generate the parameters to be used */
    // if(NULL == (dh_alice = DH_new())) handleErrors("err_new");
    // if(1 != DH_generate_parameters_ex(dh_alice, key_size, DH_GENERATOR_2, NULL)) handleErrors("err_gen_param");
    //
    // // if(1 != DH_check(dh_alice, &codes)) handleErrors("err_check");
    // // if(codes != 0)
    // // {
    // //     printf("DH_check failed\n");
    // //     abort();
    // // }
    //
    // /* Generate the public and private key pair */
    // if(1 != DH_generate_key(dh_alice)) handleErrors("err_gen_key");
    //
    // /* Receive the public key from the peer.*/
    // // if(NULL == ()) handleErrors("err_new");
    // dh_bob = DH_new()
    // dh_bob->p = BN_dup(dh_alice->p);
    // dh_bob->q = BN_dup(dh_alice->q);
    // dh_bob->g = BN_dup(dh_alice->g);
    //
    //
    // if(1 != DH_generate_key(dh_bob)) handleErrors("err_gen_key");
    // BIGNUM *pubkey = dh_bob->pub_key;
    //
    // /* Compute the shared secret */
    // unsigned char *secret;
    // if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dh_alice))))) handleErrors("err_malloc_secret");
    //
    // if(0 > (secret_size = DH_compute_key(secret, pubkey, dh_alice))) handleErrors("err_compute_secret");
    //
    // /* Do something with the shared secret */
    // // printf("The shared secret is:\n");
    // // BIO_dump_fp(stdout, secret, secret_size);
    // // BIGNUM* bnsecret = NULL;
    // // bnsecret = BN_bin2bn(secret, strlen(secret), NULL);
    // // printf("secret_length %d bit\n", BN_num_bits(bnsecret));
    // /* Clean up */
    // OPENSSL_free(secret);
    // DH_free(dh_alice);
    // DH_free(dh_bob);
}
