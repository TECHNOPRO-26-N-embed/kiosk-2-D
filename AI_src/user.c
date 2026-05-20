//Win

#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copy_text(char *dst, size_t dst_size, const char *src) {
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

int user_load_users(const char *filename, User users[], int max_count) {
    FILE *fp;
    char line[256];
    int count = 0;

    if (filename == NULL || users == NULL || max_count <= 0) {
        return 0;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0;
    }

    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL && count < max_count) {
        char *token = strtok(line, ",");
        if (token == NULL) {
            continue;
        }
        users[count].id = atoi(token);

        token = strtok(NULL, ",");
        copy_text(users[count].name, sizeof(users[count].name), token);

        token = strtok(NULL, ",");
        copy_text(users[count].address, sizeof(users[count].address), token);

        token = strtok(NULL, ",");
        users[count].age = token ? atoi(token) : 0;

        token = strtok(NULL, ",");
        copy_text(users[count].phone, sizeof(users[count].phone), token);

        token = strtok(NULL, ",");
        users[count].gender = token ? atoi(token) : 0;

        token = strtok(NULL, ",");
        users[count].now_rent = token ? atoi(token) : 0;

        token = strtok(NULL, ",");
        if (token != NULL) {
            token[strcspn(token, "\r\n")] = '\0';
        }
        copy_text(users[count].reference, sizeof(users[count].reference), token ? token : "");

        count++;
    }

    fclose(fp);
    return count;
}

int user_find_by_id(const User users[], int count, int input_id) {
    int i;

    if (users == NULL || count <= 0) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        if (users[i].id == input_id) {
            return i;
        }
    }

    return -1;
}

void user_show_info(const User *user) {
    if (user == NULL) {
        return;
    }

    printf("\n--- ユーザー情報 ---\n");
    printf("ID: %d\n", user->id);
    printf("氏名: %s\n", user->name);
    printf("住所: %s\n", user->address);
    printf("年齢: %d\n", user->age);
    printf("電話番号: %s\n", user->phone);
    printf("性別: %s\n", user->gender ? "女性" : "男性");
    printf("貸出状況: %s\n", user->now_rent == 0 ? "貸出なし" : (user->now_rent == 1 ? "貸出中" : "延滞中"));
    printf("備考: %s\n", user->reference);
}

int user_input_id(void) {
    int id;

    printf("ユーザーIDを入力してください: ");
    if (scanf("%d", &id) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        return -1;
    }

    return id;
}

void user_show_not_found_message(int input_id) {
    printf("ユーザーID %d は見つかりませんでした。\n", input_id);
}

void user_show_invalid_input_message(void) {
    printf("入力値が不正です。数値で入力してください。\n");
}

int user_authenticate_screen(const User users[], int user_count, User *authenticated_user) {
    int input_id;
    int idx;

    if (users == NULL || user_count <= 0 || authenticated_user == NULL) {
        return -1;
    }

    while (1) {
        input_id = user_input_id();
        if (input_id < 0) {
            user_show_invalid_input_message();
            continue;
        }

        idx = user_find_by_id(users, user_count, input_id);
        if (idx == -1) {
            user_show_not_found_message(input_id);
            continue;
        }

        *authenticated_user = users[idx];
        user_show_info(authenticated_user);
        return 0;
    }
}
