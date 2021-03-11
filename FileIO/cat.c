#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2 || ! argv[0])
        return -1;
    FILE* fp;
    int c;
    fp = fopen(argv[1], "r");
    while( !feof(fp) ) {
        c = fgetc(fp);
        if ( feof(fp) ) 
            break;
        printf("%c", c);
    }
    fclose(fp);
    return 0;
} 

