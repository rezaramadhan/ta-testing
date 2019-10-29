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

    printf("%.2f\n", elapsed);
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
    printf("_ERROR: %s\n", msg);
    exit(err_code);
}

void help() {
    printf("test {bn|rsa|dh} {test arguments}\n\n");
    printf(" bntest arguments : <tc_number> {add,mul,div,exp,modexp}\n");
    printf("rsatest arguments : <key_size> <exponential>\n");
    printf(" dhtest arguments : <key_size>\n\n");
}

void invalid_args() {
    help();
    error("invalid arguments", 7);
}

void run(char opt, char const *argv[]) {
    int a;
    printf("%s\n", argv[0]);
    if (0 == (a = strtol(argv[1], NULL, 10)))
        error("invalid str to int conversion", 1);

    time_s start, finish;

    clock_gettime(CLOCK_MONOTONIC, &start);
    switch (opt) {
        case 'b': bn_func(); break;
        case 'r': rsa_test(); break;
        case 'd': dh_test(a); break;
        default: error("invalid case args", 3);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);

    print_result(start, finish);
}

int main(int argc, char const *argv[]) {
    if (argc > 3 || argc < 2) invalid_args();

    if (strcmp(argv[1], "bn") == 0 && argc == 4)
        run('b', &argv[1]);
    else if (strcmp(argv[1], "rsa") == 0 && argc == 4)
        run('r', &argv[1]);
    else if (strcmp(argv[1], "dh") == 0 && argc == 3)
        run('d', &argv[1]);
    else invalid_args();

    return 0;
}
