#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>

#include <sys/stat.h>

uint16_t swap_uint16( uint16_t val ) 
{
    return (val << 8) | (val >> 8 );
}

struct Scanline {
    unsigned int bits: 16;
};

struct XHdata {
    struct Scanline scanlines[7];
    uint16_t XW;
    uint16_t HD;
    uint16_t XH;
};

#pragma pack()
struct AL {
    uint16_t height;
    unsigned int proportional: 1;
    unsigned int baseline: 7;
    uint16_t maxWidth;
    uint16_t pointers[377]; /* 256? */
    /*struct XHdata chars[10];*/
};
#pragma pack(pop)

int main(int argc, char *argv[])
{
    int fd;
    struct stat statbuf;
    size_t file_size;

    assert(argc == 2);
    
    fd = open(argv[1], O_RDONLY);
    assert(fd > 0);

    fstat(fd, &statbuf);
    file_size = statbuf.st_size;

    assert(file_size > sizeof(struct AL));

    struct AL al;

    int bytes_read = read(fd, &al, sizeof(al));
    assert(bytes_read == sizeof(al));

    printf("Height: %i\n", swap_uint16(al.height));
    printf("Is %sproportional\n", al.proportional ? "" : "not");
    printf("Baseline: %i\n", al.baseline);
    printf("maxWidth: %i\n", swap_uint16(al.maxWidth));

    int i;
    for (i = 0; i < 10; i++) {
        printf("%i\n", swap_uint16(al.pointers[i]));
    }

    return EXIT_SUCCESS;
}
