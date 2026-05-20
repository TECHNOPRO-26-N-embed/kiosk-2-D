#include "rent.h"

#include <stdio.h>

#define PRICE_1NIGHT 50
#define PRICE_2NIGHTS 100
#define PRICE_7NIGHTS 200

static const int valid_nights[] = {1, 2, 7};
static const int valid_nights_count = 3;

int rent_is_valid_night(int nights) {
    int i;
    for (i = 0; i < valid_nights_count; i++) {
        if (nights == valid_nights[i]) {
            return 1;
        }
    }
    return 0;
}

int rent_get_price_per_night(int nights) {
    switch (nights) {
        case 1:
            return PRICE_1NIGHT;
        case 2:
            return PRICE_2NIGHTS;
        case 7:
            return PRICE_7NIGHTS;
        default:
            return -1;
    }
}

int rent_calc_total_price(int book_count) {
    int nights = 0;
    int price_per_night;
    int total;

    if (book_count <= 0) {
        printf("貸出冊数が0冊のため、料金計算をスキップします。\n");
        return 0;
    }

    printf("貸出冊数: %d\n", book_count);

    while (1) {
        int read_result;
        int c;

        printf("泊数を選択してください（1, 2, 7）: ");
        read_result = scanf("%d", &nights);
        if (read_result != 1) {
            printf("入力が正しくありません。\n");
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            continue;
        }

        if (!rent_is_valid_night(nights)) {
            printf("1泊、2泊、7泊のみ選択可能です。\n");
            continue;
        }

        break;
    }

    price_per_night = rent_get_price_per_night(nights);
    if (price_per_night < 0) {
        printf("内部エラー: 単価取得失敗\n");
        return -1;
    }

    total = price_per_night * book_count;
    printf("\n【料金計算結果】\n");
    printf("泊数: %d泊\n", nights);
    printf("単価: %d円\n", price_per_night);
    printf("冊数: %d冊\n", book_count);
    printf("合計金額: %d円\n", total);

    return total;
}
