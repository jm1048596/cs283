#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    size_t previous_whitespace = 0;     //to track whether the previous char was whitespace
    size_t string_finished = 0;
    size_t user_str_len = 0;
    size_t extra_whitespace = 0;        //used to keep track of the correct position in buff to write to
    
    for (int i=0; i<len; i++){
        if (string_finished) {
            *(buff+i-extra_whitespace) = '.';
        } else if (*(user_str+i) == '\0') {
            string_finished = 1;
            *(buff+i-extra_whitespace) = '.';
            user_str_len = i;
        } else if (*(user_str+i) == ' ' || *(user_str+i) == '\t') {
            if (previous_whitespace) {
                extra_whitespace++;
                len++;
                continue;
            } else {
                *(buff+i-extra_whitespace) = ' ';
                previous_whitespace = 1;
            }
        } else {
            *(buff+i-extra_whitespace) = *(user_str+i);
            previous_whitespace = 0;
        }
    }

    if (!string_finished){
        return -1;
    }

    return user_str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int wc = 0;
    size_t word_start = 0;

    for (int i = 0; i < str_len; i++){
        if (!word_start){
            if (*(buff+i) == ' '){
                continue;
            } else {
                wc++;
                word_start = 1;
            }
        } else {
            word_start = !(*(buff+i) == ' ');
        }
    }
    printf("%d\n", wc);
    return wc;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    the first-evaluated expression checks if there are two or more args
    if that expression is false (meaning there are less than two args),
    the entire expression evaluates to false and *argv[1] is never evaluated
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    At this point, any correct option requires 3 arguments, 
    and this if confirms 3 args have been given
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*)malloc(BUFFER_SZ);
    if (buff == NULL) {
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE