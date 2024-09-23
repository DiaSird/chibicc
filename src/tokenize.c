#include "chibicc.h"
#include <time.h>

// 2024-04-19T16:01:21.421205Z DEBUG
// crates\hkx_serde\src\bytes\deserializer.rs:362: class_header:

// Input string
static char *current_input;

// Reports for debug.
void debug(char *file, int line, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  char date[64];
  time_t t = time(NULL);
  strftime(date, sizeof(date), "%Y/%m/%d %a %H:%M:%S", localtime(&t));

  fprintf(stderr, "[Debug: %s] %s:%d: ", date, file, line);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
}

// Reports an error and exit.
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Reports an error location and exit.
static void verror_at(char *loc, char *fmt, va_list ap) {
  int pos = loc - current_input;
  fprintf(stderr, "%s\n", current_input);
  fprintf(stderr, "%*s", pos, ""); // print pos spaces.
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(loc, fmt, ap);
}

void error_tok(Token *tok, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(tok->loc, fmt, ap);
}

// Consumes the current token if it matches `op`.
bool equal(Token *tok, char *op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

// Ensure that the current token is `op`.
Token *skip(Token *tok, char *op) {
  if (!equal(tok, op))
    error_tok(tok, "expected '%s'", op);
  return tok->next;
}

// fn consume
bool consume(Token **rest, Token *tok, char *str) {
  // token == expected str
  if (equal(tok, str)) {
    *rest = tok->next;
    return true;
  }

  *rest = tok;
  return false;
}

// Create a new token.
static Token *new_token(TokenKind kind, char *start, char *end) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

static bool startswith(char *p, char *q) {
  return strncmp(p, q, strlen(q)) == 0;
}

// Returns true if c is valid as the "first" character of an identifier.
static bool is_ident1(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

// Returns true if c is valid as a "non-first" character of an identifier.
static bool is_ident2(char c) { return is_ident1(c) || ('0' <= c && c <= '9'); }

// Read a punctuator token from p and returns its length.
static int read_punct(char *p) {
  if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") ||
      startswith(p, ">="))
    return 2;

  return ispunct(*p) ? 1 : 0;
}

// check keywords
static bool is_keyword(Token *tok) {
  static char *kw[] = {
      "return", "if", "else", "for", "while", "int", "sizeof", "char",
  };

  for (int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
    if (equal(tok, kw[i]))
      return true;

  return false;
}

static Token *read_string_literal(char *start) {
  char *p = start + 1;
  for (; *p != '"'; p++)
    if (*p == '\n' || *p == '\0')
      error_at(start, "unclosed string literal");
  Token *tok = new_token(TK_STR, start, p + 1);
  tok->ty = array_of(ty_char, p - start);
  tok->str = strndup(start + 1, p - start - 1);
  return tok;
}

static void convert_keywords(Token *tok) {
  for (Token *t = tok; t->kind != TK_EOF; t = t->next)
    if (is_keyword(t))
      t->kind = TK_KEYWORD;
}

// Tokenize a given string and returns new tokens.
Token *tokenize(char *p) {
  current_input = p;
  Token head = {0};
  Token *cur = &head;

  while (*p) {
    // Skip whitespace characters.
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Numeric literal
    if (isdigit(*p)) {
      cur = cur->next = new_token(TK_NUM, p, p);
      char *q = p;
      // unsigned long
      cur->val = strtoul(p, &p, 10); // parse_str<Ulong>(&str)
      cur->len = p - q;
      continue;
    }

    // String literal
    if (*p == '"') {
      cur = cur->next = read_string_literal(p);
      p += cur->len;
      continue;
    }

    // Identifier or keyword
    if (is_ident1(*p)) {
      char *start = p;
      do {
        p++;
      } while (is_ident2(*p));
      cur = cur->next = new_token(TK_IDENT, start, p);
      continue;
    }

    // Punctuators
    int punct_len = read_punct(p);
    if (punct_len) {
      cur = cur->next = new_token(TK_PUNCT, p, p + punct_len);
      p += cur->len;
      continue;
    }

    error_at(p, "invalid token");
  }

  cur = cur->next = new_token(TK_EOF, p, p);
  convert_keywords(head.next);
  return head.next;
}
