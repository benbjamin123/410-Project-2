#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
 
typedef struct Node
{
	    char *data;
	    int key;
	        struct Node*  left;
		    struct Node*  right;
		        int      height;
} Node;
 
 
void dispose(Node* t);
Node* avlFind( char *e, Node *t );
Node* find_min( Node *t );
Node* find_max( Node *t );
Node* avlInsert( char *data, int key, Node *t );
Node* avlDelete( char *data, Node *t );
void display_avl(Node* t);
char* avlGet( Node* n );
#endif // AVLTREE_H_INCLUDED
