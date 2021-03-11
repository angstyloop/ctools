#include <stdio.h>
#include <stdlib.h>

typedef struct Struct_t Struct;
struct Struct_t {
    int intProp;
    char strProp[256];
};

int main() {
    Struct datStruct;
    char fileName[] = "struct.dat";
    FILE* inFile = fopen(fileName, "r");
    while (fread(&datStruct, sizeof(Struct), 1, inFile)) 
        printf("intProp = %d, strProp = %s\n", datStruct.intProp, datStruct.strProp);
    fclose(inFile);
    return 0;
}
