#include "hashmap.h"

struct sample
{
	int a;
	int b;
};


int main(int argc, char* argv[])
{
	struct sample s = {0, 0};
	struct sample* d;
	d = createHashMap(&s, 2);
	return 0;
}
