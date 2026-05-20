//Nishimura

#include "book.h"

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

void book_runtime_init(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, ".UTF-8");
}

FILE *book_open_csv(const char *primary_path, const char *fallback_path) {
    FILE *fp;

    if (primary_path != NULL) {
        fp = fopen(primary_path, "r");
        if (fp != NULL) {
            return fp;
        }
    }

    if (fallback_path != NULL) {
        return fopen(fallback_path, "r");
    }

    return NULL;
}

int book_is_valid_jan(const char *s) {
    size_t i;

    if (s == NULL || strlen(s) != 13) {
        return 0;
    }

    for (i = 0; i < 13; i++) {
        if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }

    return 1;
}

const char *book_status_text(int status) {
    switch (status) {
        case 0:
            return "貸出可能";
        case 1:
            return "貸出中";
        case 2:
            return "延滞中";
        default:
            return "不明";
    }
}

int book_find_by_jan(FILE *bookfp, const char *jan, Book *out_book) {
    char line[512];
    Book current;

    if (bookfp == NULL || jan == NULL || out_book == NULL) {
        return -1;
    }

    out_book->jan[0] = '\0';
    out_book->title[0] = '\0';
    out_book->status = -1;

    rewind(bookfp);
    if (fgets(line, sizeof(line), bookfp) == NULL) {
        return -1;
    }

    while (fgets(line, sizeof(line), bookfp) != NULL) {
        if (sscanf(line, "%31[^,],%127[^,],%d", current.jan, current.title, &current.status) == 3) {
            if (strcmp(current.jan, jan) == 0) {
                *out_book = current;
                return 0;
            }
        }
    }

    return 1;
}

static int book_find_by_index(FILE *bookfp, int target_index, Book *out_book) {
    char line[512];
    Book current;
    int row = 0;

    if (bookfp == NULL || out_book == NULL || target_index <= 0) {
        return -1;
    }

    out_book->jan[0] = '\0';
    out_book->title[0] = '\0';
    out_book->status = -1;

    rewind(bookfp);
    if (fgets(line, sizeof(line), bookfp) == NULL) {
        return -1;
    }

    while (fgets(line, sizeof(line), bookfp) != NULL) {
        if (sscanf(line, "%31[^,],%127[^,],%d", current.jan, current.title, &current.status) == 3) {
            row++;
            if (row == target_index) {
                *out_book = current;
                return 0;
            }
        }
    }

    return 1;
}

static void book_print_list(FILE *bookfp) {
    char line[512];
    Book current;
    int row = 0;

    if (bookfp == NULL) {
        return;
    }

    rewind(bookfp);
    if (fgets(line, sizeof(line), bookfp) == NULL) {
        return;
    }

    printf("\n--- 本一覧（番号入力で選択可）---\n");
    while (fgets(line, sizeof(line), bookfp) != NULL) {
        if (sscanf(line, "%31[^,],%127[^,],%d", current.jan, current.title, &current.status) == 3) {
            row++;
            printf("%d. %s / %s / %s\n", row, current.jan, current.title, book_status_text(current.status));
        }
    }
    printf("--------------------------------\n\n");
}

static int book_prompt_add_or_cancel(void) {
    char input[16];

    while (1) {
        printf("この本を追加しますか？（a:追加 / c:キャンセル）：");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        input[strcspn(input, "\r\n")] = '\0';
        if (input[0] == 'a' || input[0] == 'A') {
            return 1;
        }
        if (input[0] == 'c' || input[0] == 'C') {
            return 0;
        }

        printf("a または c を入力してください。\n");
    }
}

int book_scan_and_checkout_loop(FILE *bookfp, int *scanned_count, int *accepted_count) {
    char input_jan[BOOK_JAN_MAX] = {0};
    char *endptr;
    Book found;
    int scanned = 0;
    int accepted = 0;
    int selected_index;
    int result;

    if (bookfp == NULL) {
        return -1;
    }

    printf("複数冊の会計モードです。終了するにはqを入力してください。\n");
    // 一覧は最初の1回だけ表示
    book_print_list(bookfp);

    int prompt_shown = 0;
    while (1) {
        memset(input_jan, 0, sizeof(input_jan)); // バッファを毎回クリア
        if (!prompt_shown) {
            printf("本番号を入力してください（q:終了）：");
            fflush(stdout);
            prompt_shown = 1;
        }
        if (fgets(input_jan, sizeof(input_jan), stdin) == NULL) {
            break;
        }

        input_jan[strcspn(input_jan, "\r\n")] = '\0';
        if (input_jan[0] == '\0') {
            memset(input_jan, 0, sizeof(input_jan));
            continue;
        }
        if (strcmp(input_jan, "q") == 0 || strcmp(input_jan, "Q") == 0) {
            break;
        }

        endptr = NULL;
        selected_index = (int)strtol(input_jan, &endptr, 10);
        if (!(endptr != input_jan && *endptr == '\0' && selected_index > 0)) {
            printf("有効な本番号を入力してください。\n");
            continue;
        }

        scanned++;
        result = book_find_by_index(bookfp, selected_index, &found);
        if (result != 0) {
            printf("商品が存在しません。番号を入れ直してください。\n");
            continue;
        }

        printf("バーコード：%s, タイトル：%s, ステータス：%d（%s）\n", found.jan, found.title, found.status, book_status_text(found.status));
        if (!book_prompt_add_or_cancel()) {
            printf("追加をキャンセルしました。\n");
            continue;
        }

        if (found.status == 0) {
            accepted++;
            printf("レンタルリストに追加されました。\n");
        } else if (found.status == 1) {
            printf("この本は現在貸出中です。\n");
        } else if (found.status == 2) {
            printf("この本は延滞中のため、貸出できません。\n");
        }
    }

    if (scanned_count != NULL) {
        *scanned_count = scanned;
    }
    if (accepted_count != NULL) {
        *accepted_count = accepted;
    }

    return 0;
}
