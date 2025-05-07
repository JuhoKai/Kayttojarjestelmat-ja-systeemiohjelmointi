#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    FILE *file;
    int char_count;
    char compressed_char[5];

    //Not enough arguments
    if(argc <= 1){
        printf("my-unzip: \n");
        exit(1);
    }

    //Go through each file that is given.
    for(int filenum=1; filenum < argc; filenum++){

        //Open file. If there is an error print error msg and exits w/ code 1
        if((file = fopen(argv[filenum], "r"))==NULL){
            printf("my-unzip: cannot open file\n");
            exit(1);
        }

        //Read 4-byte integer + char = 5-byte entrie
        while (fread(&compressed_char, 5, 1, file)) {

            //Convert 4-byte integer
            //Source: https://cboard.cprogramming.com/c-programming/87461-how-read-4-consecutive-bytes-integer.html
            char_count = compressed_char[0] | ( (int)compressed_char[1] << 8) | ( (int)compressed_char[2] << 16) | ((int)compressed_char[3] << 24);

            //Reserve memory for the amount of characters
            char *char_string = malloc(char_count + 1);

            //Repeat read character according to char_count
            for (int i=0; i<char_count; i++){
                char_string[i] = compressed_char[4];
            }

            //Print string of characters
            printf("%s", char_string);

            //Free used memory
            free(char_string);
        }
        
        //Close file
        fclose(file);
    }

    return 0;
}