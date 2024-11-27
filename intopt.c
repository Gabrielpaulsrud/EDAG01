#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct node_t
{
    int m;
    int n;
    int k;
    int h;
    double xh;
    double ak;
    double bk;
    double* min;
    double* max;
    double** a;
    double* b; 
    double* x;
    double* c;
    double z;
    node_t* next;
};



struct linked_nodes_t {
    node_t* head;
};

typedef struct node_t node_t;
typedef struct simplex_t simplex_t;
typedef struct linked_nodes_t linked_nodes_t;

void pivot(simplex_t* s, int row, int col);
double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);


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
        s->var = calloc(m+n+1, sizeof(int));
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

void prepare(simplex_t* s, int k)
{
    int m = s->m;
    int n = s->n;
    int i;
    // make room for xm+n at s.var[n] by moving s.var[n..n+m-1] one 
    // step to the right.
    for (i=m+n ;i>n; i--) {
        s->var[i] = s->var[i-1];
    }
    s->var[n] = m+n;
    // add xm+n to each constant
    n = n + 1;
    for (i=0; i<m; i++) {
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
    prepare(s, k);
    n = s->n;
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
    for (i=0; i<m+n; i++) {
        if (s->var[i] == m+n-1) {
            if (abs(s->x[i]) > epsilon) {
                free(s->x);
                free(s->c);
                return 0;
            }
            else {
                break;
            }
        }
    }
    if (i>=n) {
        for(j=k=0; k<n; k++) {
            if (fabs(s->a[i-n][k]) > fabs(s->a[i-n][j])) {
                j=k;
            }
        }
        pivot(s, i-n, j);
        i=j;
    }
    if (i<n-1) {
        k = s->var[i]; s->var[i] = s->var[n-1]; s->var[n-1] = k;
        for (k=0; k<m; k++) {
            w = s->a[k][n-1]; s->a[k][n-1] = s->a[k][i]; s->a[k][i] = w;
        }
    }
    else {
        //forget xn+m
    }
    free(s->c);
    s->c = c;
    s->y = y;
    for (k=n-1; k<n+m-1; k++) {
        s->var[k] = s->var[k+1];
    }
    n = s->n = s->n-1;
    double* t = calloc(n, sizeof(double));
    for (k=0; k<n; k++) {
        for (j=0; j<n; j++) {
            if (k == s->var[j]) {
                t[j] = t[j] + s->c[k];
                goto next_k;
            }
        }
        for (j=0; j<m; j++) {
            if (s->var[n+j] == k) {
                break;
            }
        }
        s->y = s->y + s->c[k] * s->b[j];
        for (i=0; i<n; i++) {
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }
        next_k:
            continue;
    }
    for (i=0; i<n; i++) {
        s->c[i] = t[i];
    }
    free(t);
    free(s->x);
    return 1;
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



double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h)
{
    simplex_t s;
    int i, row, col;
    if (!initial(&s, m, n, a, b, c, x, y, var)) {
        free(s.var);
        return NAN;
    }
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
    // free(s.x);
    // free(s.c);
    return s.y;
}

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y)
{
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

node_t* initial_node(int m, int n, double** a, double* b, double*c) {
    node_t* p = calloc(1, sizeof(node_t));
    p->a = make_matrix(m+1, n+1);
    p->b = calloc(m+1, sizeof(double));
    p->c = calloc(n+1, sizeof(double));
    p->x = calloc(n+1, sizeof(double));
    p->min = calloc(n, sizeof(double));
    p->max = calloc(n, sizeof(double));
    p->m = m;
    p->n = n;

    int i;
    for (i=0; i<=m; i++) {
        memcpy(p->a[i], a[i], (n+1)*sizeof(double)); //Todo, very unsure (n+1??)
    }

    memcpy(p->b, b, (m)*sizeof(double)); //Todo, Should it be +1?
    memcpy(p->c, c, (n+1)*sizeof(double));
    for (i=0; i<n; i++){
        p->min[i] = -INFINITY;
        p->max[i] = INFINITY;
    }
    return p;
}

void extend(node_t* p, int m, int n, double** a, double* b, double* c, int k, double ak, double bk) {
    node_t* q = calloc(1, sizeof(node_t));
    int i,j;
    q->k = k;
    q->ak = ak;
    q->bk = bk;
    if (ak > 0 && p->max[k] < INFINITY){
        q->m = p->m;
    }
    else if (ak < 0 && p->min[k] > 0) {
        q->m = p->m;
    }
    else {
        q->m = p->m + 1;
    }
    q->n = p->n;
    q->h = -1;
    q->a = make_matrix(q->m+1, q->n+1);
    q->b = calloc(q->m+1, sizeof(double));
    q->c = calloc(q->n+1, sizeof(double));
    q->x = calloc(q->n+1, sizeof(double));
    q->min = calloc(n, sizeof(double));
    q->max = calloc(n, sizeof(double));
    // Memcopy(dest, src, count)
    memcpy(q->min, p->min, n* sizeof(double)); //TODO, ensure this
    memcpy(q->max, p->max, n* sizeof(double));
    for(i=0; i<m; i++) {
        memcpy(q->a[m], a[m], (n+1)*sizeof(double)); //still unsure
    }
    memcpy(q->b, b, m*sizeof(double_t));
    memcpy(q->c, c, (n+1)*sizeof(double_t)); //n+1?
    //TODO copying
    if (ak > 0) {
        if (q->max[k] == INFINITY || bk < q->max[k]) {
            q->max[k] = bk;
        }
    }
    else if (q->min[k] == -INFINITY || -bk > q->min[k]) {
        q->min[k] = -bk;
    }
    for (i=m; j=0; j++) {
        if (q->min[j] > -INFINITY) {
            q->a[i][j] = -1;
            q->b[i] = -q->min[j];
            i++;
        }
        if (q->max[j] < INFINITY) {
            q->a[i][j] = 1;
            q->b[i] = q->max[j];
            i++;
        }
    }
    return q;
}

int is_integer(double* xp) {
    double x = *xp;
    double r = round(x);
    if (fabs(r-x)<epsilon) {
        *xp = r;
        return 1;
    }
    else {
        return 0;
    }
}

int integer(node_t* p) {
    int i;
    for (i = 0; i<p->n; i++) {
        if (!is_integer(&p->x[i])) {
            return 0;
        }
    }
    return 1;
}

int bound(node_t* p, int h, double* zp, int x){
    if (p->z > *zp) {
        *zp = p->z;
        memcpy(x, p->x, m*sizeof(double));
    }
}

int branch(node_t* q, double z) {
    double min, max;
    int h;
    if (q->z < z) {
        return 0;
    }
    for (h=0; h<q->n; h++) {
        if (!is_integer(&q->x[h])) {
            if (q->min[h] == -INFINITY) {
                min = 0;
            }
            else {
                min = q->min[h];
            }
            max = q->max[h];
            if (floor(q->x[h])<min || roof(q->x[h])>max) {
                continue;
            }
            q->h = h;
            q->xh = q->x[h];
            // free() something TODO
            return 1;
        }
    }
    return 0;
}

// void succ(node_t* p,)
void add_to_linked_list(linked_nodes_t* h, node_t* p) {
    p->next = h->head;
    h->head = p;
}

double intopt(int m, int n, double** a, double* b, double* c, double* x) {
    node_t* p = initial_node(m, n, a, b, c);
    linked_nodes_t* h;
    add_to_linked_list(h, p);
    double z = -INFINITY;
    p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0);
    if (integer(p) || !isfinite(p->z)) {
        z = p->z;
        if (integer(p)) {
            memcpy(x, p->x, (n+1)*sizeof(double));
        }
        free(p);
        return z;
    }
    branch(p, z);
    while (h->head != NULL) {
        /* code */
    }
    
}



int main(int argc, char** argv) {
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
    for (int i = 0; i<m; i++)
    {
        free(a[i]);
    }
    free(a);
    free(x);

    return 0;
}
