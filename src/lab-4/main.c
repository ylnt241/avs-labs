#include <stdio.h>

int main() {
  char szMessage[100] = "Madam, I\'m Adam";
  int count;
  __asm__(
      /* I use "###################### \n\t" to split functions into separate
         sections */
      /* I`ll call this section main, so I can reffer to it later */
      "call clear_reg \n\t" /* mov $0, %rax */
      "call count \n\t"  /* count string lenght and write it to %0 (or int count
                            if you preffer) */
      "mov %0,%%r8 \n\t" /* mov <string lenght>, r8 */
      "decl %%r8d \n\t"  /* getting index from lenght (cuz index starts from 0)
                          */
      "jmp reverse \n\t" /* main logic */
      "################# \n\t"
      "count: \n\t"               /* "func declaration" */
      "cmpb $0,(%1,%%rax,1) \n\t" /* Set flags based on result of 0 -
                                     szMessage[%rax]. Here we are interested in
                                     "ZF" flag, which means that operands are
                                     equal */
      "je exit_count \n\t" /* Checks ZF flag and if its set goto exit_count() */
      "inc %%rax \n\t"     /* increase counter */
      "jmp count \n\t" /* recursive (London is the capital of great britan. Fuck
                          my baka life) calls count() */
      "################# \n\t"
      "exit_count: \n\t"   /* exit point for count "func" */
      "mov %%eax,%0 \n\t"  /* write chars count to int c */
      "jmp clear_reg \n\t" /* mov $0 to rax and rdx, then ret. In this case we
                              returns to initial call from "main()" */
      "################# \n\t"
      "reverse: \n\t"        /* so called (nah) "function declaration" */
      "cmpw %%dx,%%r8w \n\t" /* like cmpb, but for 4 byte long, cuz count is
                                 integer. Can we use 2 byte registers if we go
                                 only to middle of string? Absolutely yes. Will
                                 we do it? Nope, I`m to lazy to rewrite it and
                                 let`s be honest -- is it works? K, I rewrite it
                              */
      "jle exit \n\t" /* if ZF flag is set, or SF flag equals 0 (1 operand
                         greater than second one). Long story short: if %edx >=
                         r8d we goto exit() */
      "mov (%1, %%rdx, 1),%%al \n\t" /* write left part to %al so we can push it
                                        to stack */
      "push %%rax \n\t"              /* push it to stack */
      "mov (%1, %%r8, 1),%%al \n\t"  /* mov right part to %al so we can push it
                                        to stack */
      "push %%rax \n\t"              /* push it to stack */
      "pop %%rax \n\t" /* write the top (like on top, not the highest, c`mon)
                          value from stack to %al */
      "mov %%al,(%1,%%rdx,1) \n\t" /* mov al to szMessage[rdx], so from the
                                      right. We can`t use r8d this btw, cuz we
                                      have %rcx (64-bit) in %1 */
      "pop %%rax \n\t"             /* yep. */
      "mov %%al,(%1,%%r8,1) \n\t"  /* mov al to szMessage[r8], so from the left
                                    */
      "incw %%dx \n\t"             /* increment (for 2 bytes) %dx */
      "decw %%r8w \n\t"            /* decrement (for 2 bytes) %r8w */
      "jmp reverse \n\t"           /* repeat `til the end*/
      "################# \n\t"
      "clear_reg: \n\t" /* it`s not overthinking if you have fun. */
      "mov $0, %%rdx \n\t"
      "mov $0, %%rax \n\t"
      "ret \n\t" /* to so called (only by myself) "main()" */
      "################# \n\t"
      "exit: \n\t" /* just exit from asm part */
      /* If language doesn`t have boilerplate -- write it yourself! */
      : "+m"(count)
      : "p"(szMessage)
      : "%rax", "%rdx", "r8", "memory", "cc");
  printf("%d\n", count);
  printf("%s\n", szMessage);
  return 0;
}
