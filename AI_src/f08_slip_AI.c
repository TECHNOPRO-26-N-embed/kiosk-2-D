//F08 関数実装

#include "f08_slip.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

int get_next_slip_no(const char *filename) {
    FILE *fp;
    int last_no = 0;
    int tmp_no = 0;
    char line[512];

    if (filename == NULL) {
        return 1;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d", &tmp_no) == 1) {
            last_no = tmp_no;
        }
    }

    fclose(fp);
    return last_no + 1;
}

void get_current_datetime(char *buffer, int size) {
    time_t t;
    struct tm *tm_info;

    if (buffer == NULL || size <= 0) {
        return;
    }

    t = time(NULL);
    tm_info = localtime(&t);
    if (tm_info == NULL) {
        buffer[0] = '\0';
        return;
    }

    strftime(buffer, (size_t)size, "%Y/%m/%d %H:%M", tm_info);
}

static void copy_field(char *dst, size_t dst_size, const char *src) {
    if (dst == NULL || dst_size == 0) {
        return;
    }

    if (src == NULL) {
        dst[0] = '\0';
        return;
    }

    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

void create_slip_record(
    SlipRecord *record,
    int slip_no,
    const char *user_id,
    const char *item_name,
    const char *payment_method,
    const char *register_no,
    const char *due_date,
    const char *datetime
) {
    if (record == NULL) {
        return;
    }

    record->slip_no = slip_no;
    copy_field(record->datetime, sizeof(record->datetime), datetime);
    copy_field(record->user_id, sizeof(record->user_id), user_id);
    copy_field(record->item_name, sizeof(record->item_name), item_name);
    copy_field(record->payment_method, sizeof(record->payment_method), payment_method);
    copy_field(record->register_no, sizeof(record->register_no), register_no);
    copy_field(record->due_date, sizeof(record->due_date), due_date);
}

int write_slip_to_csv(const char *filename, const SlipRecord *record) {
    FILE *fp;

    if (filename == NULL || record == NULL) {
        return -1;
    }

    fp = fopen(filename, "a");
    if (fp == NULL) {
        return -1;
    }

    fprintf(
        fp,
        "%d,%s,%s,%s,%s,%s,%s\n",
        record->slip_no,
        record->datetime,
        record->user_id,
        record->item_name,
        record->payment_method,
        record->register_no,
        record->due_date
    );

    fclose(fp);
    return 0;
}
