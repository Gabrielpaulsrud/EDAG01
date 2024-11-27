#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


double** make_matrix(int m, int n)
{
    double** a;
    int i, j;
    a = calloc(m, sizeof(double*));
    for (i = 0; i < m; i++)
    {
        a[i] = calloc(n, sizeof(double));
    }
    return a;
}

double** pop_make_matrix(int m, int n)
{
    double** a;
    int i, j;
    a = calloc(m, sizeof(double*));
    for (i = 0; i < m; i++)
    {
        a[i] = calloc(n, sizeof(double));
        for (j = 0; j < n; j++)
        {
            a[i][j] = i*10+j;
        }
        
    }
    return a;
}

void print_matrix(double **a, int m, int n){
    for (int i=0; i<m; i++)
        {
            for (int j=0; j < n; j++)
            {
                printf("%5.1lf", a[i][j]);
            }
            printf("\n");
        }
}


int main(int argc, char** argv)
{
    int m = 3;
    int n = 5;
    double** a = pop_make_matrix(m, n);
    print_matrix(a, m, n);
    printf("\n");
    
    double** c = make_matrix(m, n);
    memcpy(c[2], a[1], sizeof(double)*n);
    print_matrix(c, m, n);
}