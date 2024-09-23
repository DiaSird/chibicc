#include "test.h"

int sum(int n) { return (n * (n + 1) * (2 * n + 1) / 6); }

int sum_test(int n) {
  int i;
  int ans = 0;

  for (i = 0; i <= n; i = i + 1) {
    ans = ans + i * i;
  }

  return ans;
}

int main() {
  int n = 4096;

  iprintf("sum of n =", n);
  ASSERT(sum(n), sum_test(n));

  printf("OK\n");
  return 0;
}
