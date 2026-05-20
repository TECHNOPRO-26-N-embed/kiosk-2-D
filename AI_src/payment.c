//Tanaka

#include "payment.h"

#include <stdio.h>

static void payment_show_menu(void) {
    printf("支払い方法を選択してください\n");
    printf("1. 現金\n");
    printf("2. クレジットカード\n");
    printf("3. バーコード決済\n");
    printf("4. 交通系IC\n");
    printf("番号を入力してください: ");
}

int payment_select_method(int max_retry) {
    int method;
    int i;

    if (max_retry <= 0) {
        return -1;
    }

    for (i = 0; i < max_retry; i++) {
        payment_show_menu();
        if (scanf("%d", &method) != 1) {
            int ch;
            printf("入力が不正です。数値を入力してください。\n");
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            continue;
        }

        if (method >= PAYMENT_CASH && method <= PAYMENT_IC) {
            return method;
        }

        printf("範囲外です。1から4を選んでください。\n");
    }

    return -1;
}

const char *payment_method_label(int method) {
    switch (method) {
        case PAYMENT_CASH:
            return "現金";
        case PAYMENT_CARD:
            return "クレジットカード";
        case PAYMENT_BARCODE:
            return "バーコード決済";
        case PAYMENT_IC:
            return "交通系IC";
        default:
            return "不明";
    }
}

int payment_process(int method, int amount) {
    if (amount < 0) {
        return PAYMENT_RESULT_FAILED;
    }

    switch (method) {
        case PAYMENT_CASH:
        case PAYMENT_CARD:
        case PAYMENT_BARCODE:
        case PAYMENT_IC:
            return PAYMENT_RESULT_OK;
        default:
            return PAYMENT_RESULT_FAILED;
    }
}

int payment_run_flow(int amount, int max_retry) {
    int method = payment_select_method(max_retry);
    int result;

    if (method == -1) {
        printf("支払い方法の選択に失敗しました。\n");
        return PAYMENT_RESULT_INPUT_ERROR;
    }

    result = payment_process(method, amount);
    if (result == PAYMENT_RESULT_OK) {
        printf("支払い方法: %s\n", payment_method_label(method));
        printf("支払いが完了しました。\n");
        return PAYMENT_RESULT_OK;
    }

    printf("支払いに失敗しました。\n");
    return PAYMENT_RESULT_FAILED;
}
