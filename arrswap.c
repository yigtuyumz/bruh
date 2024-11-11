#include <stdio.h>

void
arrswap(int *arr, size_t sz)
{
    size_t s;       // swap count

    if (sz % 2) {
        s = (sz - 1) / 2;
    } else {
        s = sz / 2;
    }

    size_t i = 0;       // start index
    int temp;           // temp value for swapping

    while (s--) {
        temp = arr[i];
        arr[i++] = arr[--sz];
        arr[sz] = temp;
    }
}

void
print_arr(int arr[], size_t sz)
{
    size_t i = 0;

    while (i < sz) {
        printf("%d\n", arr[i]);
        i++;
    }
}


int
main(int argc, char *argv[])
{
    int arr[6] = {1, 2, 3, 4, 5, 6};

    arrswap(arr, 6);

    print_arr(arr, 6);

    return (0);
}

