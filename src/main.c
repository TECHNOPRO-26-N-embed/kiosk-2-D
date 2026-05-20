#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ユーザー構造体定義 ---
typedef struct {
    int id;
    char name[50];
    char address[100];
    int age;
    char phone[15];
    int gender; // 0: 男性, 1: 女性
    int now_rent; // 0: 貸出なし, 1: 貸出中 2:延滞中
    char reference[100]; // 備考
} User;

// --- F1 UserID 詳細設計 関数実装 ---
int LoadUsers(const char *filename, User users[], int max_count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char line[256];
    int count = 0;
    fgets(line, sizeof(line), fp); // ヘッダスキップ
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        char *token = strtok(line, ",");
        if (!token) continue;
        users[count].id = atoi(token);
        token = strtok(NULL, ",");
        strcpy(users[count].name, token);
        token = strtok(NULL, ",");
        strcpy(users[count].address, token);
        token = strtok(NULL, ",");
        users[count].age = atoi(token);
        token = strtok(NULL, ",");
        strcpy(users[count].phone, token);
        token = strtok(NULL, ",");
        users[count].gender = atoi(token);
        token = strtok(NULL, ",");
        users[count].now_rent = atoi(token);
        token = strtok(NULL, ",");
        if (token) token[strcspn(token, "\r\n")] = 0;
        strcpy(users[count].reference, token ? token : "");
        count++;
    }
    fclose(fp);
    return count;
}

int FindUserByID(const User users[], int count, int input_id) {
    for (int i = 0; i < count; i++) {
        if (users[i].id == input_id) return i;
    }
    return -1;
}

void ShowUserInfo(const User *user) {
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

int InputUserID(void) {
    int id;
    printf("ユーザーIDを入力してください: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n'); // 入力バッファクリア
        return -1;
    }
    return id;
}

void ShowUserNotFoundMessage(int input_id) {
    printf("ユーザーID %d は見つかりませんでした。\n", input_id);
}

void ShowInvalidInputMessage(void) {
    printf("入力値が不正です。数値で入力してください。\n");
}

void UserIDInputScreen(const User users[], int user_count) {
    int input_id;
    while (1) {
        input_id = InputUserID();
        if (input_id < 0) {
            ShowInvalidInputMessage();
            continue;
        }
        int idx = FindUserByID(users, user_count, input_id);
        if (idx == -1) {
            ShowUserNotFoundMessage(input_id);
            continue;
        }
        ShowUserInfo(&users[idx]);
        break;
    }
}

int main(void) {
    User users[100];
    int user_count = LoadUsers("src/user.csv", users, 100);
    if (user_count == 0) {
        printf("ユーザー情報の読み込みに失敗しました。\n");
        return 1;
    }
    printf("\n--- ユーザーID認証 ---\n");
    UserIDInputScreen(users, user_count);
    return 0;
}
