#include <stdio.h>
#include <stdlib.h>
// define character size// define character size
#define CHAR_SIZE 37

// A Trie node
struct Trie
{
	int isLeaf;	// 1 when node is a leaf node
	int key;	//Leaf nodes have key
	struct Trie* character[CHAR_SIZE];
};

// Function that returns a new Trie node
struct Trie* getNewTrieNode()
{
	struct Trie* node = (struct Trie*)malloc(sizeof(struct Trie));
	node->isLeaf = 0;

	for (int i = 0; i < CHAR_SIZE; i++)
		node->character[i] = NULL;

	return node;
}


void trieInsert(struct Trie *head, char* str, int key)
{
	struct Trie* curr = head;
	while (*str)
	{
		if(*str-'a' >= 0){
			if (curr->character[*str - 'a'] == NULL)
				curr->character[*str - 'a'] = getNewTrieNode();
			curr = curr->character[*str - 'a'];
		}
		else if(*str-'0' >= 0){
			if (curr->character[(*str-'0')+26] == NULL)
				curr->character[(*str-'0')+26] = getNewTrieNode();
			curr = curr->character[(*str-'0')+26];
		}
		else if(*str-'.' == 0){
			if(curr->character[36] == NULL)
				curr->character[36] = getNewTrieNode();
			curr = curr->character[36];
		}
		str++;
	}
	curr->isLeaf = 1;
	curr->key = key;
}

int trieSearch(struct Trie* head, char* str)
{
	if (head == NULL)
		return 0;
	struct Trie* curr = head;
	while (*str)
	{
		if(*str-'a' >= 0){
			curr = curr->character[*str - 'a'];
		}
		else if(*str-'0' >= 0){
			curr = curr->character[(*str - '0')+26];
		}
		else if(*str-'.' == 0){
			curr = curr->character[36];
		}
		if(curr == NULL)
			return 0;
		str++;
	}
	return curr->isLeaf;
}

int trieGetKey(struct Trie* head, char* str)
{	
	if (head == NULL)
		return 0;
	struct Trie* curr = head;
	while (*str)
	{	
		if(*str-'a' >= 0){
			curr = curr->character[*str - 'a'];
		}
		else if(*str-'0' >= 0){
			curr = curr->character[(*str - '0')+26];
		}
		else if(*str-'.' == 0){
			curr = curr->character[36];
		}
		if (curr == NULL)
			return 0;
		str++;
	}
	return curr->key;
}

int haveChildren(struct Trie* curr)
{
	for (int i = 0; i < CHAR_SIZE; i++)
	if (curr->character[i])
		return 1;	// child found

	return 0;
}

int trieDeletion(struct Trie **curr, char* str)
{
	if (*curr == NULL)
		return 0;
	if (*str)
	{
		if (*curr != NULL && (*curr)->isLeaf == 0)
		{
			if(*str-'a' >= 0 &&(*curr)->character[*str-'a'] != NULL && trieDeletion(&((*curr)->character[*str-'a']), str+1)){
				if (!haveChildren(*curr))
				{
					free(*curr);
					(*curr) = NULL;
					return 1;
				}
				else {
					return 0;
				}
			}
			else if(*str-'0' >= 0 && (*curr)->character[(*str-'0')+26] != NULL && trieDeletion(&((*curr)->character[(*str-'0')+26]), str+1)){
				if (!haveChildren(*curr))
				{
					free(*curr);
					(*curr) = NULL;
					return 1;
				}
				else {
					return 0;
				}
			}
			else if(*str-'.' == 0 &&(*curr)->character[36] != NULL && trieDeletion(&((*curr)->character[36]), str+1)){
				if (!haveChildren(*curr))
				{
					free(*curr);
					(*curr) = NULL;
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	}
	if (*str == '\0' && (*curr)->isLeaf)
	{
		if (!haveChildren(*curr))
		{
			free(*curr); // delete current node
			(*curr) = NULL;
			return 1; // delete non-leaf parent nodes
		}
		else
		{
			(*curr)->isLeaf = 0;
			return 0;	   // don't delete its parent nodes
		}
	}

	return 0;
}
