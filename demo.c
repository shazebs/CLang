#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  // Variable declaration
  int mysteryNumber = 7;
  int userInput;

  printf("Guess the mystery number: ");

  // Get input from user
  scanf("%d", &userInput);

  // Use of operator to print formatted output
  if (userInput == mysteryNumber)
  {
    printf("Congratulations! You found the mystery number.\n");
  }
  else
  {
    printf("You failed to guess the mystery number by choosing %d.\n", userInput);
  }

  return 1;
}
