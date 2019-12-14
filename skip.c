/* Skip Lists: A Probabilistic Alternative to Balanced Trees */
 
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
 
#define SKIPLIST_MAX_LEVEL 6
 
typedef struct snode {
    int key;
    char url[25];
    struct snode **forward;
} snode;
 
typedef struct skiplist {
    int level;
    int size;
    struct snode *header;
} skiplist;
 
skiplist *skiplist_init(skiplist *list) {
    int i;
    snode *header = (snode *) malloc(sizeof(struct snode));
    list->header = header;
    header->key = INT_MAX;
    strcpy(header->url, "null");
    header->forward = (snode **) malloc(
            sizeof(snode*) * (SKIPLIST_MAX_LEVEL + 1));
    for (i = 0; i <= SKIPLIST_MAX_LEVEL; i++) {
        header->forward[i] = list->header;
    }
 
    list->level = 1;
    list->size = 0;
 
    return list;
}
 
static int rand_level() {
    int level = 1;
    while (rand() < RAND_MAX / 2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}
 
int skiplist_insert(skiplist *list, int key, char* url) {
    snode *update[SKIPLIST_MAX_LEVEL + 1];
    snode *x = list->header;
    int i, level;
    for (i = list->level; i >= 1; i--) {
        while (strcmp(x->forward[i]->url, url) == -1)
            x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[1];
 
    if (strcmp(url, x->url) == 0) {
        x->key = key;
        return 0;
    } else {
        level = rand_level();
        if (level > list->level) {
            for (i = list->level + 1; i <= level; i++) {
                update[i] = list->header;
            }
            list->level = level;
        }
 
        x = (snode *) malloc(sizeof(snode));
        x->key = key;
        strcpy(x->url, url);
        x->forward = (snode **) malloc(sizeof(snode*) * (level + 1));
        for (i = 1; i <= level; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
    return 0;
}
 
snode *skiplist_search(skiplist *list, char* url) {
    snode *x = list->header;
    int i;
    for (i = list->level; i >= 1; i--) {
	fprintf(stderr, "%s, %s\n", x->forward[i]->url, url);
        while (strcmp(x->forward[i]->url, url)==-1)
            x = x->forward[i];
    }
    if (strcmp(x->forward[1]->url, url)==0) {
	fprintf(stderr, "found in skip list\n");
        return x->forward[1];
    } else {
        return NULL;
    }
    return NULL;
}
 
static void skiplist_node_free(snode *x) {
    if (x) {
        free(x->forward);
        free(x);
    }
}

static void skiplist_free(skiplist *list, snode *x){
	//fprintf(stderr, "recursive free");
	if(x->forward[1]!=list->header){
		skiplist_free(list, x->forward[1]);
	}
	else{
		fprintf(stderr, "else");
		skiplist_node_free(x);
	}
}
/*
static void skiplist_free_list(skiplist *list){
    snode *x = list->header;
    skiplist_free(list, x);
}*/
 
int skiplist_delete(skiplist *list, char* url) {
    int i;
    snode *update[SKIPLIST_MAX_LEVEL + 1];
    snode *x = list->header;
    for (i = list->level; i >= 1; i--) {
        while (strcmp(x->forward[i]->url, url)==-1)
            x = x->forward[i];
        update[i] = x;
    }
 
    x = x->forward[1];
    if (strcmp(x->url, url)==0) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->forward[i] != x)
                break;
            update[i]->forward[1] = x->forward[i];
        }
        skiplist_node_free(x);
 
        while (list->level > 1 && list->header->forward[list->level]
                == list->header)
            list->level--;
        return 0;
    }
    return 1;
}
 
static void skiplist_dump(skiplist *list) {
    snode *x = list->header;
    while (x && x->forward[1] != list->header) {
        printf("%s[%d]->", x->forward[1]->url, x->forward[1]->key);
        x = x->forward[1];
    }
    printf("NIL\n");
}


static void skiplist_free_list(skiplist *list, int size){
	snode *x = list->header;
	for(int a = size-1; a > 0; a--){
		//fprintf(stderr, "%d\n", a);
		//skiplist_dump(list);
		skiplist_node_free(x->forward[a]);
	}
	//fprintf(stderr, "for loop over");
	skiplist_node_free(x);
	free(x);
}
 /*
int main() {
    int arr[] = { 3, 6, 9, 2, 11, 1, 4 }, i;
    skiplist list;
    skiplist_init(&list);
 
    printf("Insert:--------------------\n");
    for (i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        skiplist_insert(&list, arr[i], arr[i]);
    }
    skiplist_dump(&list);
 
    printf("Search:--------------------\n");
    int keys[] = { 3, 4, 7, 10, 111 };
 
    for (i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        snode *x = skiplist_search(&list, keys[i]);
        if (x) {
            printf("key = %d, value = %d\n", keys[i], x->value);
        } else {
            printf("key = %d, not fuound\n", keys[i]);
        }
    }
 
    printf("Search:--------------------\n");
    skiplist_delete(&list, 3);
    skiplist_delete(&list, 9);
    skiplist_dump(&list);
 
    return 0;
}*/
