#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
const size_t SIZE = 30;
const char FILENAME[] = "/home/hw04/hashed.txt";

void convert_hash_toHex(unsigned char * in,unsigned char * out)
{
  
  static const char* hex_lookup = "0123456789abcdef";
  int i;
  for ( i = 0 ; i < SHA_DIGEST_LENGTH ; i++) {
//    putc(hex_lookup[in[i] >> 4],stdout);
    out[i*2] = hex_lookup[in[i] >> 4];
//    putc(hex_lookup[in[i] & 0x0F],stdout);
    out[i*2+1] = hex_lookup[in[i] & 0x0F]; 
  }
 out[SHA_DIGEST_LENGTH*2] = '\0';
}

void print_hex(unsigned char *buf, int len)
{
  int i;

  for(i=0;i<len;i++){
    printf("%02x",buf[i]);
  }
}

void trim(char * in)
{
   char * pos = NULL;
   if ((pos = strchr(in,'\n')) != NULL)
 	*pos = '\0';
}

unsigned char * getHashFromFile(char * name)
{
  FILE *fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char * readName;
  char * readHash;
  char * saveptr;
  fp = fopen(FILENAME,"r");
  
  if (fp == NULL)
	exit(1);
  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1)
  {
     readName = strtok_r(line, "\t" , &saveptr);
     readHash = strtok_r(NULL, "\t" , &saveptr);
     trim(readHash);

//     printf("Read name: %s\nname to check: %s\n",readName,name);
     if ( strcmp(readName,name) == 0)
     {
        return readHash;
     }
     
     //printf("Read name: %s\nRead hash: %s\n",readName,readHash);
     //20 bits is 40 ascii characters, +1 for terminator.
     //Problem 1 only calls for looking at the first 4 
     if( i++ >= 3) break;
     
  }

  fclose(fp);
  return NULL;
}

bool check(char * user, unsigned char * hash)
{
   unsigned char * hashFromFile;
   hashFromFile = getHashFromFile(user);
 
   unsigned char convertedHash[SHA_DIGEST_LENGTH*2+1];   
   convert_hash_toHex(hash,convertedHash);  

   if (hashFromFile != NULL && strcmp(hashFromFile,convertedHash) == 0)
   {
      return true;
   }

   return false;   
}

int main(int argc, char* argv[])
{
  char name[SIZE];
  char password[SIZE];
  unsigned char hash[SHA_DIGEST_LENGTH];

  printf("Username: ");
  fgets(name,SIZE,stdin);
  trim(name);

  printf("Password: ");
  fgets(password,SIZE,stdin);
  trim(password);

//  printf("You entered: %s and %s\n",name,password);
  
  SHA1(password,strlen(password),hash);
 // printf("Your hash is: ");
 // print_hex(hash, SHA_DIGEST_LENGTH);
//  printf("\n");
    
  bool result = check(name,hash); 
  if (result)
    printf("Success!\n");
  else
    printf("ACCESS DENIED\n"); 

  return 0;
}
