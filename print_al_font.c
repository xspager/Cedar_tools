#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <sys/stat.h>

/*
 * Print Xerox Alto fonts AL format in ascii art
 *
 * Font formats from the era are documented in http://www.bitsavers.org/pdf/xerox/alto/printing/AltoFontFormats_Oct1980.pdf
 *
 */

/*
 * charData can be AT MOST 16 horizonal bits, therefore the structure size can and do varries
 * 377b = 255 = 0xFF
 * charactes can have extensions, more bits
 * if XW <= 16 char width = (XW-1)/2
 * else XW is code for the extension charcter multiplied by 2
 * the pointers point to the computer word in XHdata for XW
 *
 */


uint16_t swap_uint16(uint16_t val)
{
    return (val << 8) | (val >> 8);
}

struct Scanline {
    unsigned int bits: 16;
};

struct XHdata {
    uint16_t XW;
    uint8_t HD;
    uint8_t XH;
}__attribute__((packed));

struct AL {
    uint16_t height;
    unsigned int proportional : 1;
    unsigned int baseline : 7;
    uint8_t maxWidth;
    uint16_t pointers[0377]; /* 255 in octal */
    //struct XHdata chars[255];
}__attribute__((packed));


void replace(char *str, char from, char to)
{
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == from) str[i] = to;
    }
}

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
    printf("Is %sproportional\n", al.proportional ? "" : "not ");
    printf("Baseline: %i\n", al.baseline);
    printf("maxWidth: %i\n", al.maxWidth);

    // print the "self-relative pointers" table
    for (int i = 1; i < '!'+20; i++) {
        printf("pointer %c => 0x%X +256 = 0x%X\n", i, swap_uint16(al.pointers[i]), ((uint8_t)swap_uint16(al.pointers[i]) * 2)+ 256 - ('0'*3));
    }

    struct XHdata charData;
    char buffer[17];

    // Dummy XW = 1, XH = 0, HD = 0 at
    // 0x203: 0x01 0x00, 0x00 0x00

    for (int i = '#'; i < 255; i++) {
        //printf("%X offset %d\n", i, al.pointers[i]);

        uint16_t charData_offset = swap_uint16(al.pointers[i]);
        printf("offset = %c => 0x%x\n", i, charData_offset);

        // seek the file to after the header and pointers
        //off_t cur_offset = lseek(fd, charData_offset+0x202, SEEK_SET);
        off_t cur_offset = lseek(fd, 0x206 + charData_offset, SEEK_SET);
        assert(cur_offset != -1);

        printf("ASCII char '%c'\n", i);
        //printf("offset = %ld\n", charData_offset);
        //printf("cur offset = %ld\n", cur_offset);

        bytes_read = read(fd, &charData, sizeof(struct XHdata));

        uint16_t scanlines[16];
        cur_offset = lseek(fd, 0x206 + charData_offset - charData.XH, SEEK_SET);

        bytes_read = read(fd, scanlines, charData.XH * 2);

        for (int i = 0; i < charData.XH; i++) {
            sprintf(buffer, "%016b", scanlines[i]);
            replace(buffer, '0', ' ');
            replace(buffer, '1', '#');
            printf("%s\n", buffer);
            //printf("%016b\n", charData.scanlines[i]);
        }
        printf("XW = %d\n", swap_uint16(charData.XW));
        printf("HD = %d\n", charData.HD);
        printf("XH = %d\n", charData.XH);
    }
    //assert(sizeof(struct XHdata) == 2*16+4);

#if 0

    //                       scanlines + three words of 2 bytes
    size_t charData_size = (al.height * 2) + 3 * 2;

    uint8_t buffer[255] = {0};

    bytes_read = read(fd, buffer, sizeof(buffer));

    for(int i = 0; i < 50; i=i+2) {
        printf("%08b%08b\n", buffer[i], buffer[i+1]);
    }
#endif
    //assert(bytes_read == sizeof(struct XHdata) + (sizeof(struct Scanline) * al.height));

    return EXIT_SUCCESS;
}
