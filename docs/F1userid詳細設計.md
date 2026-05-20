# F1（User ID）詳細設計（関数シグネチャ）

---

## 1. ユーザー情報取得・表示関連


ユーザーCSVファイルを読み込み、User配列に格納する
 ```c
    int LoadUsers(const char *filename, User users[], int max_count);
 ```
ユーザーIDでUser配列から該当ユーザーを検索する
 ```c
    int FindUserByID(const User users[], int count, int input_id);
 ```
ユーザー情報を画面に表示する
 ```c
    void ShowUserInfo(const User *user);
 ```
ユーザーID入力を受け付ける
 ```c
    int InputUserID(void);
 ```
ユーザーIDが存在しない場合のエラーメッセージ表示
 ```c
    void ShowUserNotFoundMessage(int input_id);
 ```

---

## 2. 画面遷移制御

ユーザーID入力画面の制御
 ```c
    void UserIDInputScreen(const User users[], int user_count);
```

---

## 3. 例外処理

入力値が不正な場合のエラーメッセージ表示
 ```c
    void ShowInvalidInputMessage(void);
```
