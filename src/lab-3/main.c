#include <stdbool.h>
#include <stdio.h>

void flush_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
int main() {
  /* variable for validate scanf input  */

  int n;

  /* variables for task */

  unsigned char x, y, z;
  char a, b, c;
  /* flag for overflow */

  char is_overflow = 0, is_overflow_signed = 0;

  /* getting user input until its valid  */

  while (true) {
    printf("Please, input 2 numbers (1..255): ");
    n = scanf("%hhu%hhu", &x, &y);
    if (n == EOF) {
      return 0;
    }
    if (n != 2) {
      printf("Invalid input!\n");
      flush_buffer();
      continue;
    }
    break;
  }
  a = (char)x;
  b = (char)y;
  printf("a=%hhu\nb=%hhu\n", x, y);
  __asm__(
      "mov %4,%%al \n\t" /* move x to %al */
      "add %5,%%al \n\t" /* add y to %al */
      "mov %%al,%1 \n\t" /* move %al to z */
      "jnc 1f\n\t"       /* if not overflow -- go forward to first mark*/
      "movb $1,%0 \n\t"  /* else write const "1" to is_overflow */
      "1: \n\t"
      "mov %6,%%al \n\t" /* move a to %al */
      "add %7,%%al \n\t" /* add b to %al (a) */
      "mov %%al,%3 \n\t" /* write result of b+a to c */
      "jno 2f \n\t" /* if not signed overflow -- go forward to second mark */
      "movb $1,%2 \n\t" /* else write const "2" to is_overflow_signed */
      "2: \n\t"
      : "=m"(is_overflow), "=m"(z), "=m"(is_overflow_signed), "=m"(c)
      : "m"(x), "m"(y), "m"(a), "m"(b)
      : "%eax", "%al", "memory", "cc");
  printf("a+b=%d (signed)\na+b=%hhu (unsigned)\n", c, z);
  is_overflow_signed ? printf("overflow (signed)\n")
                     : printf("no overflow (signed)\n");
  is_overflow ? printf("overflow (unsigned)\n")
              : printf("no overflow (unsigned)\n");
  return 0;
}
