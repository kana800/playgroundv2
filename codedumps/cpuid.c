#include <cpuid.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int eax, ebx, ecx, edx;
	char vendor[13];
	__cpuid(0, eax, ebx, ecx, edx);
	memcpy(vendor, &ebx, 4);
	memcpy(vendor + 4, &edx, 4);
	memcpy(vendor + 8, &ecx, 4);
	vendor[12] = '\0';
	printf("CPU: %s\n", vendor);
	return 0;
}
