#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char title[100];
    int code;
    int status; // 貸出状況
} Book;

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

typedef struct {
    int receipt_num; // 伝票番号
    int user_id; // 利用者ID

    int book_code; // 書籍コード
    char book_title[100]; // 書籍タイトル

    int rent_date; // 貸出日
    int return_date; // 返却日

    int payment; // 支払い方法
    int charge; // 合計金額

    int register_num; // レジ番号

} Log;

typedef enum {
    ONE_DAY = 50, // 1泊
    TWO_DAYS = 100, // 2泊
    SEVEN_DAYS =200, // 7泊
    OVERDUE = 1000 // 延滞
} ChargeTable;

int GetUserID(User user){};

int main() {
    FILE *userfp;
    char user_filename[] = "user.csv";

    FILE *bookfp;
    char book_filename[] = "book.csv";



    while(1){
        printf("図書館貸出システム\n");
        // 1.メインメニューを表示する

        // 2.ユーザーのIDを受け取る

        // 3.商品バーコードを通す

        // 4. 泊数を選ぶ

        // 5. 確認画面

        // 6. 支払い方法選ぶ

        // 7. 完了画面

    }
    return 0;
}