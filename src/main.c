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
    SEVEN_DAYS = 200, // 7泊
    OVERDUE = 1000 // 延滞
} ChargeTable;

int GetUserID(User users[], int count, int input_id) {
    for (int i = 0; i < count; i++) {
        if (users[i].id == input_id) {
            return i;
        }
    }
    return -1;
}

int GetBookID(Book books[], int count, int book_code) {
    for (int i = 0; i < count; i++) {
        if (books[i].code == book_code) {
            return i;
        }
    }
    return -1;
}

// Book GetCode(Book *books[100]){
//     //bookリスト取得
//     FILE *bookfp;
//     char book_filename[] = "src/book.csv";
//     fopen(book_filename, "r");
//     char str[100]="";
//     fgets(str, 100, bookfp);

//     //バーコードを入力
//     int code, i=0;
//     scanf("%d", &code);
//     //bookリストからコードと一致するものを探す
//     while(i<=100) {
//         //コードと一致するものがあれば、その書籍の情報を返す
//         if(code == books[i]->code){
//             return *books[i];
//         }
//         i++;
//     }
//     printf("コードが見つかりませんでした。");

//     Book not_found = {0};
//     not_found.code = -1;
//     return not_found;

// };

int CalculateCharge(int book_num){
    ChargeTable stay_days;
    while(1){
        printf("泊数を選んでください\n");
        printf("1. 1泊\n");
        printf("2. 2泊\n");
        printf("3. 7泊\n");
        int choice;
        scanf("%d", &choice);
        if(choice < 1 || choice > 3){
            printf("無効な入力です。もう一度選んでください。\n");
            continue;
        }
        switch(choice){
            case 1:
                stay_days = ONE_DAY;
                break;
            case 2:
                stay_days = TWO_DAYS;
                break;
            case 3:
                stay_days = SEVEN_DAYS;
                break;
        }
        break;
    }
    return stay_days * book_num;
};

int ConfirmScreen(User user, Book book, int days, Log log){

    int select;

    printf("\n===== 確認画面 =====\n");

    printf("利用者ID : %d\n", user.id);

    printf("書籍コード : %d\n", book.code);

    printf("書籍タイトル : %s\n", book.title);

    printf("泊数 : %d\n", days);

    printf("合計金額 : %d円\n", log.charge);

    printf("\n1. 確定\n");
    printf("2. キャンセル\n");

    scanf("%d", &select);

    return select;
}

int SelectPayment(){
    int pay;

    printf("支払い方法を選択してください\n");
    printf("1. 現金\n");
    printf("2. クレジットカード\n");
    printf("3. バーコード決済\n");
    printf("4. 交通系IC\n");
    printf("番号を入力してください：");
    scanf("%d", &pay);

    switch(pay){
        case 1:
            printf("支払い方法：現金\n");
            break;
        case 2:
            printf("支払い方法：クレジットカード\n");
            break;
        case 3:
            printf("支払い方法：バーコード決済\n");
            break;
        case 4:
            printf("支払い方法：交通系IC\n");
            break;
        default:
            printf("不正な入力です。\n");
            break;
    }
    return pay;
}

// 3.商品バーコードを通す
Book GetCode(Book *books[]){
    //bookリスト取得
    FILE *bookfp;
    char book_filename[] = "book.csv";
    fopen(book_filename, "r");
    char str[100]="";
    fgets(str, 100, bookfp);
    
    //バーコードを入力
    int code, i=0;
    scanf("%d", &code);
    //bookリストからコードと一致するものを探す
    while(i<=100) {
        //コードと一致するものがあれば、その書籍の情報を返す
        if(code == books[i]->code){
            return *books[i];
        }
        i++;
    }
    printf("コードが見つかりませんでした。");
    Book not_found = {0};
    not_found.code = -1;
    return not_found;
};

int main() {
    FILE *userfp;
    char user_filename[] = "src/user.csv";

    FILE *bookfp;
    char book_filename[] = "src/book.csv";

    FILE *logfp;
<<<<<<< Updated upstream
    char log_filename[] = "src/log.csv";

    User users[100];
    Book books[100]; // bookリスト
    char line[500];
    int user_count;
    int input_id;

    int book_count;
    int book_code;

    int charge = 0; // 合計金額
    int book_num = 2; // 貸出冊数

    int pay; // 支払い方法

    User user;
    Book book;
    Log log;
    //mainのなかで宣言している変数
    int days;
    int confirm;
=======
    char log_filename[] = "log.csv";
>>>>>>> Stashed changes

    User users[100];
    Book books[100];

    while(1){
        printf("図書館貸出システム\n");
        // 1.メインメニューを表示する

        // 2.ユーザーのIDを受け取る
        user_count = 0;
        userfp = fopen(user_filename, "r");
        if (userfp == NULL) {
            printf("ファイルを開けません: %s\n", user_filename);
            return 1;
        }

        fgets(line, sizeof(line), userfp);

        while (fgets(line, sizeof(line), userfp)) {
       if (sscanf(line, "%d,%[^,],%[^,],%d,%[^,],%d,%d,%[^\n]",
            &users[user_count].id,
            users[user_count].name,
            users[user_count].address,
            &users[user_count].age,
            users[user_count].phone,
            &users[user_count].gender,
            &users[user_count].now_rent,
            users[user_count].reference) == 8) {
            user_count++;
        }
    }
        fclose(userfp);

        printf("ユーザーIDを入力してください: ");
        
        fgets(line, sizeof(line), stdin);
        sscanf(line, "%d", &input_id);

        int user_index = GetUserID(users, user_count, input_id);

        if (user_index == -1) {
            printf("ユーザーが見つかりません\n");
            continue;
        }
        printf("\n--- ユーザー情報 ---\n");
        printf("ID: %d\n", users[user_index].id);
        printf("名前: %s\n", users[user_index].name);

        // 3.商品バーコードを通す
        book_count = 0;
        bookfp = fopen(book_filename, "r");
        if (bookfp == NULL) {
            printf("ファイルを開けません: %s\n", book_filename);
            return 1;
        }

        fgets(line, sizeof(line), bookfp);

        while (fgets(line, sizeof(line), bookfp)) {
            // title, code, status

       if (sscanf(line, "%s,%d,%d\n",
             books[book_count].title,
             &books[book_count].code,
             &books[book_count].status) == 3) {
            book_count++;
        }
    }
        fclose(bookfp);

        printf("本のコードを入力してください: ");
        
        fgets(line, sizeof(line), stdin);
        sscanf(line, "%d", &book_code);

        int book_index = GetBookID(books, book_count, book_code);

        if (book_index == -1) {
            printf("本が見つかりません\n");
        }
        printf("\n--- 本情報 ---\n");
        printf("コード: %d\n", books[book_index].code);
        printf("タイトル: %s\n", books[book_index].title);

        // 4. 泊数を選ぶ
        printf("冊数: %d冊\n", book_num);
        charge = CalculateCharge(book_num);
        printf("合計金額: %d円\n", charge);

        // 5. 確認画面
        confirm = ConfirmScreen(user, book, days, log); //main.cのなかでConfirmScreen関数を呼び出す

        // 6. 支払い方法選ぶ
        pay = SelectPayment();

        // 7. 完了画面
        printf("\n===== 完了画面 =====\n");

        break;

    }
    return 0;
}