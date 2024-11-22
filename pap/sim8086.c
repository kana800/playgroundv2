#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) 
{
    // TODO add error checking here
    int fd = open(argv[1], O_RDONLY);
    if (fd) {
        int len = lseek(fd, 0, SEEK_END);
        char* content = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
        fprintf(stdout, "%x\n", content);
    }
    return 0;
}
