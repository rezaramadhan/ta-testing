#include <stdio.h>
#include <time.h>
#include "bn_test.c"
#include "rsa_test.c"

typedef struct timespec time_s;

void print_result(time_s start, time_s finish) {
    double elapsed;
    elapsed = (finish.tv_sec - start.tv_sec)*1000000;
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000.0;

    printf("%.2f\n", elapsed);
}

int main(int argc, char const *argv[]) {
    time_s start, finish;

    clock_gettime(CLOCK_MONOTONIC, &start);
    bn_func();
    rsa_test();
    clock_gettime(CLOCK_MONOTONIC, &finish);

    print_result(start, finish);

    return 0;
}
