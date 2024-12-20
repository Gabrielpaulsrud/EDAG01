#include <stdio.h>
#include <string.h>

int main(){
    // printf("a");
    // printf("b");
    // printf("c");
    // printf("\n");
    int a = 1;
    char buf[100];
    buf[0] = 'a';
    buf[1] = '=';
    // buf[2] = 2+'0';
    // buf[3] = '\n';
    int num = 1432345;
    int t = snprintf(&buf[2], 15, "%d", num);
    printf("t=%d\n", t);
    // char buf[12]; // Enough for a 32-bit int (-2147483648 to 2147483647)
    // int len = snprintf(buf, sizeof(buf), "%d", num);
    buf[5] = '\n';
    buf[6] = '\0';

    printf("%s", buf);
    return 0;
}