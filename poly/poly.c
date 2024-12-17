#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

poly_t* new_poly_from_string(const char* string) {
    poly_part_t* polys = malloc(sizeof(poly_part_t)*10);
    poly_t* p = malloc(sizeof(poly_t));
    p->len = 10;
    p->polys = polys;
    
    int c = 0;
    char a;
    int i = 0;
    int ppi = 0;
    int multiplier;
    int real_length = 0;
    
    int exp = 0;
    a = string[i];
    while(a!='\0') {
        real_length += 1;
        multiplier = 1;
        exp = 0;
        c = 0;
        if (a == ' ') {
            i ++;
        }
        a = string[i];
        if (a == '-') {
            multiplier = -1;
            i+=2;
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
                    printf("char: %c\n", a);
                    exp = exp * 10 + (a - '0');
                    printf("exp: %d\n", exp);       
                    i++;
                }
            }
        }
        if (c == 0 && exp > 0){
            c = 1;
        }
        polys[ppi].c = multiplier*c;
        polys[ppi].exp = exp;
        ppi++;
    }
    p->len = real_length;
    return p;
}

void free_poly(poly_t* p) {
    free(p->polys);
    free(p);
}

poly_t* add(poly_t* p, poly_t*q) {
    poly_part_t* r_polys = malloc(sizeof(poly_part_t)*10);
    poly_t* r = malloc(sizeof(poly_t));
    r->len = p->len + q->len;
    r->polys = r_polys;
    int p_i = 0;
    int q_i = 0;
    int r_i = 0;
    while(p_i < p->len || q_i < q->len){
        if (p->polys[p_i].exp == q->polys[q_i].exp) {
            r->polys[r_i].c = p->polys[p_i].c + q->polys[q_i].c;
            r->polys[r_i].exp = p->polys[p_i].exp;
            p_i++;
            q_i++;
            r_i++;
        }
        else if (p->polys[p_i].exp > q->polys[q_i].exp)
        {
            r->polys[r_i].c = p->polys[p_i].c;
            r->polys[r_i].exp = p->polys[p_i].exp;
            p_i++;
            r_i++;
        }
        else if (p->polys[p_i].exp < q->polys[q_i].exp)
        {
            r->polys[r_i].c = q->polys[q_i].c;
            r->polys[r_i].exp = q->polys[q_i].exp;
            q_i++;
            r_i++;
        }
    }
    r->len = r_i;
    return r;
}

poly_t*	mul(poly_t* p, poly_t*q) {
    /*
    for i in p.len
        partsum = p->polys[i]
        for j in q.len
            partsum[j].c += q->polys[j];
            partsum[j].exp += q->polys[j].exp;


    add all partsums.
    free all partsums
    return r
    */
    poly_t partsums[p->len];
    for (int i = 0; i<p->len; i++){
        poly_part_t part_polys[q->len];
        for (int j = 0; j<q->len; j++) {
            partsums[i].polys[i]
        }
    }

    return add(p, q);
    // poly_part_t* polys = malloc(sizeof(poly_part_t)*10);
    // poly_t* r = malloc(sizeof(poly_t));
    // r->len = p->len + q->len;
    // r->polys = polys;
    // for (int i = 0; i < r->len; i++){
        
    // }
    // return p; 
}

void print_poly(poly_t* p) {
    for (int i = 0; i < p->len; ) {
        if (p->polys[i].c < 0) {
            printf("- ");
        }
        else if (p->polys[i].c > 1 && i > 0)
        {
            printf("+ ");
        }
        if (abs(p->polys[i].c) > 1 || p->polys[i].exp == 0) {
            printf("%d", abs(p->polys[i].c));
        }
        if (p->polys[i].exp == 1){
            printf("x");
        }
        else if (p->polys[i].exp > 1)
        {
            printf("x^%d", p->polys[i].exp);
        }
        i++;
        if (i < p->len) {
            printf(" ");
        }
        
    }
    printf("\n");
    // printf("po = %d, b = %d", p->polys, p->len);
}
