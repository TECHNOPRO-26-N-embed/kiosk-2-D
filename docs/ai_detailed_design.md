# 支払い方法（F04） 詳細設計書

---

## 1. 目的
会計時に利用者が支払い方法を選択し、決済結果と伝票記録を一貫して管理する。
本設計はCLI実装を前提とし、将来の外部決済連携が可能な構成とする。

---

## 2. 対象範囲
- 対象:
	- 支払い方法選択UI
	- 入力バリデーション
	- 決済処理分岐（現金/クレジットカード/バーコード決済/交通系IC）
	- 伝票CSVへの決済方法記録
	- 決済結果表示
- 非対象:
	- 実際の外部決済ゲートウェイ接続
	- レシート印刷
	- 会計取消・返金

---

## 3. 前提条件
- F03で算出済みの会計金額を受け取る。
- 会計対象のユーザーID、商品情報、返却期限は上位処理で確定済み。
- 伝票記録先CSV（例: data/slip.csv）が存在し、追記可能である。

---

## 4. 入出力仕様

### 4.1 入力
| 項目名 | 型 | 必須 | 仕様 |
|---|---|---|---|
| payment_menu_input | int | 必須 | 1-4の整数 |
| slip_no | int | 必須 | 伝票番号（連番） |
| user_id | int | 必須 | 利用者ID |
| total_amount | int | 必須 | 請求金額（円, 0以上） |
| item_name | string | 必須 | 商品名（書籍名等） |
| register_no | int | 必須 | レジ番号 |
| due_date | string | 必須 | YYYY/MM/DD |

### 4.2 出力
| 項目名 | 型 | 内容 |
|---|---|---|
| selected_payment_method | string | 現金/クレジットカード/バーコード決済/交通系IC |
| payment_result | int | 0:成功, 1:利用者入力エラー, 2:決済失敗, 3:記録失敗 |
| message | string | 利用者向け表示文言 |

---

## 5. データ定義

### 5.1 支払い方法定義（定数またはenum）
| 定数名 | 値 | 表示名 |
|---|---|---|
| PM_CASH | 1 | 現金 |
| PM_CARD | 2 | クレジットカード |
| PM_QR | 3 | バーコード決済 |
| PM_IC | 4 | 交通系IC |

### 5.2 伝票CSV項目
| 列順 | 項目名 | 型 | 備考 |
|---|---|---|---|
| 1 | slip_no | int | 伝票番号 |
| 2 | payment_datetime | string | YYYY/MM/DD HH:MM:SS |
| 3 | user_id | int | 利用者ID |
| 4 | item_name | string | 商品名 |
| 5 | payment_method | string | 支払い方法 |
| 6 | register_no | int | レジ番号 |
| 7 | due_date | string | 返却期限 |
| 8 | total_amount | int | 請求金額 |
| 9 | payment_status | string | SUCCESS/FAILED |

---

## 6. 関数設計

### 6.1 一覧
| 関数名 | 役割 |
|---|---|
| display_payment_options | 支払い方法メニューを表示 |
| select_payment_method | 利用者入力の取得と妥当性確認 |
| process_payment | 支払い方法に応じた決済処理 |
| record_payment_method | 伝票CSVへ決済情報を記録 |
| show_payment_result | 決済結果メッセージ表示 |
| execute_payment_flow | F04全体を統括（推奨追加） |

### 6.2 関数詳細

#### display_payment_options
- 引数: なし
- 戻り値: なし
- 処理:
	1. 「1.現金 2.クレジットカード 3.バーコード決済 4.交通系IC」を表示
	2. 入力案内を表示

#### select_payment_method
- 引数: int max_retry
- 戻り値: int（1-4: 正常, -1: 入力失敗）
- 処理:
	1. 数値入力を受け取る
	2. 1-4以外はエラー表示
	3. max_retry回まで再入力
	4. 超過時は-1を返す

#### process_payment
- 引数: int payment_method, int total_amount
- 戻り値: int（0:成功, 2:失敗）
- 処理:
	1. payment_methodで分岐
	2. 現金は成功固定（将来は預かり金処理を追加可能）
	3. 非現金は外部IF呼び出し想定のスタブを実行
	4. スタブ戻り値を正規化して返却

#### record_payment_method
- 引数:
	- int slip_no
	- const char* payment_datetime
	- int user_id
	- const char* item_name
	- const char* payment_method_name
	- int register_no
	- const char* due_date
	- int total_amount
	- const char* payment_status
- 戻り値: int（0:成功, 3:失敗）
- 処理:
	1. CSVを追記モードで開く
	2. 1行書き込み
	3. flush/close
	4. I/O失敗時は3を返す

#### show_payment_result
- 引数: int payment_result, const char* payment_method_name
- 戻り値: なし
- 処理:
	1. payment_resultに応じたメッセージを表示
	2. 失敗時は再試行可否を表示

#### execute_payment_flow
- 引数: 会計情報一式
- 戻り値: int（0:成功, 1/2/3:失敗）
- 処理:
	1. display_payment_options
	2. select_payment_method
	3. process_payment
	4. record_payment_method
	5. show_payment_result

---

## 7. 処理フロー（詳細）
1. 会計情報を受領する。
2. 支払い方法メニューを表示する。
3. 利用者入力を受け取り、1-4を検証する。
4. 不正入力なら再入力を促し、上限超過で終了する。
5. 決済処理を実行する。
6. 決済結果に応じてステータス（SUCCESS/FAILED）を決定する。
7. 伝票CSVへ記録する。
8. 結果メッセージを表示して呼び出し元へ戻る。

---

## 8. バリデーション
- 支払い方法入力: 1-4の整数のみ許容
- total_amount: 0以上
- user_id/register_no/slip_no: 正の整数
- item_name/payment_method_name: 空文字不可
- due_date/payment_datetime: 規定フォーマット文字列

---

## 9. 異常系設計
| ケース | 条件 | 処理 | 戻り値 |
|---|---|---|---|
| 入力エラー | 1-4以外、または非数値 | 再入力要求、上限で中断 | 1 |
| 決済失敗 | 外部連携スタブが失敗 | 失敗表示、伝票へFAILED記録 | 2 |
| CSV記録失敗 | ファイルオープン/書き込み失敗 | エラーログ出力、画面通知 | 3 |

---

## 11. テスト観点
- 正常: 4種類の支払い方法すべてで成功し、CSVに正しく記録される。
- 入力異常: 0, 5, 文字入力で再入力動作する。
- 決済失敗: 非現金スタブ失敗時にFAILEDで記録される。
- 記録異常: CSV書き込み失敗時に戻り値3になる。

---

## 12. 拡張方針
- 外部決済IFは `process_payment` の内部呼び出しに集約し、UI層から分離する。
- 決済方法追加時は定数・表示文言・分岐・テストケースを同時更新する。
- ログを共通化し、監査用途で伝票番号単位に追跡可能にする。
