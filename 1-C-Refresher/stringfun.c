#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
void  reverse_string(char *, int);
void  word_print(char *, int);
char* search_and_replace(char *, int, int, char *, char *);


int setup_buff(char *buff, char *user_str, int len){
    int previous_whitespace = 1;     //to track whether the previous char was whitespace
    int string_finished = 0;
    int user_str_len = 0;
    int extra_whitespace = 0;        //used to keep track of the correct position in buff to write to

    for (int i=0; i<len; i++){
        if (string_finished) {
            *(buff+i-extra_whitespace) = '.';
        } else if (*(user_str+i) == '\0') {
            string_finished = 1;
            *(buff+i-extra_whitespace) = '.';
            user_str_len = (i-extra_whitespace);

            //special case where final char was whitespace
            if (*(buff+i-extra_whitespace-1) == ' ' || *(buff+i-extra_whitespace-1) == '\t') {
                *(buff+i-extra_whitespace-1) = '.';
            }
        } else if (*(user_str+i) == ' ' || *(user_str+i) == '\t') {
            if (previous_whitespace) {
                extra_whitespace++;
                len++;              //len is incremented to ensure the loop runs enough times despite the extra whitespace
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
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int str_len){
    int wc = 0;
    int word_start = 0;

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
    return wc;
}

void reverse_string(char *buff, int str_len) {
    char tmp_char;
    int end_idx = str_len - 1;

    for (int i=0; i<(str_len/2); i++) {
        tmp_char = *(buff+i);
        *(buff+i) = *(buff+end_idx-i);
        *(buff+end_idx-i) = tmp_char;
    }

    printf("Reversed String: ");
    for (int i=0; i<str_len; i++) {
        printf("%c", *(buff+i));
    }
    puts("");

    return;
}

void word_print(char *buff, int str_len) {
    int last_char_idx = str_len-1;  //index of last char - strlen(str)-1;
    int wc = 0;         //counts words
    int wlen = 0;       //length of current word
    int word_start = 0;    //am I at the start of a new word

    for (int i = 0; i < str_len; i++){
        if (!word_start){
            if (*(buff+i) == ' '){
                continue;
            } else {
                wc++;
                word_start = 1;
                wlen = 1;
                printf("%d. ", wc);
                printf("%c", *(buff+i));
            }
        } else {
            if (*(buff+i) == ' '){
                printf(" (%d)\n", wlen);
                word_start = 0;
                wlen = 0;
            } else {
                wlen++;
                printf("%c", *(buff+i));
            }
        }
        if (i == last_char_idx) {
            printf(" (%d)\n", wlen);
        }
    }
}

char* search_and_replace(char *buff, int len, int str_len, char *old, char *new) {
    int matching = 0;
    size_t chars_into_old = 0;
    size_t copy_from_index;         // index directly following the matched word
    int bytes_from_beginning;       // amount of bytes to copy from the beginning of the buffer.
    int new_str_len = str_len;
    char *temp_old = old;
    char *temp_new = new;
    int previous_space = 1;

    // checking if the new string would be too long
    while (*temp_old != '\0') {
        new_str_len--;
        temp_old++;
    }

    while (*temp_new != '\0') {
        new_str_len++;
        temp_new++;

        if (new_str_len > len) {
            puts("New string would be too long. Exiting...");
            exit(-1);
        }
    }

    // searching for a match section (only switches once with this implementation)
    for (int i=0; i<str_len; i++) {
        if (*(buff+i) == *(old+chars_into_old)){
            if (!matching && previous_space) {        //start of potential match
                matching = 1;
                bytes_from_beginning = i;  
            }
            chars_into_old++; 
        } else {
            if (matching) {             //thought we had a match but didn't, reset the vars
                chars_into_old = 0;
                matching = 0;
            }
        }

        if (matching && *(old+chars_into_old) == '\0' && *(buff+i+1) == ' ') {    //found the match, switch it
            copy_from_index = i+1;
            char *new_buff = (char*)malloc(BUFFER_SZ);
            if (buff == NULL) {
                puts("Error allocating memory");
                exit(99);
            }

            memcpy(new_buff, buff, bytes_from_beginning);       //copy over first part

            char *ptr_to_beg = new_buff;        //for return later
            //copy over the new word
            for (int i=1; i<=bytes_from_beginning; i++) {new_buff++;}
            while (*new != '\0') {
                *new_buff = *new;
                new_buff++;
                new++;
            }

            int len_diff = new_str_len - str_len;
            //copy over second part and fill the rest with .
            memcpy(new_buff, buff+copy_from_index, BUFFER_SZ-copy_from_index-len_diff);
            memset((ptr_to_beg+new_str_len+1), '.', (BUFFER_SZ-new_str_len+1));

            printf("Modified String: ");
            for (int i=0; i<=new_str_len; i++) {
                printf("%c", *(ptr_to_beg+i));
            }
            puts("");

            free(buff);
            return ptr_to_beg;
        }

        previous_space = (*(buff+i) == ' ');
    }

    //returning pointer to original buffer if the word was not found, essentially changing nothing
    return buff;
}

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

    buff = (char*)malloc(BUFFER_SZ);
    if (buff == NULL) {
        puts("Error allocating memory");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        
        case 'r':
            reverse_string(buff, user_str_len);
            break;

        case 'w':
            puts("Word Print");
            puts("----------");
            word_print(buff, user_str_len);
            break;

        case 'x':
            if (argc < 5){
               usage(argv[0]);
                exit(1);
            }
            
            buff = search_and_replace(buff, BUFFER_SZ, user_str_len, argv[3], argv[4]);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

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
/*
Providing both the pointer to and length of the buffer can help ensure we don't write to
memory we don't "own". It also helps in a function like search_and_replace where a
new buffer of identical size is made.
*/