#include "chibicc.h"
#include <time.h>

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
