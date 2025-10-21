#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>

void print_usage(char *argv0){
    fprintf(stderr, "%s infile", argv0);
}

int main(int argc, char **argv){
    if (argc != 2) {
        print_usage(argv[0]);
        return -1;
    }
    char *infile_path = argv[1];
    FILE *infile = fopen(infile_path, "r");
    if (!infile) {
        perror("Could not open file for reading");
        return -1;
    }

    struct stat st;
    if(stat(infile_path, &st) == -1){
        char msg[1024] = {0}; // Call this enough
        snprintf(msg, 1023, "Cannot stat file %s", infile_path);
        perror(msg);
        fclose(infile);
        return -1;
    }
    size_t infile_size = st.st_size;
    if (infile_size == 0) {
        printf("Input file is empty\n");
        fclose(infile);
        return 0;
    }

    char *infile_buf = malloc(infile_size);
    if(!infile_buf){
        char msg[1024] = {0}; // Call this enough
        snprintf(msg, 1023, "Cannot allocate %lu bytes", infile_size);
        perror(msg);
        fclose(infile);
        return -1;
    }

    int nread = fread(infile_buf, 1, infile_size, infile);
    if(nread != infile_size){
        char msg[1024] = {0}; // Call this enough
        snprintf(msg, 1023, "Expected to read %lu bytes but only got %d bytes", infile_size, nread);
        perror(msg);
        fclose(infile);
        free(infile_buf);
        return -1;
    }
    
    size_t frequency[0x100] = {0};

    for(size_t i = 0; i < infile_size; i++){
        frequency[(uint8_t) infile_buf[i]]++;
    }
    
    for(size_t i = 0; i < sizeof(frequency)/sizeof(frequency[0]); i++){
        if(frequency[i]) printf("%02lx: %lu\n", i, frequency[i]);
    }

    free(infile_buf);
    fclose(infile);
}
