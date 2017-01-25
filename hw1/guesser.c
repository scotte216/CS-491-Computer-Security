// Scott Ewing
// CS 491: Intro to Computer Security
// PSU Fall 2015
// Assignment 1
// Filename: guesser.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

const int BUFF_SIZE = 256;
const char myName[] = "Bob";

//Poked around online to get a 'safer' version of scanf and found this
//implementation
//source:
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

int main()
{
    char input[BUFF_SIZE], prompt[BUFF_SIZE];
    int guess = 0;
    int highest = INT_MAX;
    int lowest = INT_MIN;

    //Get the response
    fgets_wrapper(input, sizeof(input), stdin);

    //Ensure the response is what we expect
    if (strncmp(input,"What is your name?", sizeof(input)) != 0 ) 
    {
        fprintf(stderr, "???\n");
        exit(1);
    }

    //Display name.
    fprintf(stdout,"%s\n",myName);

    //Get response
    fgets_wrapper(input, sizeof(input), stdin);

    //Create a string of our expected prompt.
    snprintf(prompt, sizeof(prompt), "What is the magic number, %s?" , myName);

    //Check response to match the expected prompt
    if (strncmp(input, prompt, sizeof(input)) != 0 )
    {
        fprintf(stderr, "???\n");
        exit(1);
    }

    //Continue guessing until SUCESS is reached
    while ( strncmp(input,"SUCCESS",sizeof(input)) != 0)
    {
        fprintf(stdout,"%d\n",guess);
        fgets_wrapper(input, sizeof(input), stdin);

        //If the guess is too high, pick the number between current guess and our
        //max guess so far
        if ( strncmp(input,"TOO HIGH",sizeof(input)) == 0 )
        {
            highest = guess;
            guess = (lowest - guess) / 2 + guess;
        }
        //If the guess is too low, pick a number between the current guess and
        //the lowest guess so far
        else if ( strncmp(input,"TOO LOW", sizeof(input)) == 0)
        {
            lowest = guess;
            guess = (highest - guess) / 2 + guess;
        }
        //If the response at this point is anything other than SUCCESS there is
        //an error.
        else if (strncmp(input,"SUCCESS", sizeof(input)) != 0)
        {
            fprintf(stderr, "???\n");
            exit(1);
        }
    }

    exit(0);
}
