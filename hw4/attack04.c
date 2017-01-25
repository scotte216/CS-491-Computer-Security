#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

const size_t SIZE = 30;
const size_t SALT = 8;
const char USERANDPASS[] = "/home/hw04/salted.txt";
const char DICTIONARY[] = "/usr/share/dict/words";
const char FLAG[] = "flag04.txt";

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


bool check(char * salt, char * referenceHash,FILE * words,char * match)
{
   char * word = NULL;
   ssize_t read;
   size_t len = 0;
   char * saveptr;
   unsigned char hash[SHA_DIGEST_LENGTH];
   unsigned char hash2[SHA_DIGEST_LENGTH];
   unsigned char convertedHash[SHA_DIGEST_LENGTH*2+1];   
   unsigned char convertedSalt[SALT];
   unsigned char convertedHash2[SHA_DIGEST_LENGTH*2+1];   
   unsigned char convertedSalt2[SALT];
   SHA_CTX ctx;
   SHA_CTX ctx2;
   
int attempts = 0;
   while ((read = getline(&word, &len, words)) != -1)
   {
       attempts++;
       trim(word);

       char *c = word;
       bool badChar = false;
       while (*c)
       {
           if (strchr("'",*c))
           {
               badChar = true;
               break;
           }
           c++;
       }
       if (badChar) continue;

       if (strlen(word) <=10)
       {
           SHA1_Init(&ctx);
           convertHexToBin(salt,convertedSalt);
           SHA1_Update(&ctx,word,strlen(word));
           SHA1_Update(&ctx,convertedSalt,SALT);
           SHA1_Final(hash,&ctx);

           convert_hash_toHex(hash,convertedHash);

           if (strcmp(referenceHash,convertedHash) == 0)
           {
               strcpy(match,word);
               rewind(words);
               return true;
           }

           if (word[0] >= 'A' && word[0] <= 'Z')
           {
               word[0] = word[0]+0x20;
               SHA1_Init(&ctx);
               convertHexToBin(salt,convertedSalt);
               SHA1_Update(&ctx,word,strlen(word));
               SHA1_Update(&ctx,convertedSalt,SALT);
               SHA1_Final(hash,&ctx);
   
               convert_hash_toHex(hash,convertedHash);

               if (strcmp(referenceHash,convertedHash) == 0)
               {   
                   strcpy(match,word);
                   rewind(words);
                   return true;
               }
           }
       }
   }

   rewind(words);
   return false;   
}
int main(int argc, char* argv[])
{

  FILE * users;
  FILE * flags;
  FILE * words;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char * readName;
  char * readSalt;
  char * readHash;
  char * saveptr;
  char password[11];

  users = fopen(USERANDPASS,"r");
  if (users == NULL)
  {
      printf("Unable to open file %s\n",USERANDPASS);
      exit(1);
  }
  
  flags = fopen(FLAG,"w");
  if(flags == NULL)
  {
      printf("Unable to open file %s\n",FLAG);
      exit(1);
  }

  words = fopen(DICTIONARY,"r");
  if (words == NULL)
  {
      printf("Unable to open file %s\n",DICTIONARY);
      exit(1);
  }

  int passwordsCracked = 0;
  int processed = 0;
  int skiplines = 0;
  while ((read = getline(&line, &len, users)) != -1)
  {
      bool result = false;
      password[0] = '\0';
      if (skiplines++ < 4) continue;
      readName = strtok_r(line, "\t" , &saveptr);
      readSalt = strtok_r(NULL, "\t" , &saveptr);
      readHash = strtok_r(NULL, "\t" , &saveptr);
      trim(readHash);
     
      result = check(readSalt,readHash,words,password);
      

      if (result)
      {
          fprintf(flags,"%s\t%s\n",readName,password);
          passwordsCracked++;
          result = false;
      }
      else
      {
          printf("Error -- Password not found for %s\n",readName);
      }
      processed++;
      rewind(words);
  }

  fclose(flags);
  fclose(words);
  fclose(users);
  printf("Users processed: %d\nPasswords cracked: %d\n",processed,passwordsCracked);
  return 0;
}
