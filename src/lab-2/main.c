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

  /* variables from task */

  int a, b, c, d, e, x;

  /* getting user input until its valid  */

  while (true) {
    printf("Please, input 5 numbers (1..100): ");
    n = scanf("%d%d%d%d%d", &a, &b, &c, &d, &e);
    if (n == EOF) {
      return 0;
    }
    if (n != 5) {
      printf("Invalid input!\n");
      flush_buffer();
      continue;
    }
    flush_buffer();
    break;
  }
  __asm__(
      "movl %1,%%eax \n\t" /* move first operand to eax register cuz addl can`t
                              do mem-mem */
      "addl %2,%%eax \n\t" /* adding var b and a, with result in eax register */
      "imull %3, %%eax \n\t" /* eax r. times value of var c (c also register
                                here) */
      "subl %4, %%eax \n\t"  /* decrease eax by d value */
      "cltd \n\t"            /* extend sign for division: eax -> edx:eax */
      "divl %5 \n\t" /* div is going to eax, mod to edx. Whenever we didn`t need
                        mod */
      "movl %%eax,%0 \n\t" /* move result from eax to output variable. */
      : "=m"(x)
      : "m"(a), "m"(b), "r"(c), "m"(d), "m"(e)
      : "%eax", "%edx", "memory", "cc");
  printf("Result is: %d\n", x);
  return 0;
}
