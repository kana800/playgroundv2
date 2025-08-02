#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("Page size: %ld bytes\n", sysconf(_SC_PAGESIZE));
}
