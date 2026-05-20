#include <stdio.h>

typedef enum {
    PAYMENT_CASH = 1,
    PAYMENT_CARD = 2,
    PAYMENT_BARCODE = 3,
    PAYMENT_IC = 4
} PaymentMethod;

typedef enum {
    PAYMENT_RESULT_OK = 0,
    PAYMENT_RESULT_INPUT_ERROR = 1,
    PAYMENT_RESULT_FAILED = 2
} PaymentResult;

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
            printf("Invalid input. Enter a number.\n");
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            continue;
        }

        if (method >= PAYMENT_CASH && method <= PAYMENT_IC) {
            return method;
        }

        printf("Invalid range. Please select 1 to 4.\n");
    }

    return -1;
}

const char* payment_method_label(int method) {
    switch (method) {
        case PAYMENT_CASH:
            return "Cash";
        case PAYMENT_CARD:
            return "Credit Card";
        case PAYMENT_BARCODE:
            return "Barcode Payment";
        case PAYMENT_IC:
            return "Transportation IC";
        default:
            return "Unknown";
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
        printf("Payment selection failed.\n");
        return PAYMENT_RESULT_INPUT_ERROR;
    }

    result = payment_process(method, amount);
    if (result == PAYMENT_RESULT_OK) {
        printf("Payment method: %s\n", payment_method_label(method));
        printf("Payment completed.\n");
        return PAYMENT_RESULT_OK;
    }

    printf("Payment failed.\n");
    return PAYMENT_RESULT_FAILED;
}


// テスト用main関数
int main(void) {
    int amount = 1000;
    int max_retry = 3;
    printf("\n--- Payment Module Test ---\n");
    printf("Amount: %d\n", amount);
    int result = payment_run_flow(amount, max_retry);
    printf("Result code: %d\n", result);
    return 0;
}
