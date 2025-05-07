#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_file(FILE *file, char *search){

    char *line = NULL;
    size_t len = 0;

    //Go through each line of the file and find the search term
    while(getline(&line, &len, file) != -1){
        if(strstr(line, search)){
            printf("%s", line);
        }
    }

    //Close file and free used memory
    fclose(file);
    free(line);
}

int main(int argc, char *argv[]){

    FILE *file;

    //Not enough arguments
    if(argc <= 1){
        printf("my-grep: searchterm [file...]\n");
        exit(1);
    }

    //File is specified
    if(argv[2]){

        //Go through each file that is given
        for(int filenum=2; filenum < argc; filenum++){

            //Open file. If there is an error print error msg and exits w/ code 1
            if((file = fopen(argv[filenum], "r"))==NULL){
                printf("my-grep: cannot open file\n");
                exit(1);
            }

            read_file(file, argv[1]);
        }

    //File is not specified
    }else{

        //Set input to stdin
        file = stdin;
        read_file(file, argv[1]);

        //If the program is ended with ctrl+d, no memory is leaked
        //But if the program is ended with ctrl+c, the memory is leaked

    }

    return 0;
}