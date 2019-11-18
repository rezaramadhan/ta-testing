/*
    How to run:
    test {bn|rsa|dh}

     bn option -> <tc_number> {add,mul,div,exp,modexp}
    rsa option -> <key_size> <exponential>
     dh option -> <key_size>
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "bn_test.c"
#include "rsa_test.c"
#include "dh_test.c"

typedef struct timespec time_s;

void print_result(time_s start, time_s finish) {
    double elapsed;
    elapsed = (finish.tv_sec - start.tv_sec)*1000000;
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000.0;

    fprintf(stdout, "%.2f\n", elapsed);
}


/*
        1              Operation not permitted
        2              No such file or directory
        3              No such process
        4              Interrupted system call
        5              I/O error
        6              No such device or address
        7              Argument list too long
        8              Exec format error
        9              Bad file number
        10             No child processes
        11             Try again
        12             Out of memory
        13             Permission denied
*/
void error(char * msg, int err_code) {
    fprintf(stderr, "_ERROR: %s\n", msg);
    exit(err_code);
}

void help() {
    printf("test {bn|rsa_gen|rsa_enc|dh} {test arguments}\n\n");
    printf("bn test arguments : <tc_number> {add,mul,div,exp,modexp}\n");
    printf("rsa_gen test arguments : <key_size> <exponential>\n");
    printf("rsa_enc test arguments : <message_size>\n");
    printf("dh test arguments : <key_size>\n\n");
}

void invalid_args() {
    help();
    error("invalid arguments", 7);
}

void process_opts(char* opt, int* i) {
    if (strcmp(opt, "add") == 0)
        i[0] = 1;
    else if (strcmp(opt, "mul") == 0)
        i[1] = 1;
    else if (strcmp(opt, "div") == 0)
        i[2] = 1;
    else if (strcmp(opt, "exp") == 0)
        i[3] = 1;
    else if (strcmp(opt, "modmul") == 0)
        i[4] = 1;
    else if (strcmp(opt, "modexp") == 0)
        i[5] = 1;
    else if (strcmp(opt, "mul_recursive") == 0)
        i[6] = 1;
    else if (strcmp(opt, "sqr") == 0)
        i[7] = 1;
    else
        error("invalid bn_test options", 1);

}

void run(char opt, char const *argv[]) {
    int a, b;
    if (0 == (a = strtol(argv[1], NULL, 10)))
        error("invalid str to int conversion", 1);

    if (opt == 'r')
        if (0 == (b = strtol(argv[2], NULL, 10)))
            error("invalid str to int conversion", 1);

    int i_arr[BN_OPR_COUNT] = {0};
    if (opt == 'b') {
        char *ptr, *bntest_opt = strdup(argv[2]);

        ptr = strtok(bntest_opt, ";");
        while(ptr != NULL) {
            process_opts(ptr, i_arr);

            ptr = strtok(NULL, ";");
        }
    }

    time_s start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    switch (opt) {
        case 'b': bn_func(a, i_arr); break;
        case 'r': rsa_gen_key(a, b); break;
        case 'R': rsa_enc(a); break;
        case 'd': dh_test(a); break;
        default: error("invalid case args", 3);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);

    print_result(start, finish);
}

int main(int argc, char const *argv[]) {
    if (argc < 2 || argc > 4) invalid_args();

    if (strcmp(argv[1], "bn") == 0 && argc == 4)
        run('b', &argv[1]);
    else if (strcmp(argv[1], "rsa_gen") == 0 && argc == 4)
        run('r', &argv[1]);
    else if (strcmp(argv[1], "rsa_enc") == 0 && argc == 3)
        run('R', &argv[1]);
    else if (strcmp(argv[1], "dh") == 0 && argc == 3)
        run('d', &argv[1]);
    else invalid_args();

    return 0;
}
