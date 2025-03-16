#include <stdio.h>
#include <stdlib.h>

// EXTRA CREDIT - print the drexel dragon from the readme.md
extern void print_dragon(){
    FILE *file = fopen("drgn.txt", "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    char line[256];  // Buffer for each line
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    puts("");

    fclose(file);
}