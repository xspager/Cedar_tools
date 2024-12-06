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
 * this pointers needed to be added to the numeric value of the ASCII character and the two word offset for the header then multiplied by two to get the right offset in bytes.
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
    struct XHdata chars[255];
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
    assert(sizeof(struct XHdata) == 4);

    int bytes_read = read(fd, &al, sizeof(al));
    assert(bytes_read == sizeof(al));

    printf("Height: %i\n", swap_uint16(al.height));
    printf("Is %sproportional\n", al.proportional ? "" : "not ");
    printf("Baseline: %i\n", al.baseline);
    printf("maxWidth: %i\n", al.maxWidth);

#if 0
    // print the "self-relative pointers" table
    for (int i = 1; i < '!'+20; i++) {
        printf("pointer %c => 0x%X +256 = 0x%X\n", i, swap_uint16(al.pointers[i]), ((uint8_t)swap_uint16(al.pointers[i]) * 2)+ 256 - ('0'*3));
    }
#endif

    struct XHdata charData;
    char buffer[17];

    // Dummy XW = 1, XH = 0, HD = 0 at
    // 0x203: 0x01 0x00, 0x00 0x00

    for (int i = 0; i < 128; i++) {
        //printf("%X offset %d\n", i, al.pointers[i]);

        uint16_t charData_offset = swap_uint16(al.pointers[i]);
        // pointer + char * 2 + header offset
        int real_offset = (charData_offset + i + 2)*2;
        printf("offset = %d => 0x%x => 0x%X\n", i, charData_offset, real_offset);

        // seek the file to XHdata for this char
        off_t cur_offset = lseek(fd, real_offset, SEEK_SET);
        assert(cur_offset != -1);

        printf("ASCII char '%c'\n", i);
        bytes_read = read(fd, &charData, sizeof(struct XHdata));

        uint16_t scanlines[16] = {0};
        // go back in file XH words
        cur_offset = lseek(fd, real_offset - (charData.XH * 2), SEEK_SET);

        bytes_read = read(fd, scanlines, charData.XH * 2);

        // FIXME: We need to put the wider charactes toguether
        // FIXME: We need to handle the omited all zeroes scanlines
        for (int i = 0; i < charData.XH; i++) {
            sprintf(buffer, "%016b", swap_uint16(scanlines[i]));
            replace(buffer, '0', ' ');
            replace(buffer, '1', '#');
            printf("%s\n", buffer);
        }

        printf("XW = %d HD = %d XH = %d\n", swap_uint16(charData.XW), charData.HD, charData.XH);
    }

    return EXIT_SUCCESS;
}
