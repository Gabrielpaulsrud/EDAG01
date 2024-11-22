#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double epsilon = 0.000001;

struct simplex_t
{
    int m;
    int n;
    int* var;
    double** a;
    double* b;
    double* c;
    double* x;
    double y;
};

typedef struct simplex_t simplex_t;

double* scan_vector(double* v, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        scanf("%lf", &v[i]);
    }
    return v;
}

double** make_matrix(int m, int n)
{
    double **   a;
    int i;
    a = calloc(m, sizeof(double*));
    for (i = 0; i < m; i++)
    {
        a[i] = calloc(n, sizeof(double));
    }
    return a;
}

void scan_matrix(double** m, int row, int col){
    for (int i=0; i<row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // printf("i = %d, j = %d\n", i, j);
            scanf("%lf", &m[i][j]);
        }
    }
}

void print_array(double* array, int len)
{
    for (int i=0; i<len; i++)
    {
        printf("%12.3lf", array[i]);
    }
    printf("\n");
}


void print_a_b(double** a, double* b, int m, int n)
{
    for (int i=0; i<m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%12.3lf", a[i][j]);
            }
            printf(" \u2264 %9.3lf\n", b[i]);
        }
}

void print_system(double* c, double** a, double* b, int m, int n){
    printf("%-12s", "max z =");
    print_array(c, n);
    print_a_b(a, b, m, n);
}

int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var){
    int i,k;
    s->m = m;
    s->n = n;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;
    s->var = var;
    if (s->var == NULL) {
        s->var = calloc(m+n+1, sizeof(double));
        for (i=0; i<m+n; i++) {
            s->var[i] = i;
        }
    }
    for (k=0, i=1; i<m; i++) {
        if (b[i] < b[k]) {
            k = i;
        }
    }
    return k;
}

int select_nonbasic(simplex_t* s){
    int i;
    for (i=0; i<s->n; i++) {
        if (s->c[i] > epsilon) {
            return i;
        }
    }
    return -1;
}

void pivot(simplex_t* s, int row, int col)
{
    double** a = s->a;
    double* b = s->b;
    double* c = s->c;
    int m = s->m;
    int n = s->n;
    int i,j,t;
    t = s->var[col];
    s->var[col] = s->var[n+row];
    s->var[n+row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];
    for (i=0; i < n; i++) {
        if (i != col) {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = -c[col]/a[row][col];
    for (i=0; i < m; i++) {
        if (i != row) {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }
    for (i = 0; i < m; i++) {
        if (i != row) {
            for (j=0; j<n; j++) {
                if (j != col) {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }
    for (i = 0; i < m; i++) {
        if (i != row) {
            a[i][col] = -a[i][col]/a[row][col];
        }
    }
    for (i=0; i<n; i++) {
        if (i!=col) {
            a[row][i] = a[row][i]/a[row][col];
        }
    }
    b[row] = b[row]/a[row][col];
    a[row][col] = 1/a[row][col];
}


void prepare(simplex_t* s, int k)
{
    int m = s->m;
    int n = s->n;
    int i;
    // make room for xm+n at s.var[n] by moving s.var[n..n+m-1] one 
    // step to the right.
    for (i=m+n;i>n;i=i-1)
    {
        s->var[i] = s->var[i-1];
    }
    s->var[n] = m+n;
    // add xm+n to each constant
    n = n + 1;
    for (i=0;i<m;i+=1)
    {
        s->a[i][n-1] = -1; //arrow?
    }
    s->x = calloc(m+n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n-1] = -1;
    s->n = n;
    pivot(s, k, n-1);
}

int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var)
{
    int i,j,k;
    double w;
    k = init(s, m, n, a, b, c, x, y, var);
    if (b[k] >= 0)
    {
        return 1; // feasible
    }
    printf("OHH NOO");
    prepare(s, k);
    return 0;
}

int glob;
double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h)
{
    simplex_t s;
    int i, row, col;
    if (!initial(&s, m, n, a, b, c, x, y, var)) {
        free(s.var);
        return NAN;
    }
    glob += 1;
    while((col = select_nonbasic(&s))>=0) {
        row = -1;
        for (i = 0; i < m; i++) {
            if (a[i][col] > epsilon && 
            (row < 0 || b[i]/a[i][col] < b[row]/a[row][col])) {
                row = i;
            }
        }
        if (row < 0) {
            free(s.var);
            return INFINITY;
        }
        pivot(&s, row, col);
        // printf("PIVOTED");
        // print_system(c, a, b, m, n);
    }
    if (h == 0) {
        for (i=0; i<n; i++) {
            if (s.var[i] < n) {
                x[s.var[i]] = 0;
            }
        }
        for (i=0; i<m; i++) {
            if (s.var[n+i] < n) {
                x[s.var[n+i]] = s.b[i];
            }
        }
        free(s.var);
    }
    else
    {
        for (i=0; i<n; i++) {
            x[i] = 0;
        }
        for (i=n; i < n+m; i++) {
            x[i] = s.b[i-n];
        }
    }
    free(s.x);
    // free(s.c);
    double ret = s.y;
    return ret;
}

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y)
{
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

int main(int argc, char** argv)
{
    int m, n;
    scanf("%d %d", &m, &n);
    double* c = calloc(n+1, sizeof(double));
    scan_vector(c, n);
    
    double** a = make_matrix(m, n+1);
    scan_matrix(a, m, n);

    double* b = calloc(m, sizeof(double));
    scan_vector(b, m);

    double* x = calloc(n+1, sizeof(double));
    double y = 0.0;

    // print stuff
    printf("%-12s", "max z =");
    print_array(c, n);
    print_a_b(a, b, m, n);

    
    y = simplex(m, n, a, b, c, x, y);
    printf("result. z = %f\n", y);
    
    print_system(c, a, b, m, n);

    free(c);
    free(b);
    glob +=1;
    for (int i = 0; i<m; i++)
    {
        free(a[i]);
    }
    free(a);

    return 0;
}
