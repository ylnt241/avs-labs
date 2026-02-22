#include <stdio.h>
void flush_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void print_l_endian(unsigned char *char_pointer, unsigned int size) {
  unsigned int i, j;
  unsigned char byte;

  for (i = 0; i < size; i++) {
    for (j = 0; j < 8; j++) {
      unsigned char byte = char_pointer[i];
      if (byte & 0x80 >> j)
        printf("1");
      else
        printf("0");
    }
    printf(" ");
  }
  printf("\n");
}

void print_b_endian(unsigned char *char_pointer, unsigned int size) {
  unsigned int i, j;
  unsigned char byte;

  for (i = size; i > 0; i--) {
    for (j = 0; j < 8; j++) {
      unsigned char byte = char_pointer[i - 1];
      if (byte & 0x80 >> j)
        printf("1");
      else
        printf("0");
    }
    printf(" ");
  }
  printf("\n");
}

int main() {
  int n;
  int int_buffer;
  float float_buffer;
  double double_buffer;

  printf("Please, write your input: ");
  n = scanf("%d", &int_buffer);
  flush_buffer();
  if (n == EOF) {
    printf("\n");
    return 0;
  }
  if (n != 1) {
    printf("Invalid Input!\n");
    return 1;
  }
  float_buffer = (float)int_buffer;
  double_buffer = (double)int_buffer;

  printf("===INT===\n");
  printf("---\nLittle Endian:");
  print_l_endian((unsigned char *)&int_buffer, sizeof(int));
  printf("---\nBig Endian:");
  print_b_endian((unsigned char *)&int_buffer, sizeof(int));
  printf("===FLOAT===\n");
  printf("---\nLittle Endian:");
  print_l_endian((unsigned char *)&float_buffer, sizeof(float));
  printf("---\nBig Endian:");
  print_b_endian((unsigned char *)&float_buffer, sizeof(float));
  printf("===DOUBLE===\n");
  printf("---\nLittle Endian:");
  print_l_endian((unsigned char *)&double_buffer, sizeof(double));
  printf("---\nBig Endian:");
  print_b_endian((unsigned char *)&double_buffer, sizeof(double));
  return 0;
}
