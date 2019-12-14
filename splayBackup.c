#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#define LEFT (0)
#define RIGHT (1)
#define TREE_NUM_CHILDREN (2)
#define TREE_EMPTY (0)
#define URL_SIZE 25

struct tree {
    /* we'll make this an array so that we can make some operations symmetric */
    struct tree *child[TREE_NUM_CHILDREN];
    char data[URL_SIZE];
    int ascii;
    int key;
};

int asciiVal(char *url){
    int val;
    for(int x = 0; x < URL_SIZE; x++){
    	val += url[x];
    }
    return val;
}

void init_tree_types(struct tree **tree){
    fprintf(stderr, "initializing node\n");
    fprintf(stderr, "initializing: %d\n", (*tree));
    fprintf(stderr, "key: %d\n", (*tree)->key);
    //fprintf(stderr, "url: %s\n", tree->data);
    strcpy((*tree)->data, "data");
    fprintf(stderr, "initialize url insertion");
    fprintf(stderr, "url: %s\n", (*tree)->data);
    fprintf(stderr, "url initialized\n");
    (*tree)->key = 0;
    (*tree)->ascii = 0;
    fprintf(stderr, "node initialized\n");
}

/* free all elements of a tree, replacing it with TREE_EMPTY */
/*void 
treeDestroy(struct tree **root)
{
    //we want to avoid doing this recursively, because the tree might be deep
    //so we will repeatedly delete the root until the tree is empty
    while(*root) {
        treeDelete(root, (*root)->data);
    }
}
*/
/* rotate child in given direction to root */
void
treeRotate(struct tree **root, int direction)
{
    struct tree *x;
    struct tree *y;
    struct tree *b;

    //init_tree_types(x);
    //init_tree_types(y);
    //init_tree_types(b);

    /*
     *      y           x 
     *     / \         / \
     *    x   C  <=>  A   y
     *   / \             / \
     *  A   B           B   C
     */

    y = *root;                  assert(y);
    x = y->child[direction];    assert(x);
    b = x->child[!direction];

    /* do the rotation */
    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;
}

/* link operations for top-down splay */
/* this pastes a node in as !d-most node in subtree on side d */
static inline void
treeLink(struct tree ***hook, int d, struct tree *node)
{
    *hook[d] = node;
    /* strictly speaking we don't need to do this, but it allows printing the partial trees */
    node->child[!d] = 0;
    hook[d] = &node->child[!d];
}

/* splay last element on path to target to root */
static void
treeSplay(struct tree **root, int target)
{
    fprintf(stderr, "splay\n");
    struct tree *t;
    struct tree *child;
    struct tree *grandchild;
    struct tree *top[TREE_NUM_CHILDREN];   /* accumulator trees that will become subtrees of new root */
    struct tree **hook[TREE_NUM_CHILDREN]; /* where to link new elements into accumulator trees */
    int d;
    int dChild;        /* direction of child */                   
    int dGrandchild;   /* direction of grandchild */

    /* we don't need to keep following this pointer, we'll just fix it at the end */
    fprintf(stderr, "root: %d\n", *root);
    //fprintf(stderr, "ascii: %d\n", (*root)->ascii);
    t = *root;

    //fprintf(stderr, "%d\n", t->ascii);

    /* don't do anything to an empty tree */
    if(t == 0) { 
	    fprintf(stderr, "nothing");
	    return; }

    /* ok, tree is not empty, start chopping it up */
    for(d = 0; d < TREE_NUM_CHILDREN; d++) {
        top[d] = 0;
        hook[d] = &top[d];
    }

    /* keep going until we hit the key or we would hit a null pointer in the child */
    while(t->ascii != target && (child = t->child[dChild = t->ascii < target]) != 0) {
        /* child is not null */
        grandchild = child->child[dGrandchild = child->ascii < target];

#ifdef DEBUG_SPLAY
        treePrint(top[0]);
        puts("---");
        treePrint(t);
        puts("---");
        treePrint(top[1]);
        puts("===");
#endif

        if(grandchild == 0 || child->ascii == target) {
            /* zig case; paste root into opposite-side hook */
            treeLink(hook, !dChild, t);
            t = child;
            /* we can break because we know we will hit child == 0 next */
            break;
        } else if(dChild == dGrandchild) {
            /* zig-zig case */
            /* rotate and then hook up child */
            /* grandChild becomes new root */
            treeRotate(&t, dChild);
            treeLink(hook, !dChild, child);
            t = grandchild;
        } else {
            /* zig-zag case */
            /* root goes to !dChild, child goes to dChild, grandchild goes to root */
            treeLink(hook, !dChild, t);
            treeLink(hook, dChild, child);
            t = grandchild;
        }
    }

    /* now reassemble the tree */
    /* t's children go in hooks, top nodes become t's new children */
    for(d = 0; d < TREE_NUM_CHILDREN; d++) {
        *hook[d] = t->child[d];
        t->child[d] = top[d];
    }

    /* and put t back in *root */
    *root = t;
}

/* splay last element on path to target to root */
/*static void
treeSplay(struct tree **root, char *target)
{
    //printf("tree splay\n");
    //printf("%s\n", target);
    struct tree *t;
    fprintf(stderr, "splay initial url: %s\n", t->data);
    struct tree *child;
    struct tree *grandchild;
    struct tree *top[TREE_NUM_CHILDREN];   // accumulator trees that will become subtrees of new root 
    struct tree **hook[TREE_NUM_CHILDREN]; // where to link new elements into accumulator trees 
    fprintf(stderr, "initializing in splayi function\n");
    //init_tree_types(&child);
    fprintf(stderr, "child initialized in splay\n");
    //init_tree_types(child);
    //init_tree_types(grandchild);
    //fprintf(stderr, "trees initialized\n");
    //init_tree_types(*top);
    //init_tree_types(**hook);
    int d;
    int dChild;        // direction of child                   
    int dGrandchild;   // direction of grandchild 

    /// we don't need to keep following this pointer, we'll just fix it at the end 
    t = *root;

    // don't do anything to an empty tree 
    if(t == 0) { fprintf(stderr, "nothing\n"); 
	    return; }
    //fprintf(stderr,"something\n");
    // ok, tree is not empty, start chopping it up 
    for(d = 0; d < TREE_NUM_CHILDREN; d++) {
        top[d] = 0;
        hook[d] = &top[d];
    }
    //if(t->child[0] == 0 || t->child[1] == 0){
	    //return;
    //}
    fprintf(stderr, "splay t data: %s\n", t->data);
    //fprintf(stderr, "%d\n", t);
    //fprintf(stderr, "%d\n", t->child[0]);
    //fprintf(stderr, "%d\n", t->child[1]);
    //fprintf(stderr, "tree choppng\n");
    // keep going until we hit the key or we would hit a null pointer in the child 
    int comp = -(strcmp(t->data, target));
    if(comp == -1)
	    comp = 0;
    while(strcmp(t->data, target) != 0 && (child = t->child[dChild = comp]) != 0) {
        fprintf(stderr, "while loop running\n");
	// child is not null 
	if(child == NULL){
		fprintf(stderr, "null");
	}
	fprintf(stderr, "child: %d\n", child);
	fprintf(stderr, "url: %s\n", child->data);
	//fprintf(stderr, "grandchild: %d\n", child->child[0]);
	//fprintf(stderr, "direction: %d\n", strcmp(child->data, target));
        
    	comp = -(strcmp(child->data, target));
    	if(comp == -1)
	    	comp = 0;
	fprintf(stderr, "comp: %d\n", comp);
	grandchild = child->child[dGrandchild = comp];
	fprintf(stderr, "grandchild\n");
#ifdef DEBUG_SPLAY
        treePrint(top[0]);
        puts("---");
        treePrint(t);
        puts("---");
        treePrint(top[1]);
        puts("===");
#endif
	fprintf(stderr, "grandchild set");
	comp = strcmp(child->data, target);
	if(comp == -1)
		comp = 0;
        if(grandchild == 0 || comp == 0) {
            // zig case; paste root into opposite-side hook 
            treeLink(hook, !dChild, t);
            t = child;
            // we can break because we know we will hit child == 0 next 
            break;
        } else if(dChild == dGrandchild) {
            // zig-zig case 
            // rotate and then hook up child 
            // grandChild becomes new root 
            treeRotate(&t, dChild);
            treeLink(hook, !dChild, child);
            t = grandchild;
        } else {
            // zig-zag case 
            // root goes to !dChild, child goes to dChild, grandchild goes to root 
            treeLink(hook, !dChild, t);
            treeLink(hook, dChild, child);
            t = grandchild;
        }
    }

    // now reassemble the tree 
    // t's children go in hooks, top nodes become t's new children 
    for(d = 0; d < TREE_NUM_CHILDREN; d++) {
        *hook[d] = t->child[d];
        t->child[d] = top[d];
    }

    // and put t back in *root 
    *root = t;
}*/

/* return 1 if target is in tree, 0 otherwise */
int
treeContains(struct tree **root, char *target)
{
    if(*root == 0){
	    fprintf(stderr, "empty");
	    return 0;
    }
    //fprintf(stderr, "treeContains\n"); 
    fprintf(stderr, "before splay\n");
    int val = asciiVal(target);
    treeSplay(root, val);
    //fprintf(stderr, "after splay\n");
    int contains = *root != 0 && strcmp((*root)->data, target)==0;
    fprintf(stderr, "%d\n", contains);
    return contains;
}

//find the key of the inputted string
int
treeFind(struct tree **root, char *target){
    fprintf(stderr, "treefind\n");
    int val = asciiVal(target);
    treeSplay(root, val);
    if(treeContains(root, target) == 1){
	    fprintf(stderr, "found\n");
	    int key = (*root)->key;
	    return key;
    }
    else{
	    fprintf(stderr, "not found\n");
	    return 0;
    }
}


/* insert an element into a tree pointed to by root */
void
treeInsert(struct tree **root, char *newData, int newKey)
{
    //printf("insert");
    //printf("%s\n", newData);
    struct tree *e;
    struct tree *t;
    //init_tree_types(&e);
    //init_tree_types(t);
    int d;             /* which side of e to put old root on */
    int val;

    val = asciiVal(newData);

    fprintf(stderr, "insert splay\n");
    treeSplay(root, val);

    t = *root;

    /* skip if already present */
    if(t && t->key == newKey) { return; }

    /* otherwise split the tree */
    e = malloc(sizeof(*e));
    assert(e);
    
    //fprintf(stderr, "url: %s\n", newData);
    strcpy(e->data, newData);
    e->ascii = val;
    e->key = newKey;
    //fprintf(stderr, "%s, %d\n", e->data, e->key);

    if(t == 0) {
	//fprintf(stderr, "t=0\n");
        e->child[LEFT] = e->child[RIGHT] = 0;
    } else {
	//fprintf(stderr, "not t = 0\n");
        /* split tree and put e on top */
        /* we know t is closest to e, so we don't have to move anything else */
        d = strcmp((*root)->data, newData)>0;
        //fprintf(stderr, "d: %d\n", d);
	e->child[d] = t;
	//fprintf(stderr, "child[d]: %d\n", e->child[d]);
        e->child[!d] = t->child[!d];
        t->child[!d] = 0;
    }

    /* either way we stuff e in *root */
    //fprintf(stderr, "e: %d\n", e);
    //fprintf(stderr, "e[0] %d\n", e[0]);
    *root = e;
    //fprintf(stderr, "%d\n", *(&root)->child[0]);
    //printf("inserted");
    //treeSplay(root, newData);
}

/* delete target from the tree */
/* has no effect if target is not in tree */
void 
treeDelete(struct tree **root, char *target)
{
/*    struct tree *left;
    struct tree *right;

    treeSplay(root, target);

    if(*root && (*root)->key == target) {
        //save pointers to kids
        left = (*root)->child[LEFT];
        right = (*root)->child[RIGHT];

        //free the old root
        free(*root);

        //if left is empty, just return right
        if(left == 0) {
            *root = right;
        } else {
            //first splay max element in left to top
            treeSplay(&left, INT_MAX);

            //now paste in right subtree
            left->child[RIGHT] = right;

            //return left
            *root = left;
        }
    }*/
}

/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
static void
treePrintIndented(const struct tree *root, int depth)
{
    int i;

    if(root != 0) {
        treePrintIndented(root->child[LEFT], depth+1);

        for(i = 0; i < INDENTATION_LEVEL*depth; i++) {
            putchar(' ');
        }
        printf("%s: %d (%p)\n", root->data, root->key, (void *) root);

        treePrintIndented(root->child[RIGHT], depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
}


//#ifdef TEST_MAIN
/*int
main(int argc, char **argv)
{
    int i;
    const int n = 10;
    struct tree *root = TREE_EMPTY;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    for(i = 0; i < n; i++) {
        assert(!treeContains(&root, i));
        treeInsert(&root, i);
        assert(treeContains(&root, i));
        treePrint(root);
        puts("===");
    }

     now delete everything 
    for(i = 0; i < n; i++) {
        assert(treeContains(&root, i));
        treeDelete(&root, i);
        assert(!treeContains(&root, i));
        treePrint(root);
        puts("===");
    }

    treeDestroy(&root);

    return 0;
}*/
//#endif
