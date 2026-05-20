// main.c
// 泊数選択・料金計算機能
// 2026/05/20 作成

#include <stdio.h>
#include <stdlib.h>

// 泊数ごとの単価定義
#define PRICE_1NIGHT 50
#define PRICE_2NIGHTS 100
#define PRICE_7NIGHTS 200

// 泊数選択肢
const int valid_nights[] = {1, 2, 7};
const int valid_nights_count = 3;

// 泊数が有効かどうかを判定
int is_valid_night(int nights) {
    for (int i = 0; i < valid_nights_count; i++) {
        if (nights == valid_nights[i]) return 1;
    }
    return 0;
}

// 泊数に応じた単価を返す
int get_price_per_night(int nights) {
    switch (nights) {
        case 1: return PRICE_1NIGHT;
        case 2: return PRICE_2NIGHTS;
        case 7: return PRICE_7NIGHTS;
        default: return -1; // 無効値
    }
}

// 泊数選択・料金計算メイン処理
// 引数: 冊数（F02から引き継ぎ）
// 戻り値: 合計金額
int calc_total_price(int book_count) {
    int nights = 0;
    int price_per_night = 0;
    int total = 0;

    // 冊数表示
    printf("貸出冊数: %d\n", book_count);

    // 泊数選択ループ
    while (1) {
        printf("泊数を選択してください（1, 2, 7）: ");
        if (scanf("%d", &nights) != 1) {
            // 入力エラー
            printf("入力が正しくありません。\n");
            // 標準入力バッファクリア
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        if (!is_valid_night(nights)) {
            printf("1泊、2泊、7泊のみ選択可能です。\n");
            continue;
        }
        break;
    }

    // 単価取得
    price_per_night = get_price_per_night(nights);
    if (price_per_night < 0) {
        printf("内部エラー: 単価取得失敗\n");
        return -1;
    }

    // 合計金額計算
    total = price_per_night * book_count;
    printf("\n【料金計算結果】\n");
    printf("泊数: %d泊\n", nights);
    printf("単価: %d円\n", price_per_night);
    printf("冊数: %d冊\n", book_count);
    printf("合計金額: %d円\n", total);

    // 会計処理への連携はここで行う（F04へ）
    // ...

    return total;
}


int main() {
    int book_count = 2; // 仮の冊数
    calc_total_price(book_count);
    return 0;
}
