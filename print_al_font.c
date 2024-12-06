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
 * this pointers needed to be added to the numeric value of the ASCII character and the two word offset for the header then multiplied by two to get the right offset in bytes. I assume adding the offset of the pointer + it's value (times two) should produce the same final offset for the relative XHdata structture.
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
    uint16_t pointers[0377]; // 255 in octal
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

    if (argc <= 1) {
        printf("usage: %s [-pointers -header -text] font.al\n", argv[0]);
        exit(1);
    }

    int print_header = 1;
    int print_pointers = 1;
    int print_text = 0;
    if (argc >= 2) {
        if (strncmp("-pointers", argv[1], 10) == 0) {
            print_pointers = 1;
            print_header = 0;
        }
        if (strncmp("-header", argv[1], 7) == 0) {
            print_header = 1;
            print_pointers = 0;
        }
        if (strncmp("-text", argv[1], 7) == 0) {
            print_header = 0;
            print_pointers = 0;
            print_text = 1;
        }
    }
    
    fd = open(argv[argc - 1], O_RDONLY);
    assert(fd > 0);

    fstat(fd, &statbuf);
    file_size = statbuf.st_size;

    assert(file_size > sizeof(struct AL));

    struct AL al;
    assert(sizeof(struct XHdata) == 4);

    int bytes_read = read(fd, &al, sizeof(al));
    assert(bytes_read == sizeof(al));

    if (print_header) {
        printf("Height: %i\n", swap_uint16(al.height));
        printf("Is %sproportional\n", al.proportional ? "" : "not ");
        printf("Baseline: %i\n", al.baseline);
        printf("maxWidth: %i\n", al.maxWidth);
    }
    if (print_pointers) {
        // print the "self-relative pointers" table
        for (int i = 0; i < 255; i++) {
            printf("pointer at %d = 0x%X => 0x%X\n", i, swap_uint16(al.pointers[i]), (swap_uint16(al.pointers[i]) + i + 2) * 2);
        }
    }

    struct XHdata charData;
    char buffer[17];

    // Dummy XW = 1, XH = 0, HD = 0 at
    // 0x203: 0x01 0x00, 0x00 0x00
    if (!print_text) {
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
    } else {
        char *text = argv[2];
        // Now we need (16 x strlen(text)) x 16 bits of height to place the pixels
        uint16_t *out_buffer = malloc(strlen(text) * 16 * 16);

        for (int i = 0; i < strlen(text); i++) {
            uint16_t bit_buffer[16] = {0};
            uint16_t charData_offset = swap_uint16(al.pointers[(int)text[i]]);
            int real_offset = (charData_offset + text[i] + 2)*2;
            //printf("offset for %c is 0x%X\n", text[i], real_offset);
            off_t cur_offset = lseek(fd, real_offset, SEEK_SET);
            assert(cur_offset != -1);
            struct XHdata charData;
            int bytes_read = read(fd, &charData, sizeof(struct XHdata));
            assert(bytes_read == sizeof(struct XHdata));
            cur_offset = lseek(fd, real_offset - (charData.XH * 2), SEEK_SET);
            assert(cur_offset != -1);
            bytes_read = read(fd, bit_buffer, charData.XH * 2);
            assert(bytes_read == charData.XH * 2);

            // FIXME: respect the XW value
            // FIXME: write in less than 16 increments/spaces

            //printf("XW = %d HD = %d XW = %d\n", swap_uint16(charData.XW), charData.HD,  charData.XH);

            for (int row = 0; row < charData.XH; row++) {
                //printf("row %d \tbit_buffer = %016b\n", row, swap_uint16(bit_buffer[row]));
                out_buffer[row+(16*i) + 16-charData.XH] = swap_uint16(bit_buffer[row]);
            }
        }

        close(fd); // just in case

        char filename_buffer[50];
        sprintf(filename_buffer, "%s.pbm", argv[argc-1]);

        int new_fd = open(filename_buffer, O_WRONLY|O_CREAT|O_TRUNC, 0600);
        if (new_fd < 0) {
            perror("Opening file");
            exit(-1);
        };
        char header[40];
        sprintf(header, "P4\n%d %d\n", 16, (int) strlen(text) * 16);
        write(new_fd, header, strlen(header));
        write(new_fd, out_buffer, strlen(text) * 16 * 16);
        close(new_fd);

        char *line_buffer = malloc((strlen(text) * 30) + 1);
        char *line_buffer_ptr = line_buffer;
        for (int j = 0; j < 16; j++) {
            memset(line_buffer, ' ', strlen(text) * 16);
            line_buffer[strlen(text) * 16] = '\0';
            for (int k = 0; k < strlen(text); k++) {
                sprintf(line_buffer_ptr, "%016b", out_buffer[j+(16*k)]);
                line_buffer_ptr += 16;
            }
            replace(line_buffer, '0', ' ');
            replace(line_buffer, '1', '#');
            printf("%s\n", line_buffer);
            line_buffer_ptr = line_buffer;
        }
        free(line_buffer);

        free(out_buffer);
    }

    return EXIT_SUCCESS;
}
