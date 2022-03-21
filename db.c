#define _CRT_SECURE_NO_WARNINGS
#define NONE -1
#define REQ_SIZE 20
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <stddef.h>

typedef enum { RED, BLACK } color_t;

typedef struct node_t {
    color_t color;
    int data;
    struct node_t* parent, * left, * right;
} node_t;

node_t* SLL_pool = NULL;
unsigned long SLL_cnt = 0;
unsigned long UsedMemoryForSLLs = 0;

node_t* allocSLL(void);
void freeSLL(node_t* ptr);
void freeSLL_pool(void);

typedef struct {
    node_t* root;
    node_t* nil;  // for sentinel
} rbtree;


rbtree* new_rbtree(void);
void delete_rbtree(rbtree*);
void free_node(rbtree* t, node_t* x);

void left_rotation(rbtree* tree, node_t* x);
void right_rotation(rbtree* tree, node_t* x);


node_t* rbtree_insert(rbtree*, int);
void rbtree_insert_fixup(rbtree* t, node_t* z);
node_t* rbtree_find(rbtree*, int);
int rbtree_erase(rbtree*, node_t*);
void rbtree_delete_fixup(rbtree* t, node_t* x);
void rbtree_transplant(rbtree* t, node_t* u, node_t* v);

void inorder(rbtree* t, node_t* cur) {
    if (cur == t->nil)
        return;
    inorder(t, cur->left);
    printf("%d\n", cur->data);
    inorder(t, cur->right);
}
int main(int argc, char* argv[]) {
    clock_t start_time = clock();
    FILE* fp, * result_fp;
    int data;
    char cmd;
    if (argc != 2)printf("usuage: dataname");
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("file open error\n");
        return 0;
    }
    result_fp = fopen("db_result.txt", "w");
    rbtree* rbtreeDB = new_rbtree();
    node_t* tmp = NULL;
    while (fscanf(fp, "%c %d\n", &cmd, &data) != EOF) {
        switch (cmd) {
        case 'i':
            rbtree_insert(rbtreeDB, data);
            break;
        case 'd':
            tmp = rbtree_find(rbtreeDB, data);
            if (tmp != NULL) {
                rbtree_erase(rbtreeDB, tmp);
            }
            else {
                fprintf(result_fp, "ignore\n");
            }
            break;
        case 's':
            tmp = rbtree_find(rbtreeDB, data);
            if (tmp != NULL) {
                fprintf(result_fp, "true\n");
            }
            else {
                fprintf(result_fp, "false\n");
            }
            break;
        }
    }
    fclose(fp);
    fclose(result_fp);
    printf("%lf\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);
    delete_rbtree(rbtreeDB);
    freeSLL_pool();
    return 0;
}


node_t* allocSLL(void) {
    node_t* ptr;
    if (SLL_pool == NULL) {
        ptr = (node_t*)malloc(sizeof(node_t));
        if (ptr == NULL) {
            printf("memory allocation error\n");
            exit(0);
        }
        UsedMemoryForSLLs += sizeof(node_t);
    }
    else {
        ptr = SLL_pool;
        SLL_pool = ptr->right;
    }
    ptr->right = NULL;
    ++SLL_cnt;
    return(ptr);
}
void freeSLL(node_t* ptr) {
    ptr->data = NONE;
    ptr->right = SLL_pool;
    SLL_pool = ptr;
    --SLL_cnt;
}
void freeSLL_pool(void) {
    node_t* ptr = SLL_pool;
    while (ptr != NULL) {
        SLL_pool = ptr->right;
        free(ptr);
        UsedMemoryForSLLs -= sizeof(node_t);
        ptr = SLL_pool;
    }
    if (SLL_cnt != 0) {
        printf("somethiing wrong\n");
    }
}


rbtree* new_rbtree(void) {

    rbtree* p = (rbtree*)calloc(1, sizeof(rbtree));
    //node_t* nilNode = (node_t*)calloc(1, sizeof(node_t));
    node_t* nilNode = allocSLL();
    nilNode->color = BLACK;
    p->nil = nilNode;
    p->root = nilNode;

    return p;
}

void right_rotation(rbtree* tree, node_t* target) {
    // TODO!
    // 1. target�� ���� child�� ������ ����Ʈ���� target�� ���� ����Ʈ���� �����.
    node_t* left_of_target;
    left_of_target = target->left;
    target->left = left_of_target->right;
    //(leftchild�� ������ ��尡 NIL�� �ƴ϶��,�θ� target���� ����)
    if (left_of_target->right != tree->nil) {
        left_of_target->right->parent = target;
    }
    // 2. leftchild�� �θ� ��带  (target) -> (target�� �θ���)�� ����
    left_of_target->parent = target->parent;
    //(���� target�� �θ� ��尡 nil�̶��,target�� root �̹Ƿ� Ʈ�� ����ü�� root�� left of target�� ����)
    if (target->parent == tree->nil)
        tree->root = left_of_target;
    // (�ƴ϶�� target�� �θ� target��ſ� left of target�� ����Ű���� �Ѵ�.)
    else if (target == target->parent->left)
        target->parent->left = left_of_target;
    else
        target->parent->right = left_of_target;
    // 3. target�� left of target�� right child�� �ǵ��� �Ѵ�.
    left_of_target->right = target;
    target->parent = left_of_target;
}

void left_rotation(rbtree* tree, node_t* target) {
    // TODO!
    //target�� right child�� ���� ����Ʈ���� target�� ������ ����Ʈ���� �����.
    node_t* right_of_target;
    right_of_target = target->right;
    target->right = right_of_target->left;
    //rigth child�� ���� ����Ʈ���� nil�� �ƴ϶�� ���� ����Ʈ���� parent�� target���� ����
    if (right_of_target->left != tree->nil)
        right_of_target->left->parent = target;
    //rightoftarget�� �θ� target�� �θ�� ����
    right_of_target->parent = target->parent;

    if (target->parent == tree->nil)
        tree->root = right_of_target;
    else if (target == target->parent->left)
        target->parent->left = right_of_target;
    else
        target->parent->right = right_of_target;

    right_of_target->left = target;
    target->parent = right_of_target;
}

void free_node(rbtree* t, node_t* x) {
    // postorder memory free
    if (x->left != t->nil)
        free_node(t, x->left);
    if (x->right != t->nil)
        free_node(t, x->right);
    //free(x);
    freeSLL(x);
    x = NULL;
}

void delete_rbtree(rbtree* t) {
    if (t->root != t->nil)
        free_node(t, t->root);
    //free(t->nil);
    freeSLL(t->nil);
    free(t);
}

void rbtree_insert_fixup(rbtree* t, node_t* z) {
    node_t* y;

    while (z->parent->color == RED) {
        // z�� �θ� ���θ��� ���� ���� Ʈ���� ���
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;

            // CASE 1 : ��� z�� ���� y�� ������ ���
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            // CASE 2 : z�� ���� y�� ����̸��� z�� ������ �ڽ��� ���
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotation(t, z);
                }
                // CASE 3 : z�� ���� y�� ����̸��� z�� ������ �ڽ��� ���
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotation(t, z->parent->parent);
            }
        }
        // z�� �θ� ���θ��� ���� ���� Ʈ���� ���
        else {
            y = z->parent->parent->left;

            // CASE 4 : ��� z�� ���� y�� ������ ���
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            // CASE 5 : z�� ���� y�� ����̸��� z�� ������ �ڽ��� ���
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotation(t, z);
                }
                // CASE 6 : z�� ���� y�� ����̸��� z�� ������ �ڽ��� ���
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotation(t, z->parent->parent);
            }
        }
    }
    t->root->color = BLACK;
}

node_t* rbtree_insert(rbtree* t, int data) {
    node_t* y = t->nil;
    node_t* x = t->root;
    //node_t* z = (node_t*)calloc(1, sizeof(node_t));
    node_t* z = allocSLL();
    z->data = data;

    while (x != t->nil) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == t->nil) {
        t->root = z;
    }
    else if (z->data < y->data) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    z->left = t->nil;
    z->right = t->nil;
    z->color = RED;

    rbtree_insert_fixup(t, z);

    return z;
}

node_t* rbtree_find(rbtree* tree, int data) {
    node_t* curnode = tree->root;
    while (curnode != tree->nil) {
        if (curnode->data == data)
            return curnode;
        if (curnode->data < data)
            curnode = curnode->right;
        else
            curnode = curnode->left;
    }
    return NULL;
}

void rbtree_transplant(rbtree* t, node_t* u, node_t* v) {
    if (u->parent == t->nil) {
        t->root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree* t, node_t* x) {
    while (x != t->root && x->color == BLACK) {
        // CASE 1 ~ 4 : LEFT CASE
        if (x == x->parent->left) {
            node_t* w = x->parent->right;

            // CASE 1 : x�� ���� w�� ������ ���
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotation(t, x->parent);
                w = x->parent->right;
            }

            // CASE 2 : x�� ���� w�� ����̰� w�� �� ������ ��� ����� ���
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }

            // CASE 3 : x�� ���� w�� ���, w�� ���� �ڽ��� ����, w�� ������ �ڽ��� ����� ���
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotation(t, w);
                    w = x->parent->right;
                }

                // CASE 4 : x�� ���� w�� ����̰� w�� ������ �ڽ��� ������ ���
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotation(t, x->parent);
                x = t->root;
            }
        }
        // CASE 5 ~ 8 : RIGHT CASE
        else {
            node_t* w = x->parent->left;

            // CASE 5 : x�� ���� w�� ������ ���
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotation(t, x->parent);
                w = x->parent->left;
            }

            // CASE 6 : x�� ���� w�� ����̰� w�� �� ������ ��� ����� ���
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }

            // CASE 7 : x�� ���� w�� ���, w�� ���� �ڽ��� ����, w�� ������ �ڽ��� ����� ���
            else
            {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotation(t, w);
                    w = x->parent->left;
                }

                // CASE 8 : x�� ���� w�� ����̰� w�� ������ �ڽ��� ������ ���
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotation(t, x->parent);
                x = t->root;
            }
        }
    }

    x->color = BLACK;
}

int rbtree_erase(rbtree* t, node_t* p) {
    // TODO: implement erase
    node_t* y;
    node_t* x;
    color_t yOriginalColor;

    y = p;
    yOriginalColor = y->color;

    if (p->left == t->nil) {
        x = p->right;
        rbtree_transplant(t, p, p->right);
    }
    else if (p->right == t->nil) {
        x = p->left;
        rbtree_transplant(t, p, p->left);
    }
    else {
        y = p->right;
        while (y->left != t->nil) {
            y = y->left;
        }
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == p) {
            x->parent = y;
        }
        else {
            rbtree_transplant(t, y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }

        rbtree_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }

    if (yOriginalColor == BLACK) {
        rbtree_delete_fixup(t, x);
    }

    //free(p);
    freeSLL(p);

    return 0;
}