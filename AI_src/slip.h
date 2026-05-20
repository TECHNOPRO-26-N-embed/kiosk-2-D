//Ou

#ifndef SLIP_H
#define SLIP_H

#define USER_ID_MAX 16
#define ITEM_NAME_MAX 64
#define PAYMENT_METHOD_MAX 16
#define REGISTER_NO_MAX 8
#define DATETIME_MAX 20
#define DUE_DATE_MAX 11

typedef struct {
    int slip_no;
    char datetime[DATETIME_MAX];
    char user_id[USER_ID_MAX];
    char item_name[ITEM_NAME_MAX];
    char payment_method[PAYMENT_METHOD_MAX];
    char register_no[REGISTER_NO_MAX];
    char due_date[DUE_DATE_MAX];
} SlipRecord;

int get_next_slip_no(const char *filename);
void get_current_datetime(char *buffer, int size);
void create_slip_record(
    SlipRecord *record,
    int slip_no,
    const char *user_id,
    const char *item_name,
    const char *payment_method,
    const char *register_no,
    const char *due_date,
    const char *datetime
);
int write_slip_to_csv(const char *filename, const SlipRecord *record);

#endif