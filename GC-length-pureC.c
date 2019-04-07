#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// taken from http://seqanswers.com/forums/showthread.php?t=7059 
// slightly modified

int main(int argc, char *argv[]) {
    char *name = NULL, line[16300]; //maximum line length in my transcriptome assembly
    int i;
    unsigned long long GC, AT, length;
    FILE *fp;

    if(argc != 2 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: %s file.fa\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r");
    while(fgets(line, 16300, fp) != NULL) {
        if(*line == '>') {
            if(name != NULL) {
                printf("%s\t%f\t%l\n", name, ((float) GC)/(GC+AT), length);
                free(name);
            }
            GC = AT = length = 0;
            name = strdup(line+1);
            for(i=0;i<strlen(name);i++) { //trim off line endings or descriptors
                if(*(name+i) == ' ' || *(name+i) == '\n') {
                    *(name+i) = '\0';
                    break;
                }
            }
        } else {
            i=0;
            while(1) {
                if(*(line+i) == 'G' || *(line+i) == 'C' || *(line+i) == 'g' || *(line+i) == 'c') GC++;
                else AT++;
                if(*(line+ (++i)) == '\n') {
                    length = i;
                    break;
                }
            }
        }
    }
    if(name != NULL) {
        printf("%s\t%f\t%l\n", name, ((float) GC)/(GC+AT), length);
        free(name);
    }
    fclose(fp);
    return 0;
} 
