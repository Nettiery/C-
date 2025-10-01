#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *file1 = fopen("high_score_snake.txt", "w");
  fprintf(file1, "%d", 0);
  fclose(file1);
  return 0;
}