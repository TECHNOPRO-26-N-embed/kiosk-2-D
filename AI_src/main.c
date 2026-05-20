#include <stdio.h>
#include "book.h"
#include "payment.h"
#include "rent.h"
#include "user.h"

int main(void) {
    User users[100];
    User authenticated_user;
    int user_count;
    FILE *bookfp;
    int scanned_count = 0;
    int accepted_count = 0;
    int amount;
    int payment_result;
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
        payment_result = payment_run_flow(amount, max_retry);
        if (payment_result != PAYMENT_RESULT_OK) {
            fprintf(stderr, "支払い処理に失敗しました\n");
            return 1;
        }
    } else {
        printf("貸出可能な書籍がないため、支払いはスキップします。\n");
    }

    return 0;
}