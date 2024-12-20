#include <stdio.h>

int main(){
    // printf("a");
    // printf("b");
    // printf("c");
    // printf("\n");
    char buf[10];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\n';
    buf[4] = '\0';
    printf("%s", buf);
    return 0;
}