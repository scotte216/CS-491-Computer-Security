#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

const size_t SIZE = 30;
const size_t SALT = 8;
const char USERANDPASS[] = "salted.txt";
const char DICTIONARY[] = "words";
const char FLAG[] = "flag02.txt";

//primatives
void trim(char * in);

void convert_hash_toHex(unsigned char * in,unsigned char * out)
{
  int length = SHA_DIGEST_LENGTH;
  static const char* hex_lookup = "0123456789abcdef";
  int i;
  for ( i = 0 ; i < length ; i++) {
    out[i*2] = hex_lookup[in[i] >> 4];
    out[i*2+1] = hex_lookup[in[i] & 0x0F]; 
  }
 out[length*2] = '\0';
}

void convertHexToBin(unsigned char * in,unsigned char out[SALT])
{
    int len = 16;
    int i;
    int last;
    int index = 0;
    for (i = 0; i < len ; i++)
    {
       unsigned char buff[2];
       buff[0] = in[i++];
       buff[1] = in[i];
       out[index++] = (unsigned char) strtol(buff,NULL,16);
    }
}

void trim(char * in)
{
   char * pos = NULL;
   if ((pos = strchr(in,'\n')) != NULL)
 	*pos = '\0';
}


bool getHashFromFile(char * name,unsigned char * salt,unsigned char * hash)
{
  FILE *fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char * readName;
  char * readSalt;
  char * readHash;
  char * saveptr;

  fp = fopen(USERANDPASS, "r");
  if (fp == NULL)
  {
        printf("Unable to open file %s\n",USERANDPASS);
	    exit(1);
  }

  int linesRead = 0;
  while ((read = getline(&line, &len, fp)) != -1)
  {
     readName = strtok_r(line, "\t" , &saveptr);
     readSalt = strtok_r(NULL, "\t" , &saveptr);
     readHash = strtok_r(NULL, "\t" , &saveptr);
     trim(readHash);
 
     if ( strcmp(readName,name) == 0)
     {
        strcpy(salt,readSalt);
        strcpy(hash,readHash);
        
        return true;
     }
     
     //Problem 1 only calls for looking at the first 4 
     if( linesRead++ >= 3) break;
     
  }
  fclose(fp);
  return false;
}
bool check(char * user, char * password)
{
   unsigned char saltFromFile[17];
   unsigned char hashFromFile[41];
   char * saveptr;
   unsigned char hash[SHA_DIGEST_LENGTH];
   unsigned char convertedHash[SHA_DIGEST_LENGTH*2+1];   
   unsigned char convertedSalt[SALT];
   bool result = false;   
   SHA_CTX ctx;
   
   SHA1_Init(&ctx);

   result = getHashFromFile(user,saltFromFile,hashFromFile);

   if (!result) return false;

   convertHexToBin(saltFromFile,convertedSalt);

   SHA1_Update(&ctx,password,strlen(password));
   SHA1_Update(&ctx,convertedSalt,SALT);
   SHA1_Final(hash,&ctx);

   convert_hash_toHex(hash,convertedHash);  

   if (strcmp(hashFromFile,convertedHash) == 0)
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

  bool result = check(name,password); 
  if (result)
    printf("Success!\n");
  else
    printf("ACCESS DENIED\n"); 

  return 0;
}
