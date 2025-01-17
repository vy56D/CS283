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

//Adds characters into buff while removing leading, trailing and extra whitespaces
int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    if (user_str == NULL){
        return -2;
    }
    int i = 0; //for interating through the user_str
    int j = 0; //for interating through buff
    int spaceCount = 1; //Variable for checking if there has been more than one whitespace. Set to 1 to prevent leading whitespace
    while (*(user_str + i) != '\0'){  //Checks if it is the end
        if (j >= len){     // checks if the buffer still has enough space
            return -1;
        }
        if (*(user_str + i) == ' ' || *(user_str + i) == '\t' || *(user_str + i) == '\n'){
            if (spaceCount!=1){ //only adds to buff if it isn't additional whitespace
                spaceCount=1;
                *(buff + j) = *(user_str + i);
                j++;
            }
        }
        else{
            spaceCount = 0; // reset so it can add whitespace to buff again
            *(buff + j) = *(user_str + i);
            j++;
        }
        i++;
    }
    //removing whitespace from the end
    if (*(buff +j-1) == ' ' || *(buff +j-1) == '\t' || *(buff +j-1) == '\n'){
        *(buff +j-1) = '.';
    }
    while (j<len){ //adding . for the rest of buff
        *(buff + j) = '.';
        j++;
    }

    return i; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

//counts number of words in buff
int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    if (str_len < 1 || buff == NULL){ //checks for invalid str_len and buff inputs
        return -2; //invalid input
    }
    int i = 0;// to interate through buff
    int count = 0; //to the count of spaces
    while (i < len){
        if (*(buff + i) == ' '){ //adds 1 to count if there is a space
            count += 1; 
        }
        i++;
    }
    count+=1; //There is one more word than the number of spaces
    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

//reverses buff then prints
int reverse_character(char *buff, int len, int str_len){
    if (str_len < 1 || buff == NULL || len < str_len){ //checks for invalid inputs
        return -2; //invalid input
    }
    int i = 0; //element from the start
    int j = str_len-1; //element from the end
    while (i<j){
        char temp = *(buff + i); //temperary varible to swap
        *(buff + i) = *(buff + j);
        *(buff + j) = temp;
        i++;
        j--;
    }
    
    return 0; //no errors
}

//prints the word and number of characters in the word on separate lines
int word_print(char *buff, int len, int str_len){
    if (str_len < 1 || buff == NULL || len < str_len){ //checks for invalid inputs
        return -2; //invalid input
    }
    int i = 0; //to iterate through buff
    int count = 0; //to count number of characters
    int word = 1; //to count number of words
    int line_check = 1; //to check when to number the words
    printf("Word Print\n----------\n1. "); //printing the beginning part of word print
    while (i < str_len){
        if (*(buff + i) == ' ' ||  *(buff + i) == '\n' || *(buff + i) == '\t'){
            word++;
            printf("(%d)",count);
            line_check = 0; //resets so the words can be numbered
            count = 0; //resetting character count for each word
        }
        else{
            if (line_check != 1){
                printf("\n%d. ",word); //numbers the word
                line_check = 1;
            }
            count++;
            printf("%c",*(buff + i));
             if (i == str_len-1){
                printf("(%d)",count);
            }
            
        }
        i++;
    }
    printf("\n\nNumber of words returned: %d\n",word); //printing number of words returned
    return 0; //no errors
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This is safe because the if statement checks for argc < 2. If argc < 2 is true then that means argv[1] doesn't exist but since it is an or statement and the first half of the or statement is true, *argv[1] != '-' would not run. Since  *argv[1] != '-' is not run, there will not be an out-of-bounds error therefore making it safe.
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
    // The if statement checks the number of arguments received. If the number of arguments is less than 3 then the usage method is run to print to the screen, showing how to use the program. It then exits with a return code of 1 signifying a command line problem.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL)
    {
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
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
        
        case 'r':
            rc = reverse_character(buff, BUFFER_SZ, user_str_len);  
            if (rc < 0){
                printf("Error reversing buff, rc = %d", rc);
                exit(2);
            }
            break;
        
        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error printing words, rc = %d", rc);
                exit(2);
            }
            break;

        case 'x':
            if (argc != 5)
            {
                usage(argv[0]);
                exit(1);
            }
            else{
                printf("Not Implemented!");
                exit(3);
            }
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
//          By providing both the pointer and the length the coding and debugging process will be easier. 
//          Having the length will make it easier to stay within bounds. It will reduce the amount of errors when coding. 
//          When coding the functions the length and pointer were both used to maintain a level of readability and organization.