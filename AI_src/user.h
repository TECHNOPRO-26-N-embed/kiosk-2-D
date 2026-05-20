//Win

#ifndef USER_H
#define USER_H

#define USER_NAME_MAX 50
#define USER_ADDRESS_MAX 100
#define USER_PHONE_MAX 15
#define USER_REFERENCE_MAX 100

typedef struct {
    int id;
    char name[USER_NAME_MAX];
    char address[USER_ADDRESS_MAX];
    int age;
    char phone[USER_PHONE_MAX];
    int gender;
    int now_rent;
    char reference[USER_REFERENCE_MAX];
} User;

int user_load_users(const char *filename, User users[], int max_count);
int user_find_by_id(const User users[], int count, int input_id);
void user_show_info(const User *user);
int user_input_id(void);
void user_show_not_found_message(int input_id);
void user_show_invalid_input_message(void);
int user_authenticate_screen(const User users[], int user_count, User *authenticated_user);

#endif
