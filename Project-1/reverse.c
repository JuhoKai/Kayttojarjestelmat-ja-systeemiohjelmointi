#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// How many characters can one line contain
// -> Lines end with characters \n\0
#define LINEBUF_SIZE 3
#define COLUMNBUF_SIZE 5 

// Helper function that prints given string with \n and \0 showing
void reveal(char *str) {
    for (size_t i = 0; i < LINEBUF_SIZE; ++i) {
        switch (str[i]) {
            case '\n':
                printf("\\n");
                break;
            case '\0':
                printf("\\0");
                break;
            default:
                printf("%c", str[i]);
        }
    }
    printf("\n");
}

int main(void) {
    // Current line buffer
    char line_buf[LINEBUF_SIZE] = {0};
    size_t line_offset_size = sizeof(int) * COLUMNBUF_SIZE;

    int *line_offset = malloc(line_offset_size);
    if (line_offset == NULL) {
        printf("Memory allocation failed");
        return 1;
    }

    // int line_offset[COLUMNBUF_SIZE];
    size_t line_offset_index = 0;

    // Current line pointer
    char *line;
    char *file_buf = (char*)malloc(LINEBUF_SIZE);

    if (file_buf == NULL){
        printf("Memory allocation failed");
        return 1;
    } 

    int num_lines = 0;

    // Read stdin until Control+d
    while ((line = fgets(line_buf, LINEBUF_SIZE, stdin)) != NULL) {
        size_t line_len = strlen(line) + 1;
        line_offset[line_offset_index++] = num_lines;

        // Check if buffer overflow may occur
        if (line_len > LINEBUF_SIZE) {
            printf("Line buffer overflow. Exiting...\n");
            free(file_buf);
            free(line_offset); 
            return 1;
        }

        // Resize line_offset array if needed
        if (line_offset_index >= line_offset_size) {
            line_offset_size += COLUMNBUF_SIZE * sizeof(line_offset[0]);
            
            line_offset = (int*)realloc(line_offset, line_offset_size);

            if (line_offset == NULL) {
                printf("Memory allocation failed");
                free(file_buf);
                free(line_offset);
                return 1;
            }
        }
        
        memcpy(file_buf + num_lines, line, line_len);
        num_lines += line_len;
    }

    for(int i = line_offset_index - 1; i>=0; i--){
        printf("1 DEBUG : %d\n", i);
        printf("%s", file_buf+line_offset[i]);
        printf("2 DEBUG : %d\n", i);
    }
    
    free(file_buf);
    free(line_offset);
    return 0;
}