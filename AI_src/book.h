#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>

#define BOOK_JAN_MAX 32
#define BOOK_TITLE_MAX 128

typedef struct {
    char jan[BOOK_JAN_MAX];
    char title[BOOK_TITLE_MAX];
    int status;
} Book;

void book_runtime_init(void);
FILE *book_open_csv(const char *primary_path, const char *fallback_path);
int book_is_valid_jan(const char *s);
const char *book_status_text(int status);
int book_find_by_jan(FILE *bookfp, const char *jan, Book *out_book);
int book_scan_and_checkout_loop(FILE *bookfp, int *scanned_count, int *accepted_count);

#endif
