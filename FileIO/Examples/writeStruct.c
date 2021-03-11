#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Struct_t Struct;
struct Struct_t {
    int intProp;
    char strProp[256];
};

int main() {
    char* fileName = "struct.dat";
    FILE* outFile = fopen(fileName, "w"); 
    if (! outFile) {
        fprintf(stderr, "\nError opening file %s.\n", fileName);
        exit(1);
    }
    Struct struct1 = {0, "foo"};
    Struct struct2 = {1, "bar"};
    const size_t numStructs1 = fwrite(&struct1, sizeof(Struct), 1, outFile);
    const size_t numStructs2 = fwrite(&struct2, sizeof(Struct), 1, outFile);
    if ( ! (numStructs1 && numStructs2) ) {
        fprintf(stderr, "\nError writing to file %s.", fileName);
        exit(1);
    }
    fclose(outFile);
    return 0;
}
