#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include "zaitypes.h"
const char* read_shd_file(const char* file) {

    FILE *fptr = fopen(file, "r");
    if(fptr == NULL) {
        printf("[ERROR] Could not open file\n");
        return NULL;
    }
    printf("fdjkf"); 
    char *buffer = NULL;
    size_t i = 0;
    i32 c;
    while((c = fgetc(fptr)) != EOF) {
        char shd_character = (char) c;
        buffer[i++] = shd_character; 
    }
    printf("fdfgf");
    if(ferror(fptr)) {
        printf("[ERROR] When reading\n");
    }

    fclose(fptr);
    return buffer;
}

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return NULL;
    }

    char *buffer = NULL;
    int buffer_size = 0;
    int character;

    while ((character = fgetc(file)) != EOF) {
        if (buffer_size == 0) {
            buffer = (char*) malloc(1);
            if (!buffer) {
                fprintf(stderr, "Failed to allocate memory\n");
                fclose(file);
                return NULL;
            }
        } else {
            char *new_buffer = (char*) realloc(buffer, buffer_size + 1);
            if (!new_buffer) {
                fprintf(stderr, "Failed to reallocate memory\n");
                fclose(file);
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }

        buffer[buffer_size++] = (char) character;
    }

    fclose(file);

    if (buffer_size == 0) {
        return NULL;
    }

    buffer[buffer_size] = '\0';

    return buffer;
}

int main(void) {

    
    printf("hello\n");
    const char* vertex = read_file("vertex.glsl");
    printf("%s", vertex); 
    const char* frag = read_file("frag.glsl");
    printf("hello\n");
    printf("%s", frag); 
    return 0;

}


