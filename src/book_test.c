#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char title[128];
	int code;
	int status;
} Book;

static FILE *open_book_csv(void) {
	FILE *fp = fopen("src/book.csv", "r");
	if (fp != NULL) {
		return fp;
	}
	return fopen("book.csv", "r");
}

int main(void) {
	FILE *bookfp = open_book_csv();
	if (bookfp == NULL) {
		fprintf(stderr, "Failed to open book.csv\n");
		return 1;
	}

	int input_code = 0;
	char line[512];
	Book found = {{0}, -1, -1};

	printf("Input barcode(JAN/code): ");
	if (scanf("%d", &input_code) != 1) {
		fprintf(stderr, "Invalid code input\n");
		fclose(bookfp);
		return 1;
	}

	if (fgets(line, sizeof(line), bookfp) == NULL) {
		fprintf(stderr, "book.csv is empty\n");
		fclose(bookfp);
		return 1;
	}

	while (fgets(line, sizeof(line), bookfp) != NULL) {
		Book current;
		if (sscanf(line, "%127[^,],%d,%d", current.title, &current.code, &current.status) == 3) {
			if (current.code == input_code) {
				found = current;
				break;
			}
		}
	}

	fclose(bookfp);

	if (found.code == -1) {
		printf("No matching book found.\n");
		return 0;
	}

	printf("Found: title=%s, code=%d, status=%d\n", found.title, found.code, found.status);
	return 0;
}
