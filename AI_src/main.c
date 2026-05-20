#include <stdio.h>
#include "book.h"
#include "payment.h"
#include "rent.h"
#include "user.h"
#include "slip.h"

int main(void) {
    User users[100];
    User authenticated_user;
    int user_count;
    FILE *bookfp;
    int scanned_count = 0;
    int accepted_count = 0;
    int amount;
    int payment_result;
    int payment_method = -1;
    const int max_retry = 3;

    book_runtime_init();

    user_count = user_load_users("AI_src/user.csv", users, 100);
    if (user_count == 0) {
        fprintf(stderr, "user.csvを開けませんでした、またはデータが空です\n");
        return 1;
    }

    printf("\n--- ユーザーID認証 ---\n");
    if (user_authenticate_screen(users, user_count, &authenticated_user) != 0) {
        fprintf(stderr, "ユーザー認証に失敗しました\n");
        return 1;
    }

    printf("認証ユーザー: %s (ID: %d)\n", authenticated_user.name, authenticated_user.id);

    bookfp = book_open_csv("AI_src/book.csv", "src/book.csv");
    if (bookfp == NULL) {
        fprintf(stderr, "book.csvを開けませんでした\n");
        return 1;
    }

    if (book_scan_and_checkout_loop(bookfp, &scanned_count, &accepted_count) != 0) {
        fclose(bookfp);
        fprintf(stderr, "会計処理でエラーが発生しました\n");
        return 1;
    }

    fclose(bookfp);
    printf("スキャン済み：%d冊、受理済み：%d冊\n", scanned_count, accepted_count);

    amount = rent_calc_total_price(accepted_count);
    if (amount < 0) {
        fprintf(stderr, "料金計算に失敗しました\n");
        return 1;
    }


    if (amount > 0) {
        payment_method = payment_select_method(max_retry);
        if (payment_method == -1) {
            fprintf(stderr, "支払い方法の選択に失敗しました\n");
            return 1;
        }
        payment_result = payment_process(payment_method, amount);
        if (payment_result == PAYMENT_RESULT_OK) {
            printf("支払い方法: %s\n", payment_method_label(payment_method));
            printf("支払いが完了しました。\n");
        } else {
            printf("支払いに失敗しました。\n");
            return 1;
        }
    } else {
        printf("貸出可能な書籍がないため、支払いはスキップします。\n");
    }

    //log record    
    if (amount > 0 && payment_result == PAYMENT_RESULT_OK) {
        SlipRecord slip;
        char datetime[DATETIME_MAX];
        int slip_no = get_next_slip_no("log.csv");

        get_current_datetime(datetime, sizeof(datetime));
        create_slip_record(
            &slip,
            slip_no,
            authenticated_user.name, // またはID等
            "本のタイトル",           // 実際は貸出本タイトルを連結して渡す
            payment_method_label(payment_method), // 支払い方法名
            "1",                    // レジ番号等
            "2026/05/31",           // 返却期限等
            datetime
        );
        write_slip_to_csv("log.csv", &slip);
    }
    return 0;
}