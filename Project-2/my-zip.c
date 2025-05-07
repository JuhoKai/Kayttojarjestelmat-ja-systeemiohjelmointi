#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    
    FILE *file;
    int char_count=0;
    char next_char[2]="";
    char prev_char[2]="";

    //Not enough arguments
    if(argc <= 1){
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    //Go through each file that is given
    for(int filenum=1; filenum < argc; filenum++){

        //Open file. If there is an error print error msg and exits w/ code 1
        if((file = fopen(argv[filenum], "r"))==NULL){
            printf("my-zip: cannot open file\n");
            exit(1);
        }

        //Read each character in the file
        while (fread(next_char, 1, 1, file)){

            //If the next character is the same as the previous character, increase char_count
            if(strcmp(prev_char, next_char) == 0){
                char_count++;
            }

            else{

                //If the character isn't a null character then write the previous character and char_count to the stdout
                if(prev_char[0] != '\0'){
                    fwrite(&char_count, 4, 1, stdout);
                    fwrite(prev_char, 1, 1, stdout);
                }

                //reset char_count and prev_char
                char_count = 1;
                strcpy(prev_char, next_char);
            }
        }

        //Close file
        fclose(file);
    }

    //Write last character
    fwrite(&char_count, 4, 1, stdout);
    fwrite(prev_char, 1, 1, stdout);
    
    return 0;
}