// Scott Ewing
// CS 491: Intro to Computer Security 
// PSU Fall 2015
// Assignment 1
// filename: questioner.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFF_SIZE = 256;
const char FILENAME[] = "answers.txt";
//Poked around online to get a 'safer' version of scanf and found this
//implementation online. Source:
//stackoverflow.com/questions/1694036/why-is-the-gets-function-so-dangerous-that-it-should-not-be-used
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp)
{
    if (fgets(buffer, buflen, fp) != 0)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[len-1] = '\0';
        else
        {
            int ch;
            while ((ch = getc(fp)) != EOF && ch != '\n')
                ;
        }
        return buffer;
    }
    return 0;
}

int main ()
{
    int magicNumber, guess, numFromFile;
    char userName[BUFF_SIZE], terminate, nameFromFile[BUFF_SIZE];
    FILE *file;

    //default magic number
    magicNumber = 12345;

    //Determine their name
    printf("What is your name?\n");
    fgets_wrapper(userName, sizeof(userName), stdin);

    //Open and load the file. 
    file = fopen(FILENAME,"r");
    if (file == NULL)
    {
        fprintf(stderr,"ERROR: Can't open file %s\n",FILENAME);
        exit(1);
    }
    else
    {
        int result;
        //This will read line by line from the file, but it vulnerable to buffer
        //exploits. 
        while ((result = fscanf(file, "%s %d", nameFromFile, &numFromFile)) == 2)
        {
            if (strncmp(userName, nameFromFile, sizeof(userName)) == 0 )
            {
                magicNumber = numFromFile;
                break;
            }
        }
       
        if (result != EOF && result != 2)
        {
            fprintf(stderr, "ERROR: Invalid file format in %s\n",FILENAME);
            exit(1);
        } 
    }
    fclose(file); 

    printf("What is the magic number, %s?\n",userName);

    //Checks to ensure they enter an integer followed by newline
    if (scanf("%d%c",&guess, &terminate) != 2 || terminate != '\n')
    {
        printf("INVALID GUESS\n");
        exit(1);
    }

    while (guess != magicNumber)
    {
        if (guess < magicNumber)
        {
            printf("TOO LOW\n");
        }
        else
        {
            printf("TOO HIGH\n");
        }

        if (scanf("%d%c",&guess, &terminate) != 2 || terminate != '\n')
        {
            printf("INVALID GUESS\n");
            exit(1);
        }
    }
 
    printf("SUCCESS\n");

    exit(0); 
}
