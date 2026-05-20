//F08 単体テスト

#include "f08_slip.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_get_next_slip_no_file_not_found(void) {
    const char *path = "test_receipt_missing.csv";
    remove(path);
    assert(get_next_slip_no(path) == 1);
}

static void test_get_next_slip_no_existing_file(void) {
    const char *path = "test_receipt_existing.csv";
    FILE *fp = fopen(path, "w");
    assert(fp != NULL);

    fprintf(fp, "slip_no,datetime,user_id,item_name,payment_method,register_no,due_date\n");
    fprintf(fp, "1,2026/05/20 10:00,U1001,BookA,CASH,01,2026/05/27\n");
    fprintf(fp, "2,2026/05/20 10:10,U1002,BookB,CARD,01,2026/05/28\n");
    fclose(fp);

    assert(get_next_slip_no(path) == 3);
    remove(path);
}

static void test_get_current_datetime_format(void) {
    char dt[DATETIME_MAX];
    get_current_datetime(dt, (int)sizeof(dt));

    assert(strlen(dt) == 16);
    assert(dt[4] == '/');
    assert(dt[7] == '/');
    assert(dt[10] == ' ');
    assert(dt[13] == ':');
}

static void test_create_slip_record_and_write_csv(void) {
    const char *path = "test_receipt_write.csv";
    char dt[DATETIME_MAX] = "2026/05/20 14:23";
    SlipRecord rec;
    FILE *fp;
    char line[512];
    int found = 0;

    remove(path);

    create_slip_record(
        &rec,
        10,
        "U123456",
        "NorwegianWood",
        "CARD",
        "01",
        "2026/05/27",
        dt
    );

    assert(rec.slip_no == 10);
    assert(strcmp(rec.user_id, "U123456") == 0);
    assert(strcmp(rec.item_name, "NorwegianWood") == 0);
    assert(strcmp(rec.payment_method, "CARD") == 0);
    assert(strcmp(rec.register_no, "01") == 0);
    assert(strcmp(rec.due_date, "2026/05/27") == 0);
    assert(strcmp(rec.datetime, "2026/05/20 14:23") == 0);

    assert(write_slip_to_csv(path, &rec) == 0);

    fp = fopen(path, "r");
    assert(fp != NULL);

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "10,2026/05/20 14:23,U123456,NorwegianWood,CARD,01,2026/05/27") != NULL) {
            found = 1;
        }
    }

    fclose(fp);
    remove(path);

    assert(found == 1);
}

int main(void) {
    test_get_next_slip_no_file_not_found();
    test_get_next_slip_no_existing_file();
    test_get_current_datetime_format();
    test_create_slip_record_and_write_csv();

    printf("PASS: all F08 unit tests passed.\n");
    return 0;
}
