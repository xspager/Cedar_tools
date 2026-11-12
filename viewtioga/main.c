#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "tioga.h"
#include "tread.h"

static int StartNode();
static int EndNode();
static int Look();
static int Insert();
static int Prop();

#ifndef DUMP
#define DUMP 0
#endif

static struct tread_Procs procs = {
    StartNode,
    EndNode,
    Look,
    Insert,
    Prop
};

typedef struct Flags {
    int print_comments;
} Flags;

void parseFlags(int argc, char *argv[], Flags *flags)
{
    int opt;
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p':
                flags->print_comments = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-p] filename\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    Flags flags = {0};
    struct tread_Reader *r;
    FILE *file;
    char *data = "Nothing";

    parseFlags(argc, argv, &flags);

    if (optind >= argc){
        printf("usage: %s: tioga_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    file = fopen(argv[optind], "r");
    if (file == NULL) {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    /* Now we're ready to go */
    r = (struct tread_Reader *) malloc(sizeof (*r));
    if (tioga_FromFile(r, file, &procs, data) < 0) {
	    /* Not a tioga file, read normally. */
	    free(r);
	    rewind(file);
    }

    return EXIT_SUCCESS;
}

int StartNode(struct tread_Reader *r, char *format)
{
    /*printf("StartNode: %s\n", format);*/
    return 0;
}
static int EndNode(struct tread_Reader *r)
{
    /*printf("%s\n", format);*/
    return 0;
}

static int Look(struct tread_Reader *r, long looks, long start, long end)
{
    if (looks == 0) return -1;

    /*printf("\t\tlooks = %ld start = %ld, end = %ld\n", looks, start, end);*/
    return 0;
}

static int Insert(struct tread_Reader *r, char *t, long len, int comment)
{
    char *s = t;
    char *c = strchr(t, 0xD3);
    if (c != NULL) {
        s = malloc(sizeof(char) * len + 1); /* 1 byte longer */
        strncpy(s, t, len);
        int c_index = c - t;
        /* add the two bytes utf-8 char '©'  */
        s[c_index] = 0xC2; s[c_index+1] = 0xA9;
        /* copy over the original string over the copy after the '©' character */
        strncpy(&s[c_index+2], &t[c_index+1], len - c_index);
    }
    if (comment == 1) {
        printf(DUMP ? "%s" : "\e[3m %s \e[0m", s);
    } else {
        printf("%s", s);
    }
    if (s != t) {
        free(s);
    }
    return 0;
}

static int Prop(struct tread_Reader *r, char *propName, char *prop, long len)
{
    printf("propName = %s prop = %s len = %ld\n", propName, prop, len);
    return 0;
}

