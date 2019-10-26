#include <stdlib.h>
#include <time.h>
#include <values.h>
#include <unistd.h>
#include <assert.h>
#include "stdio.h"
#include "malloc.h"
#include "logic_easy.h"
#include "logic_pro.h"
#include "input_helpers.h"

#define ARRAY_SIZE 100*1024*1024

int main(int argc, char *argv[]) {
    size_t real_size;
    printf("Введите кол-во процессов\n");
    int num_of_forks = Input_int();
    while (num_of_forks < 1) {
        printf("Значение для кол-ва процессов должно быть > 0\n");
        num_of_forks = Input_int();
    }
    int *array = (int *) malloc(ARRAY_SIZE);

    if (argc > 1) {

        FILE *f = fopen(argv[1], "rt");
        int i = 0;
        while (!feof(f)) {
            fscanf(f, "%d", &array[i]);
            i++;
        }
        real_size = i;
        fclose(f);
    } else {
        printf("Введите кол-во элементов в массиве\n");
        real_size = Input_int();
        printf("Введите %zu элементов\n", real_size);
        for (size_t i = 0; i < real_size; ++i) {
            array[i] = Input_int();
        }

    }

    clock_t t = clock();

    unsigned long long easy_hash = sum_easy(array, real_size);

    double tt = (double) (clock() - t) / CLOCKS_PER_SEC;
    clock_t t2 = clock();

    unsigned long long pro_hash = sum_pro(array, real_size, num_of_forks);

    double tt2 = (double) (clock() - t2) / CLOCKS_PER_SEC;

    assert(easy_hash == pro_hash);

    printf(" Время работы простого алгоритма - %f\n", tt);
    printf(" Время работы сложного алгоритма - %f\n", tt2);

    //printf("Введите кол-во элементов в массиве - %zu\n", real_size);


    free(array);
}
