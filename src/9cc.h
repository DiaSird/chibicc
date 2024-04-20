
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
};

// 現在着目しているトークン
Token *token;

// 入力プログラム
char *user_input;

extern Token *token;
extern char *user_input;

#ifdef __9CC__
//--- グローバル変数の定義
// 現在着目しているトークン
Token *token;

// 入力プログラム
char *user_input;
#endif
