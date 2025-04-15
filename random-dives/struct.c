#include <stdio.h>

struct foo {
    int a;
    int b;
    int c;
};

int main(int argc,char* argv[])
{
    struct foo a;
    struct foo b;

    printf("size of %ld %ld\n", sizeof(a), sizeof(b));
    printf("substracted foo(a) - foo(b) = %ld\n", a-b);

    return 0;
}
