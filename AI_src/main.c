#include <stdio.h>
#include "book.h"
#include "payment.h"
#include "rent.h"
#include "slip.h"
#include "user.h"

static const char *payment_method_code(int method) {
    switch (method) {
        case PAYMENT_CASH:
            return "CASH";
        case PAYMENT_CARD:
            return "CARD";
        case PAYMENT_BARCODE:
            return "BARCODE";
        case PAYMENT_IC:
            return "IC";
        default:
            return "UNKNOWN";
    }
}

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

    //---------------------Win----------------------------------//

    user_count = user_load_users("AI_src/user.csv", users, 100);
    if (user_count == 0) {
        fprintf(stderr, "user.csvを開けませんでした、またはデータが空です。\n");
        return 1;
    }

    printf("\n--- ユーザーID認証 ---\n");
    if (user_authenticate_screen(users, user_count, &authenticated_user) != 0) {
        fprintf(stderr, "ユーザー認証に失敗しました。\n");
        return 1;
    }

    printf("認証ユーザー: %s (ID: %d)\n", authenticated_user.name, authenticated_user.id);

    //---------------------Nishimura----------------------------------//

    bookfp = book_open_csv("AI_src/book.csv", "src/book.csv");
    if (bookfp == NULL) {
        fprintf(stderr, "book.csvを開けませんでした。\n");
        return 1;
    }

    if (book_scan_and_checkout_loop(bookfp, &scanned_count, &accepted_count) != 0) {
        fclose(bookfp);
        fprintf(stderr, "会計処理でエラーが発生しました。\n");
        return 1;
    }

    fclose(bookfp);
    printf("スキャン済み：%d冊、受理済み：%d冊\n", scanned_count, accepted_count);

    //---------------------Kobayashi----------------------------------//

    amount = rent_calc_total_price(accepted_count);
    if (amount < 0) {
        fprintf(stderr, "料金計算に失敗しました。\n");
        return 1;
    }

    //---------------------Tanaka----------------------------------//

    if (amount > 0) {
        payment_method = payment_select_method(max_retry);
        if (payment_method == -1) {
            fprintf(stderr, "支払い方法選択の最大試行回数に達しました。\n");
            fprintf(stderr, "最初からやり直してください。\n");
            return 1;
        }

        payment_result = payment_process(payment_method, amount);
        if (payment_result != PAYMENT_RESULT_OK) {
            fprintf(stderr, "支払い処理に失敗しました。\n");
            return 1;
        }

        printf("支払い方法: %s\n", payment_method_label(payment_method));
        printf("支払いが完了しました。\n");
    } else {
        printf("貸出可能な書籍がないため、支払いはスキップします。\n");
    }

    if (amount > 0 && payment_result == PAYMENT_RESULT_OK) {
        SlipRecord slip;
        char datetime[DATETIME_MAX];
        char user_id[USER_ID_MAX];
        char item_name[ITEM_NAME_MAX];
        const char *primary_log_path = "AI_src/log.csv";
        const char *fallback_log_path = "log.csv";
        int slip_no;
        int write_result;

        snprintf(user_id, sizeof(user_id), "%d", authenticated_user.id);
        snprintf(item_name, sizeof(item_name), "accepted_books:%d", accepted_count);
        get_current_datetime(datetime, sizeof(datetime));

        slip_no = get_next_slip_no(primary_log_path);
        create_slip_record(
            &slip,
            slip_no,
            user_id,
            item_name,
            payment_method_code(payment_method),
            "01",
            "-",
            datetime
        );

        write_result = write_slip_to_csv(primary_log_path, &slip);
        if (write_result != 0) {
            slip_no = get_next_slip_no(fallback_log_path);
            create_slip_record(
                &slip,
                slip_no,
                user_id,
                item_name,
                payment_method_code(payment_method),
                "01",
                "-",
                datetime
            );
            write_result = write_slip_to_csv(fallback_log_path, &slip);
        }

        if (write_result != 0) {
            fprintf(stderr, "log.csvへの書き込みに失敗しました\n");
            return 1;
        }

        printf("ログを保存しました。\n");
    }

    return 0;
}