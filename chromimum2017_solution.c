#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXN 50000
#define MAX_TREE_SIZE (4*(MAXN))
#define MOD 1000000007LL
#define TRANSFORM_SIZE 3

typedef struct {
    long long a, b, c, d, e, f;
} transform_t;

typedef struct { long long x; long long y; } point_t;

#define P(x, y) (((long long)(x) * (long long)(y)))

#define FIX(v) do { if (v < 0 || v > MOD) v = v % MOD + MOD; } while (0);

static void compose(transform_t *t1, transform_t *t2, transform_t *result) {
    long long a, b, c, d, e, f;
     a = P(t1->a, t2->a) + P(t1->b, t2->d); FIX(a);
     b = P(t1->a, t2->b) + P(t1->b, t2->e); FIX(b);
     c = P(t1->a, t2->c) + P(t1->b, t2->f) + t1->c; FIX(c);
     d = P(t1->d, t2->a) + P(t1->e, t2->d); FIX(d);
     e = P(t1->d, t2->b) + P(t1->e, t2->e); FIX(e);
     f = P(t1->d, t2->c) + P(t1->e, t2->f) + t1->f; FIX(f);
     result->a = a;
     result->b = b;
     result->c = c;
     result->d = d;
     result->e = e;
     result->f = f;
}

static long long apply(transform_t *t, point_t *p) {
    long long x, y, r;
    x = P(t->a, p->x) + P(t->b, p->y) + t->c;
    y = P(t->d, p->x) + P(t->e, p->y) + t->f;
    r = x + y; FIX(r);
    return r;
}

static void ident(transform_t *t) {
    memset(t, 0, sizeof(transform_t));
    t->a = t->e = 1L;
}

typedef struct {
    transform_t transform;
    transform_t rev_transform;
} node_t;

#define LEFT(node) (2 * (node) + 1)
#define RIGHT(node) (2 * (node) + 2)

static int tree_size;
static int aux_nodes;
static node_t tree[MAX_TREE_SIZE];

static void init_tree(int n) {
    int i;
    tree_size = 1;
    n--;
    while (n > 0) {
        tree_size = (tree_size << 1) + 1;
        n >>= 1;
    }
    aux_nodes = tree_size >> 1;
    for (i = 0; i < tree_size; i++) {
        ident(&tree[i].transform);
        ident(&tree[i].rev_transform);
    }
}

static void reverse_transforms(int target) {
    int node_idx = 0;
    transform_t rev_transform;
    int size = aux_nodes + 1;

    ident(&rev_transform);
    while (node_idx < aux_nodes) {
        size = size >> 1;
        compose(&tree[node_idx].rev_transform, &rev_transform, &rev_transform);
        if (target < size) {
            node_idx = LEFT(node_idx);
        } else {
            node_idx = RIGHT(node_idx);
            target -= size;
        }
    }
    memcpy(&tree[node_idx].transform, &rev_transform, sizeof(transform_t));
}

static void apply_transform_left_of(transform_t *t, transform_t *rev, int target) {
    int node_idx = 0;
    int size = aux_nodes + 1;
    transform_t transform, rev_transform;

    memcpy(&transform, t, sizeof(transform_t));
    memcpy(&rev_transform, rev, sizeof(transform_t));

    while (node_idx < tree_size) {
        if (target == size) {
            compose(&transform, &tree[node_idx].transform, &tree[node_idx].transform);
            compose(&tree[node_idx].rev_transform, &rev_transform, &tree[node_idx].rev_transform);
            return;
        }

        size = size >> 1;
        if (size == 0) { return; }
        compose(&tree[node_idx].rev_transform, &transform, &transform);
        compose(&transform, &tree[node_idx].transform, &transform);
        compose(&tree[node_idx].rev_transform, &rev_transform, &rev_transform);
        compose(&rev_transform, &tree[node_idx].transform, &rev_transform);

        if (target <= size) {
            node_idx = LEFT(node_idx);
        } else {
            compose(&transform, &tree[LEFT(node_idx)].transform, &tree[LEFT(node_idx)].transform);
            compose(&tree[LEFT(node_idx)].rev_transform, &rev_transform, &tree[LEFT(node_idx)].rev_transform);
            node_idx = RIGHT(node_idx);
            target -= size;
        }
    }
}

static void apply_transform_right_of(transform_t *t, transform_t *rev, int target) {
    int node_idx = 0;
    int size = aux_nodes + 1;
    transform_t transform, rev_transform;

    memcpy(&transform, t, sizeof(transform_t));
    memcpy(&rev_transform, rev, sizeof(transform_t));

    while (node_idx < tree_size) {
        if (target == 0) {
            compose(&transform, &tree[node_idx].transform, &tree[node_idx].transform);
            compose(&tree[node_idx].rev_transform, &rev_transform, &tree[node_idx].rev_transform);
            return;
        }

        size = size >> 1;
        if (size == 0) { return; }
        compose(&tree[node_idx].rev_transform, &transform, &transform);
        compose(&transform, &tree[node_idx].transform, &transform);
        compose(&tree[node_idx].rev_transform, &rev_transform, &rev_transform);
        compose(&rev_transform, &tree[node_idx].transform, &rev_transform);

        if (target < size) {
            compose(&transform, &tree[RIGHT(node_idx)].transform, &tree[RIGHT(node_idx)].transform);
            compose(&tree[RIGHT(node_idx)].rev_transform, &rev_transform, &tree[RIGHT(node_idx)].rev_transform);
            node_idx = LEFT(node_idx);
        } else {
            node_idx = RIGHT(node_idx);
            target -= size;
        }
    }
}

static point_t init_point = { .x=0, .y=0 };

static long long gather(int node_idx, int n, transform_t *transform) {
    long long r;

    r = 0;
    if (node_idx < aux_nodes) {
        compose(transform, &tree[node_idx].transform, transform);
        r = gather(LEFT(node_idx), n, transform);
        r = (r + gather(RIGHT(node_idx), n, transform)) % MOD;
        FIX(r);
        compose(transform, &tree[node_idx].rev_transform, transform);
    } else if (node_idx - aux_nodes < n) {
        compose(transform, &tree[node_idx].transform, &tree[node_idx].transform);
        r += apply(&tree[node_idx].transform, &init_point) + 1;
        FIX(r);
    }
    return r;
}

transform_t f1 = { .a = 1, .b = 1, .c = 1,
                   .d = 0, .e = 1, .f = 0};
transform_t rev_f1 = { .a=1, .b=MOD-1, .c=MOD-1,
                      .d=0,  .e=1,  .f=0};

transform_t f2 = {.a=1, .b=0, .c=0,
                  .d=1, .e=1, .f=1};
transform_t rev_f2 = {.a=1, .b=0, .c=0,
                      .d=MOD-1, .e=1, .f=MOD-1};

static int* sigh[MAXN];

int cmp(const void * a, const void * b) {
   return ( **(int**)a - **(int**)b );
}

int solution(int H[], int N) {
    transform_t transform;
    ident(&transform);
    init_tree(N);
    for (int i = 0; i < N; i++) {
        sigh[i] = &(H[i]);
    }
    qsort(sigh, N, sizeof(int*), cmp);
    for (int i = 0; i < N; i++) {
        int idx = sigh[i] - H;
        apply_transform_left_of(&f1, &rev_f1, idx);
        apply_transform_right_of(&f2, &rev_f2, idx);
        reverse_transforms(idx);
    }
    return (int)(gather(0, N, &transform) % MOD);
}
