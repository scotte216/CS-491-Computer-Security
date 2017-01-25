#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
const size_t SIZE = 30;
const char USERANDPASS[] = "/home/hw04/hashed.txt";
const char DICTIONARY[] = "/usr/share/dict/words";
const char HASHEDDICT[] = "hashed";
const char FILTERED[] = "filtered";
const char FLAG[] = "flag02.txt";
typedef struct hashNode 
{
    // hash length +1 for /t + 10 for max password size + 1 for terminator
    unsigned char value [SHA_DIGEST_LENGTH*2+12];
    struct hashNode * left;
    struct hashNode * right;
}hashNode;

//Primatives for BST
hashNode * create_node(unsigned char *);
hashNode * add(hashNode *,char *);
char * find(hashNode *, char *);
void freeTree(hashNode * );
void displayAll(hashNode *);
int treeSize(hashNode *);
//other primatives
void trim(char * in);

//Functions
int treeSize(hashNode * root)
{ 
    if(root == NULL) return 0;

    return 1 + treeSize(root->left) + treeSize(root->right);
}

hashNode * add(hashNode * root,char * toAdd)
{
   if (root == NULL)
   {
       
       return create_node(toAdd);
   }

   if (strcmp(root->value,toAdd) > 0 )
      root->left = add(root->left,toAdd);
   else if (strcmp(root->value,toAdd) < 0)
      root->right = add(root->right,toAdd);
   //else it already is in the tree. Do nothing. 
   return root;
}

void freeTree(hashNode * head)
{
   if (head == NULL) return;

   freeTree(head->left);
   freeTree(head->right);
   head->left = NULL;
   head->right = NULL;
   free(head);
   head = NULL;
}

char * find(hashNode * root, char * toFind)
{

   if (root == NULL) return NULL;

   ssize_t read;
   char * readPassword;
   char * readHash;
   char * saveptr;
   char value[SHA_DIGEST_LENGTH*2+12];
   strcpy(value,root->value);

   readHash = strtok_r(value, "\t" , &saveptr);
   readPassword = strtok_r(NULL, "\t" , &saveptr);
   trim(readPassword);
   if (strcmp(readHash,toFind) == 0) 
      return readPassword;
   else if (strcmp(readHash,toFind) > 0) 
      return find(root->left,toFind);
   
   return find(root->right,toFind);
}  

void displayAll(hashNode * head)
{
    if (head == NULL) return;
    displayAll(head->left);
    printf("%s\n",head->value);
    displayAll(head->right);
}

hashNode * create_node(unsigned char * data)
{
   hashNode * new_node = (hashNode *)malloc(sizeof(hashNode));
   strcpy(new_node->value,data);
   new_node-> left = NULL;
   new_node->right = NULL;
   return new_node;
}

void convert_hash_toHex(unsigned char * in,unsigned char * out)
{
  
  static const char* hex_lookup = "0123456789abcdef";
  int i;
  for ( i = 0 ; i < SHA_DIGEST_LENGTH ; i++) {
    out[i*2] = hex_lookup[in[i] >> 4];
    out[i*2+1] = hex_lookup[in[i] & 0x0F]; 
  }
 out[SHA_DIGEST_LENGTH*2] = '\0';
}

void trim(char * in)
{
   char * pos = NULL;
   if ((pos = strchr(in,'\n')) != NULL)
 	*pos = '\0';
}

void createHashDict()
{
   FILE *source;
   FILE *dest;
   int count = 0;
   if (access(HASHEDDICT, F_OK) != -1 )
   {

      dest = fopen(HASHEDDICT,"r");
      int ch = 0;
      while(!feof(dest))
      {
         ch = fgetc(dest);
         if(ch == '\n')
         { 
           count++;
         }
      }
      printf("Hashed dictionary exists with %d entries.\n",count);

   }
   else
   {
      printf("Creating hashed dictionary.\n");
      
      char * line = NULL;
      size_t len = 0;
      ssize_t read;
      unsigned char hash[SHA_DIGEST_LENGTH];
      unsigned char hashAsASCII[SHA_DIGEST_LENGTH*2+1];  
      source = fopen(DICTIONARY, "r");
      if (source == NULL)
      {
          printf("Can't open file %s\n",DICTIONARY); 
          exit(1);
      }
      dest = fopen(HASHEDDICT,"w");
      if (dest == NULL) 
      {
          printf("Can't open file %s\n",HASHEDDICT);
          exit(1);
      }
      while ((read = getline(&line, &len, source)) != -1)
      {
         trim(line);
         char *c = line;
         bool badChar = false;
         while (*c)
         {
            if (strchr("'",*c))
            {  
               badChar = true;
               break; // ends checking for bad characters
            }
            c++;
         }
         //if there was a bad char, continue on to the next line
         if (badChar) continue;

         if (strlen(line) <= 10)
         { 
             //hash and write to new file
            SHA1(line,strlen(line),hash);
            convert_hash_toHex(hash,hashAsASCII);
            fprintf(dest,"%s\t%s\n",hashAsASCII,line);
            count++;
            if (line[0] >= 'A' && line[0] <= 'Z')
            {
               line[0] = line[0]+0x20;
               SHA1(line,strlen(line),hash);
               convert_hash_toHex(hash,hashAsASCII);
               fprintf(dest,"%s\t%s\n",hashAsASCII,line);
               count++;
            }
         }
      }
      printf("Hashed dictionary file created with %d entries.\n",count);
      fcloseall();
      
   }
}

hashNode * populateTree(hashNode * head)
{
  int i=0;
  FILE *fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  
  fp = fopen(HASHEDDICT,"r");
  
  if (fp == NULL)
  {
     printf("Error opening file: %s\n",HASHEDDICT);
     exit(1);
  }

  while ((read = getline(&line, &len, fp)) != -1)
  {
     trim(line);
     head = add(head,line);  
  }
  return head;
}

void crack_password_file(hashNode * head)
{
  int i=0;
  FILE *fp;
  FILE *flags;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char * readName;
  char * readHash;
  char * saveptr;
  char * storedHash;
  char * storedPass;
  char * saveptr2;
  int count = 0;
  fp = fopen(USERANDPASS, "r");
  if (fp == NULL)
  {
     printf("Unable to open file %s\n",USERANDPASS);
     exit(1);
  }
  flags = fopen(FLAG, "w");
  if (flags == NULL)
  {
     printf("Unable to open file %s\n",FLAG);
     exit(1);
  } 
  int skip = 0;
  while ((read = getline(&line, &len, fp)) != -1)
  {
     // First 4 are for problem 1, not problem 2.
     if (skip++ < 4) continue;
     readName = strtok_r(line, "\t" , &saveptr);
     readHash = strtok_r(NULL, "\t" , &saveptr);
     trim(readHash);
     bool matched = false;     
     int i;
     char *foundPassword = find(head,readHash);
     if (foundPassword != NULL)
     {
        count++;
        matched = true;
        fprintf(flags,"%s\t%s\n",readName,foundPassword);
     }
     if (!matched)
     {
         printf("no match for: %s\t%s\n",readName,readHash);
     }
  }
  fclose(fp);
  printf("Passwords found: %d\n",count);
}

int main(int argc, char* argv[])
{
  char name[SIZE];
  char password[SIZE];
  unsigned char hash[SHA_DIGEST_LENGTH];
  hashNode * head = NULL;

  createHashDict();

  head = populateTree(head);

  int uniqueHashes;
  uniqueHashes = treeSize(head);
  printf("Unique hashes found: %d\n",uniqueHashes);

  crack_password_file(head);
  freeTree(head);
  exit(0);
}
