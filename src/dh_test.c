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

    FILE *fp_param = NULL;
    char fparamname[100];
    sprintf(fparamname, "key/dh/%d_dh.pem", key_size);

    fp_param = fopen(fparamname, "r");

    if (fp_param == NULL) {
        printf("file err. Key %s doesn't exist.\n", fparamname);
        exit(1);
    }

    dh_alice =  PEM_read_DHparams(fp_param, NULL, NULL, NULL);

    fclose(fp_param);

    if (dh_alice == NULL)
        handle_error();


    /* Generate the public and private key pair */
    if(1 != DH_generate_key(dh_alice)) handleErrors();


    /* Receive the public key from the peer.*/
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

    OPENSSL_free(secret);
    DH_free(dh_alice);
    DH_free(dh_bob);
}
