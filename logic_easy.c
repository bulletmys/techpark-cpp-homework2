#include <glob.h>
#include "logic_easy.h"

unsigned long long int sum_easy(const int* array, int size) {
    unsigned long long int sum = 0;

    for (size_t j = 0; j < size; ++j) {
        sum += array[j] % 1024;
    }

    return sum;
}
