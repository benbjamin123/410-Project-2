#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avltree.h"
/*
 *     remove all nodes of an AVL tree
 *     */
void dispose(Node* t)
{
	    if( t != NULL )
		        {
				        dispose( t->left );
					        dispose( t->right );
						        free( t );
							    }
}
 
/*
 *     find a specific node's key in the tree
 *     */
Node* avlFind(char *e, Node* t )
{
	    if( t == NULL )
		            return NULL;
	        if(strcmp(e, t->data) < 0)
			        return avlFind( e, t->left );
		    else if(strcmp(e, t->data) > 0)
			            return avlFind( e, t->right );
		        else
				        return t;
}
 
/*
 *     find minimum node's key
 *     */
Node* find_min( Node* t )
{
	    if( t == NULL )
		            return NULL;
	        else if( t->left == NULL )
			        return t;
		    else
			            return find_min( t->left );
}
 
/*
 *     find maximum node's key
 *     */
Node* find_max( Node* t )
{
	    if( t != NULL )
		            while( t->right != NULL )
				                t = t->right;
	     
	        return t;
}
 
/*
 *     get the height of a node
 *     */
static int height( Node* n )
{
	    if( n == NULL )
		            return -1;
	        else
			        return n->height;
}
 
/*
 *     get maximum value of two integers
 *     */
static int max( int l, int r)
{
	    return l > r ? l: r;
}
 
/*
 *     perform a rotation between a k2 node and its left child
 *      
 *          note: call single_rotate_with_left only if k2 node has a left child
 *          */
 
static Node* single_rotate_with_left( Node* k2 )
{
	    Node* k1 = NULL;
	     
	        k1 = k2->left;
		    k2->left = k1->right;
		        k1->right = k2;
			 
			    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
			        k1->height = max( height( k1->left ), k2->height ) + 1;
				    return k1; /* new root */
}
 
/*
 *     perform a rotation between a node (k1) and its right child
 *      
 *          note: call single_rotate_with_right only if
 *              the k1 node has a right child
 *              */
 
static Node* single_rotate_with_right( Node* k1 )
{
	   Node* k2;
	     
	        k2 = k1->right;
		    k1->right = k2->left;
		        k2->left = k1;
			 
			    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
			        k2->height = max( height( k2->right ), k1->height ) + 1;
				 
				    return k2;  /* New root */
}
 
/*
 *  
 *      perform the left-right double rotation,
 *       
 *           note: call double_rotate_with_left only if k3 node has
 *               a left child and k3's left child has a right child
 *               */
 
static Node* double_rotate_with_left( Node* k3 )
{
	    /* Rotate between k1 and k2 */
	    k3->left = single_rotate_with_right( k3->left );
	     
	        /* Rotate between K3 and k2 */
	        return single_rotate_with_left( k3 );
}
 
/*
 *     perform the right-left double rotation
 *      
 *         notes: call double_rotate_with_right only if k1 has a
 *            right child and k1's right child has a left child
 *            */
 
 
 
static Node* double_rotate_with_right( Node* k1 )
{
	    /* rotate between K3 and k2 */
	    k1->right = single_rotate_with_left( k1->right );
	     
	        /* rotate between k1 and k2 */
	        return single_rotate_with_right( k1 );
}
 
/*
 *     insert a new node into the tree
 *     */
Node* avlInsert(char *e, int key, Node* t )
{
	if( t == NULL )
	{
		//printf("insert into empty tree\n");
		/* Create and return a one-node tree */
	        t = (Node*)malloc(sizeof(Node));
	        if( t == NULL )
	        {
	            fprintf (stderr, "Out of memory!!! (insert)\n");
                    exit(1);
	        }
	        else
	        {
		    //printf("make node\n");
	            t->data = e;
		    t->key = key;
                    t->height = 0;
	            t->left = t->right = NULL;
                }
        }
        else if(strcmp(e, t->data)<0 )
	{
	    //printf("insert to left\n");
            t->left = avlInsert( e, key, t->left );
            if( height( t->left ) - height( t->right ) == 2 )
    	    if( strcmp(e, t->left->data)<0 )
                t = single_rotate_with_left( t );
            else
                t = double_rotate_with_left( t );
        }
 	else if( strcmp(e, t->data)>0 )
        {
		//printf("insert to right\n");
	        t->right = avlInsert( e, key, t->right );
	        if( height( t->right ) - height( t->left ) == 2 )
	            if( strcmp(e, t->right->data)>0 )
	                    t = single_rotate_with_right( t );
	            else
	                    t = double_rotate_with_right( t );
	}
        /* Else X is in the tree already; we'll do nothing */    
        t->height = max( height( t->left ), height( t->right ) ) + 1;
	//printf("%d\n", t);
	return t;
}
 
/*
 *     remove a node in the tree
 *     */
Node* avlDelete( char *e, Node* t )
{
	    printf( "Sorry; Delete is unimplemented; %s remains\n", e );
	        return t;
}
 
/*
 *     data data of a node
 *     */
char* avlGet(Node* n)
{
	    return n->data;
}

int avlGetKey(Node* n){
	return n->key;
}

/*
 *     Recursively display AVL tree or subtree
 *     */
void display_avl(Node* t)
{
	//printf("display avl\n");
	if (t == NULL){
		//printf("empty\n");
		            return;
	}
	//printf("not empty\n");
	        printf("%s",t->data);
		 
		    if(t->left != NULL)
			            printf("(L:%s)",t->left->data);
		        if(t->right != NULL)
				        printf("(R:%s)",t->right->data);
			    printf("\n");
			     
			        display_avl(t->left);
				    display_avl(t->right);
}
