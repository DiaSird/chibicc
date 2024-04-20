#include "9cc.h"

// エラーを報告するための関数
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// エラーを報告するための関数 (改善パターン)
// printfと同じ引数を取る
// ...: 可変長引数
// エラー箇所を報告する
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  // エラーの場所を揃える
  // ---------
  //     ^----
  // offset from start address = current position address - start position
  // address
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " "); // pos個の空白を出力
  fprintf(stderr, "^ ");

  // vfprintf() docs:
  // https://www.ibm.com/docs/ja/i/7.3?topic=functions-vfprintf-print-argument-data-stream
  // in Rust: format_arg!()
  // https://github.com/SARDONYX-sard/My-rCore-Tutorial-v3/blob/main/os/src/console.rs#L44
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");

  // debug
  // fprintf(stderr, "pos = %d \n", pos);
  // fprintf(stderr, "loc = %p\n", (void *)loc);
  // fprintf(stderr, "user_input = %p\n", (void *)user_input);
  // fprintf(stderr, "\n");

  exit(1);
}
