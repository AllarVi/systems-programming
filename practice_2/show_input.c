#include <stdio.h>
 
int main(int argc, char **argv)
{
    FILE *filePointer;

    char buffer[20];
    
    filePointer = fopen("./input.txt", "r");
    if (filePointer == NULL) { 
        printf("Cannot open file \n"); 
        return 0; 
    } 

    while( fread(buffer, sizeof(buffer), 1, filePointer) == 1 )
    {
        printf("%s", buffer);
    }

    fclose(filePointer); 

    return 0;
}
