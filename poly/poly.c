#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "error.h"
#include "poly.h"

struct poly_part_t {
    int c;
    int exp;
};

typedef struct poly_part_t poly_part_t;

struct poly_t {
    int len;
    poly_part_t* polys;
};

typedef struct poly_t poly_t;

static inline int abs_int(int x) {
    int mask = x >> (sizeof(int) * 8 - 1);  // Create a mask of all 1s if x < 0, else all 0s
    return (x + mask) ^ mask;
}

void add_poly(poly_t* poly, poly_part_t* poly_part) {
    for (int i=0; i<poly->len; i++) {
        if (poly->polys[i].exp == poly_part->exp) {
            poly->polys[i].c += poly_part->c;
            return;
        }
    }
    for (int i=0; i<=poly->len; i++){
        if (poly_part->exp > poly->polys[i].exp) {
            for (int j=poly->len-1; j>=i; j--){
                poly->polys[j+1] = poly->polys[j];
            }
            poly->polys[i] = *poly_part;
            poly->len++;
            return;
        }
    }
    poly->polys[poly->len] = *poly_part;
    poly->len++;
    return;
   
}


poly_t* new_poly_from_string(const char* string) {
    int j = 0;
    char a;
    int new_half_len = 2;
    do {
        a = string[j];
        if (a == ' '){
            new_half_len+=1;
        }
        j++;
    } while(a!='\0');
    int new_len = new_half_len/2;


    poly_t* p = malloc(sizeof(poly_t));
    p->len = 0;
    poly_part_t* polys = malloc(sizeof(poly_part_t)*new_len);
    p->polys = polys;
    
    int c = 0;
    int i = 0;
    int ppi = 0;
    int multiplier;
    
    int exp = 0;
    a = string[i];
    while(a!='\0') {
        p->len += 1;
        multiplier = 1;
        exp = 0;
        c = 0;
        if (a == ' ') {
            i ++;
        }
        a = string[i];
        if (a == '-') {
            multiplier = -1;
            if (i == 0){
                i++;
            }
            else {
                i+=2;
            }
        }
        else if (a == '+') {
            i+=2;
        }
        
        while ((a = string[i]) != 'x' && a != '\0') {
            c = c * 10 + (a - '0');
            i++;
        }
        if (a == 'x'){
            i++;
            a = string[i];
            exp = 1;
            if (a == '^') {
                exp = 0;
                i++;
                while ((a = string[i]) != ' ') {  
                    exp = exp * 10 + (a - '0');
                    i++;
                }
            }
        }
        if (c == 0 && exp > 0){
            c = 1;
        }
        p->polys[ppi].c = multiplier*c;
        p->polys[ppi].exp = exp;
        ppi++;
    }
    return p;
}

void free_poly(poly_t* p) {
    free(p->polys);
    free(p);
}

poly_t*	mul(poly_t* p, poly_t*q) {
    poly_t* r = malloc(sizeof(poly_t));
    r->len = 0;
    poly_part_t* polys = malloc(sizeof(poly_part_t)*p->len*q->len);
    r->polys = polys;
    for (int i = 0; i < p->len; i++) {
        for (int j = 0; j < q->len; j++) {
            poly_part_t part_sum;
            part_sum.c = p->polys[i].c * q->polys[j].c;
            part_sum.exp = p->polys[i].exp + q->polys[j].exp;
            add_poly(r, &part_sum);
        }
    }
    return r;
}

void print_poly(poly_t* p) {
    char buf[100];
    int buf_i = 0;
    for (int i = 0; i < p->len; i++) {
        buf_i = 0;
        int c = p->polys[i].c;
        int exp = p->polys[i].exp;
        int abs_c = abs_int(c);
        // printf("\n\nactual %dx^%d\n", c, exp);
        if (c == 0){
            continue;
        }
        if (i > 0) {
            printf(" ");
            // buf[buf_i] = ' ';
            // buf_i++;
        }
        if (c < 0) {
            // printf("- ");
            buf[buf_i] = '-';
            buf_i++;
            buf[buf_i] = ' ';
            buf_i++;
        }
        else if (i > 0) {
            // printf("+ ");
            buf[buf_i] = '+';
            buf_i++;
            buf[buf_i] = ' ';
            buf_i++;
        }
        buf[buf_i]='\0';
        printf("%s", &buf[0]);
        if (abs_c > 1 || exp == 0) {
            printf("%d", abs_c);
            // int t = snprintf(&buf[buf_i], 10, "%d", abs_c);
            // buf_i += t;
        }
        if (exp >= 1){
            // buf[buf_i] = 'x';
            // buf_i+=1;
            printf("x");
        }
        if (exp > 1)
        {
            printf("^%d", p->polys[i].exp);
            // buf[buf_i] = '^';
            // buf_i+=1;
            // int t = snprintf(&buf[buf_i], 10, "%d", exp);
            // buf_i += t;
        }
        // if (i==(p->len)){
        //     buf[buf_i] = '\n';
        //     buf_i++;
        // }
        // buf[buf_i]='\0';
        // printf("%s", &buf[0]);
    }
    // buf[buf_i]='\0';
    printf("\n");
}
