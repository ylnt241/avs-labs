#include <stdio.h>

int main() {
  int n = 4;

  __asm__ (
    "mov %0,%%edx \n\t" /* we mov number, from which we calculate factorial to %edx */
    "push %%rdx \n\t" /* we push %rdx (which store our original number in first 32 bits) to stack */
    "call fact_check \n\t" /* we call fact_check(), so now in stack we have 1. return addr, 2. %rdx */
    "add $8,%%rsp \n\t" /* remove obsolete number,
                           which we put before calling fact_check(), from stack */
    "mov %%eax, %0 \n\t" /* write result to %0 */

    "jmp exit \n\t" /* literally me. Ugh.. I mean exit */

    "############ \n\t"

    "fact_check: \n\t"
      "cmpl $1,8(%%rsp) \n\t" /* %rsp points to top of stack (in that case ret addr),
                                 so 8(%%rsp) means 8-byte offset from top of stack, where we store %rdx.
                                 Futhermore, because we use cmpL (4 bytes comparasion), we take only first 4 bytes from this addr.
                                 And just like that we compare 1 and %edx
                              */
      "jg fact_calc \n\t"     /* if second operand bigger than $1, go to fact_calc. Here we dont put ret addr in stack, cuz its not
                              "call"
                              */
      "mov $1,%%eax \n\t"
      "ret \n\t"

    "############ \n\t"

    "fact_calc: \n\t"
      "mov 8(%%rsp),%%edx \n\t" /* we copy number from 'top of stack + 8 bytes' addr to %edx.
                                    In first iteration it doesn`t make sense, because we already store that number in %edx
                                */
      "push %%rdx \n\t"         /* we push %rdx to stack, so now we have 1. %rdx, 2. ret addr, 3. number that we put there before */
      "decl (%%rsp) \n\t"       /* we decrement number that stores on top of stack */
      "call fact_check \n\t"    /* we call fact_check again, so now it will check n - 1 number */
      "add $8,%%rsp \n\t"       /* we remove one element from top of stack, because it can be only 1 or number that we already have */
      "mull 8(%%rsp) \n\t"      /* multiply current number by %eax and write it to %eax. */
      "ret \n\t"


      /*
       * Basically, each call of fact_calc put in stack number and return addr (to :38) and we call it unless 8(%rsp) != 1. If 8(%rsp) == 1, we do "ret", so we return to :38 which all number sequence and its ret addresses in stack.
       * After that, we do "add $8, %%rsp", so we remove last element from stack, multiply next number (after ret addr) with %eax, store result in %eax and jump to next number. After we jump to next number we have [number from previous step] [ret addr] [our number] [etc..], so we want to remove first element, and multiply element that stays after [ret addr].
       *
       * Lets watch an example: fact for 3.
       * At "main()" we push 3 to %edx.
       * Push full %rdx to stack
       * Call fact_check
       * At that moment we have in stack: [ret main()] [3]
       * We compare 1 and 3, because 3 > 1 we jump to fact_calc.
       * We copy 3 to %edx
       * We push rdx to stack, now we have [3] [ret fact_check()] [ret main()] [3]
       * We decrement 3, so now we have [2] [ret fact_check()] [ret main()] [3]
       * We call fact_check again
       * At that moment we have in stack [ret fact_calc() ][2] [ret fact_check()] [ret main()] [3]
       * We compare 1 by 2 and cuz 2 > 1 jump to fact_calc(). Stack doesnt change at this point.
       * We copy 2 to %edx.
       * We push 2 to stack, so now we have [2] [ret fact_calc() ] [2] [ret fact_check()] [ret main()] [3]
       * We decrement 2 and now we have [1] [ret fact_calc() ] [2] [ret fact_check()] [ret main()] [3]
       * We call fact_check() and now we have [ret fact_calc()] [1] [ret fact_calc() ][2] [ret fact_check()] [ret main()] [3]
       * We compare 1 and 1 and cuz 1 = 1, we write 1 to %eax. We need to do it because after ret control will go to "add", not start of the fact_calc() func
       * We ret and now we have [1] [ret fact_calc() ][2] [ret fact_check()] [ret main()] [3] at stack. We returned to next instruction after ret addr, at this case its on :38.
       * We remove [1] from stack, because its useless in case with 1 and we already have this number in case where number >1. Now we have [ret fact_calc()] [2] [ret fact_check()] [ret main()] [3]
       * We multiply 2 by %eax and store it to %eax
       * We do ret and now we have [2] [ret fact_check()] [ret main()] [3]. See? We already multiplyed eax by 2, so we dont need it.
       * We remove element from top of stack
       * We multiply 3 by eax and store result in eax
       * We return to main() (:10)
       * We remove obsolete [3] from stack
       * We write result to %0, in our case its int n
       * Jump to exit
       * Yay!
       *

       */
    "############ \n\t"

    "exit: \n\t"
    : "+m"(n)
    :: "memory", "cc", "%rax", "%rdx"
  );
  printf("%d\n", n);
  return 0;
}
