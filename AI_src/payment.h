//Tanaka

#ifndef PAYMENT_H
#define PAYMENT_H

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

int payment_select_method(int max_retry);
const char *payment_method_label(int method);
int payment_process(int method, int amount);
int payment_run_flow(int amount, int max_retry);

#endif
