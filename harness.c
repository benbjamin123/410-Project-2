#include <stdlib.h>
#include "rbtree.c"
#include "splay.c"
#include "trie.c"
#include "avl.c"
#include "hash.c"
#include "skip.c"
#include <time.h>

#define MIN_NODE 0
#define MAX_NODE 1000
#define TEST_1 10000
#define TEST_2 50000
#define TEST_REST 100000
#define TEST_END 1000000
#define TEST_INC 100000
#define NUM_RUNS 5

int findIndex(int *array, size_t size, int target)
{
    int i=0;
    while((i<size) && (array[i] != target)) i++;

    return (i<size) ? (i) : (-1);
}

int main(){
	//The naming conventions of the implementations I found are confusing. The red-black tree declares a root node and the
	//splay tree declares a tree struct.`
	//printf("assign variables\n");
	struct node *rb = NULL;
	struct tree *splay = NULL;
	init_tree_types(&splay);
	//treeInsert(&splay, "garbage", 0);
	//fprintf(stderr, "splay: %d\n", splay);
	struct Trie* trie = getNewTrieNode();
	struct Node *avl = NULL;
	//skiplist skip;
	//skiplist_init(&skip);
	clock_t start, end;
	double total_time, avg_time;
	FILE *fp, *keys, *randInput = fopen("./random.txt", "r"), *repeatInput=fopen("./repeats.txt", "r"), *altInput=fopen("./alternate.txt", "r");
	FILE** fileArr = malloc(3*sizeof(FILE*));
	fileArr[0] = randInput;
	fileArr[1] = repeatInput;
	fileArr[2] = altInput;
	FILE *times = fopen("./results2.txt", "w");
	int a, b, c, num=0, count, key, temp;
	//struct DataItem* item;
	char urlArray[1000][50];
	int keyArray[1000];
	char urlLine[150], keyLine[150];
	char *url, *url_line_split, *key_line_split, *urlBuff = urlLine, *keyBuff = keyLine;
	int runSets[] = {10000, 50000, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000};
	size_t bufSize = 150;

	//printf("open files\n");
   	fp = fopen("sites.txt", "r");
   	keys = fopen("keys.txt", "r");
	
	//item->key = -1;
	//item->data = "";
	
	printf("start making map\n");
	while (num < 1000){
		getline(&urlBuff, &bufSize, fp);
		getline(&keyBuff, &bufSize, keys);
		url_line_split = strtok(urlLine, "\t");
		count = 0;
		//printf("first while\n");
		while(url_line_split != NULL){
			//printf("%s\n", url_line_split);
			if(count == 1){
				url = url_line_split;
	        		//printf("url: %s\n", url);
			}
        		url_line_split = strtok (NULL, "\t");
        		count++;
		}
		//printf("url: %s\n", url);
		key_line_split = strtok(keyLine, "\t");
		count = 0;
		//printf("second while\n");
		while(key_line_split != NULL){
			//printf("%s\n", key_line_split);
			if(count == 1){
				key = atoi(key_line_split);
	        		//printf("Key: %d\n", key);
			}
        		key_line_split = strtok (NULL, "\t");
        		count++;
			//printf("%d\n", key);
		}
		//printf("(%d, %s)\n", key, url);
		//insert(key, url);
		//printf("search: (%d, %s)\n", key, search(key)->data);
		strcpy(urlArray[num], url);
		//printf("%s\n", urlArray[num]);
		keyArray[num] = key;
		num++;
		//display();
		printf("Fill loop %d\n", num);
	}
	printf("arrays filled\n");
	//key = getIdx(66)->key;
	//printf("find: (%d, %s)\n", key, search(key)->data);
	//display();
	//printf("find: (%d, %s)\n", search(key)->key, search(key)->data);
	fclose(fp);
	fclose(keys);
	free(url_line_split);
	free(key_line_split);
	//free(urlBuff);
	//free(keyBuff);

	/*printf("start filling arrays");
	for(int i = 0; i < 1000; i++){
		//printf("%s, %d\n", urlArray[i], keyArray[i]);
		//item = getIdx(i);
		key = keyArray[i];
		//url = search(key)->data;
		url = urlArray[i];
		//printf("%d, %s)\n", key, url);
		//treeInsert(&splay, url, key);
		//rbInsert(&rb, url, key);
		//avlInsert(url, key, avl);
	}
	printf("done filling arrays");*/

	int runSetsSize = (int)(sizeof(runSets)/sizeof(int));
	int runSet, idx, found, size;
	FILE* f;
	clock_t splayStart, splayEnd, rbStart, rbEnd, avlStart, avlEnd, trieStart, trieEnd;
	double splayTotal, splayAvg, rbTotal, rbAvg, avlTotal, avlAvg, trieTotal, trieAvg;
	struct node *rbNode;
	struct Node *avlNode;
	//struct snode *skipNode;
	struct Trie* trieNode;
	struct tree* splayNode;

	printf("start running\n");	
	//iterate through files
	for(int j = 0; j < 3; j++){
		//Iterate through run set value
		f = fileArr[j];
		fprintf(times, "Set %d\n", j);
		fprintf(times, " \tSplay\tRB\tAVL\tTrie\n");
		for(int k = 0; k < runSetsSize; k++){
			runSet = runSets[k];
			//run for NUM_RUNS number of times
			for(int a = 0; a < NUM_RUNS; a++){
				//run fpr runSet number of times
				for(int l = 0; l < runSet; l++){
					//find elements
					fscanf(f, "%d", &num);
					key = num;
					idx = findIndex(keyArray, 1000, key);
					url = urlArray[idx];
					//printf("%d, %s\n", key, url);		
					//insert elements into tree
					//fprintf(stderr, "start test\n");
					//fprintf(stderr, "%d\n", splay);
					/*skipStart = clock();
					//fprintf(stderr, "start clock");
					skipNode = skiplist_search(&skip, url);
					skipEnd = clock();
					skipTotal += ((double)(skipEnd-skipStart))/CLOCKS_PER_SEC;
					if(skipNode == NULL){
						fprintf(stderr, "skip list insert from harness\n");
						skipStart = clock();
						skiplist_insert(&skip, key, url);
						skipEnd = clock();
						skipTotal += ((double)(skipEnd-skipStart))/CLOCKS_PER_SEC;
						size++;
					}
					else{
						fprintf(stderr, "skip list find from harness\n");
						skipStart = clock();
						skipNode->key;
						skipEnd = clock();
						skipTotal += ((double)(skipEnd-skipStart))/CLOCKS_PER_SEC;
					}
					fprintf(stderr, "%d\n", size);
						//fprintf(stderr, "skip list node inserted");
					//skiplist_dump(&skip);*/
					
					splayStart = clock();;	
					splayNode = treeGetNode(&splay, url);
					splayEnd = clock();
					splayTotal += ((double)(splayEnd-splayStart))/CLOCKS_PER_SEC;
					if(splayNode == NULL){
						//fprintf(stderr, "harness tree contains\n");
						splayStart = clock();
						treeInsert(&splay, key, url);
						splayEnd = clock();	
						splayTotal += ((double)(splayEnd-splayStart))/CLOCKS_PER_SEC;
					}
					else{
						//fprintf(stderr, "harness tree insertion\n");
						splayStart = clock();
						splayNode->key;
						//fprintf(stderr, "finished splay insert in harness\n");
						splayEnd = clock();
						splayTotal += ((double)(splayEnd-splayStart))/CLOCKS_PER_SEC;
					}
					rbStart = clock();
					rbNode = rbSearch(rb, url);
					rbEnd = clock();
					rbTotal += ((double)(rbEnd-rbStart))/CLOCKS_PER_SEC;
					if(rbNode == NULL){
						rbStart = clock();
						rbInsert(&rb, url, key);
						rbEnd = clock();
						rbTotal += ((double)(rbEnd-rbStart))/CLOCKS_PER_SEC;
					}
					else{
						rbStart = clock();
						rbNode->key;
						rbEnd = clock();
						rbTotal += ((double)(rbEnd-rbStart))/CLOCKS_PER_SEC;
					}
					
					avlStart = clock();
					avlNode = avlFind(url, avl);
					avlEnd = clock();
					avlTotal += ((double)(avlEnd-avlStart))/CLOCKS_PER_SEC;
					if(avlNode==NULL){
						avlStart = clock();
						avl = avlInsert(url, key, avl);
						avlEnd = clock();
						avlTotal += ((double)(avlEnd-avlStart))/CLOCKS_PER_SEC;
					}
					else{
						avlStart = clock();
						avlNode->key;
						avlEnd = clock();
						avlTotal += ((double)(avlEnd-avlStart))/CLOCKS_PER_SEC;
					}
					
					trieStart = clock();
					found = trieSearch(trie, url);
					trieEnd = clock();
					trieTotal += ((double)(trieEnd-trieStart))/CLOCKS_PER_SEC;
					if(found == 1){
						trieStart = clock();
						trieGetKey(trie, url);
						trieEnd = clock();
						trieTotal += ((double)(trieEnd-trieStart))/CLOCKS_PER_SEC;
					}
					else{
						trieStart = clock();
						trieInsert(trie, url, key);
						trieEnd = clock();
						trieTotal += ((double)(trieEnd-trieStart))/CLOCKS_PER_SEC;
					}
				}
				fprintf(stderr, "finished set");
				rewind(f);
				//treePrint(splay);
				//inorder(rb);
				//display_avl(avl);
				treeDestroy(&splay);
				//skiplist_free_list(&skip, size);
				//fprintf(stderr, "freed skip list");
				//skiplist_init(&skip);
				rb = NULL;
				avl = NULL;
				free(trie);
				struct Trie* trie = getNewTrieNode();
				printf("finished %d run set\n", runSet);
				size = 0;
			}
			splayAvg = splayTotal/NUM_RUNS;
			rbAvg = rbTotal/NUM_RUNS;
			avlAvg = avlTotal/NUM_RUNS;
			trieAvg = trieTotal/NUM_RUNS;
			fprintf(times, "%d\t%f\t%f\t%f\t%f\n", runSet, splayAvg, rbAvg, avlAvg, trieAvg);
			/*fprintf(times, "Splay tree: \t%f\n", splayAvg);
			fprintf(times, "RB tree: \t%f\n", rbAvg);
			fprintf(times, "AVL tree: \t%f\n", avlAvg);
			fprintf(times, "Trie: \t%f\n", trieAvg);*/
			splayTotal = 0;
			rbTotal = 0;
			avlTotal = 0;
			trieTotal = 0;
		}
		printf("finished file #%d\n", j);
	}
	//treePrint(splay);
	/*
	for(a = 0; a < NUM_RUNS; a++){
		start = clock();
		for(b = 0; b < TEST_1; b++){
			fscanf(input, "%d", &num);
			treeInsert(&splay, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Splay Random Input %d times\n", TEST_1);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = 0; a < NUM_RUNS; a++){
		treeDestroy(&splay);
		start = clock();
		for(b = 0; b < TEST_2; b++){
			fscanf(input, "%d", &num);
			treeInsert(&splay, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Splay Random Input %d times\n", TEST_2);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = TEST_REST; a <= TEST_END; a += TEST_INC){
		for(b = 0; b < NUM_RUNS; b++){
			treeDestroy(&splay);
			start = clock();
			for(c = 0; c < a; c++){
				fscanf(input, "%d", &num);
				treeInsert(&splay, num);
			}
			end = clock();
			total_time += ((double)(end-start))/CLOCKS_PER_SEC;
		}
		avg_time = total_time/NUM_RUNS;
		fprintf(times, "Splay Random Input %d times\n", a);
		fprintf(times, "%f\n\n", avg_time);
		total_time = 0;
	}

	for(a = 0; a < NUM_RUNS; a++){
		treeDestroy(&splay);
		num = -1;
		start = clock();
		for(b = 0; b < TEST_1; b++){
			if(num >= 1001){
				num =0;
			}
			else{
				num++;
			}
			treeInsert(&splay, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Splay Ascending Input %d times\n", TEST_1);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = 0; a < NUM_RUNS; a++){
		treeDestroy(&splay);
		num = -1;
		start = clock();
		for(b = 0; b < TEST_2; b++){
			if(num >= 1001){
				num =0;
			}
			else{
				num++;
			}
			treeInsert(&splay, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Splay Ascending Input %d times\n", TEST_2);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = TEST_REST; a <= TEST_END; a += TEST_INC){
		for(b = 0; b < NUM_RUNS; b++){
			treeDestroy(&splay);
			num = -1;
			start = clock();
			for(c = 0; c < a; c++){
				if(num >= 1001){
					num =0;
				}
				else{
					num++;
				}
				treeInsert(&splay, num);
			}
			end = clock();
			total_time += ((double)(end-start))/CLOCKS_PER_SEC;
		}
		avg_time = total_time/NUM_RUNS;
		fprintf(times, "Splay Ascending Input %d times\n", a);
		fprintf(times, "%f\n\n", avg_time);
		total_time = 0;
	}
	
	for(a = 0; a < NUM_RUNS; a++){
		treeDestroy(&splay);
		num = 1001;
		start = clock();
		for(b = 0; b < TEST_1; b++){
			if(num <= -1){
				num = 1000;
			}
			else{
				num--;
			}
			treeInsert(&splay, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Splay Descending Input %d times\n", TEST_1);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = 0; a < NUM_RUNS; a++){
		treeDestroy(&splay);
		num = 1001;
		start = clock();
		for(b = 0; b < TEST_2; b++){
			if(num <= -1){
				num = 1000;
			}
			else{
				num--;
			}
			treeInsert(&splay, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Splay Descending Input %d times\n", TEST_2);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;
	
	for(a = TEST_REST; a <= TEST_END; a += TEST_INC){
		for(b = 0; b < NUM_RUNS; b++){
			treeDestroy(&splay);
			num = 1001;
			start = clock();
			for(c = 0; c < a; c++){
				if(num <= -1){
					num = 1000;
				}
				else{
					num--;
				}
				treeInsert(&splay, num);
			}
			end = clock();
			total_time += ((double)(end-start))/CLOCKS_PER_SEC;
		}
		avg_time = total_time/NUM_RUNS;
		fprintf(times, "Splay Descending Input %d times\n", a);
		fprintf(times, "%f\n\n", avg_time);
		total_time = 0;
	}

	
	for(a = 0; a < NUM_RUNS; a++){
		rb = NULL;
		start = clock();
		for(b = 0; b < TEST_1; b++){
			fscanf(input, "%d", &num);
			insert(&rb, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Red Black Random Input %d times\n", TEST_1);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = 0; a < NUM_RUNS; a++){
		rb = NULL;
		start = clock();
		for(b = 0; b < TEST_2; b++){
			fscanf(input, "%d", &num);
			insert(&rb, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Red Black Random Input %d times\n", TEST_2);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = TEST_REST; a <= TEST_END; a += TEST_INC){
		for(b = 0; b < NUM_RUNS; b++){
			rb = NULL;
			start = clock();
			for(c = 0; c < a; c++){
				fscanf(input, "%d", &num);
				insert(&rb, num);
			}
			end = clock();
			total_time += ((double)(end-start))/CLOCKS_PER_SEC;
		}
		avg_time = total_time/NUM_RUNS;
		fprintf(times, "Red Black Random Input %d times\n", a);
		fprintf(times, "%f\n\n", avg_time);
		total_time = 0;
	}

	for(a = 0; a < NUM_RUNS; a++){
		rb = NULL;
		num = -1;
		start = clock();
		for(b = 0; b < TEST_1; b++){
			if(num >= 1001){
				num =0;
			}
			else{
				num++;
			}
			insert(&rb, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Red Black Ascending Input %d times\n", TEST_1);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = 0; a < NUM_RUNS; a++){
		rb = NULL;
		num = -1;
		start = clock();
		for(b = 0; b < TEST_2; b++){
			if(num >= 1001){
				num =0;
			}
			else{
				num++;
			}
			insert(&rb, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Red Black Ascending Input %d times\n", TEST_2);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = TEST_REST; a <= TEST_END; a += TEST_INC){
		for(b = 0; b < NUM_RUNS; b++){
			rb = NULL;
			num = -1;
			start = clock();
			for(c = 0; c < a; c++){
				if(num >= 1001){
					num =0;
			}
				else{
					num++;
				}
				insert(&rb, num);
			}
			end = clock();
			total_time += ((double)(end-start))/CLOCKS_PER_SEC;
		}
		avg_time = total_time/NUM_RUNS;
		fprintf(times, "Red Black Ascending Input %d times\n", a);
		fprintf(times, "%f\n\n", avg_time);
		total_time = 0;
	}
	
	for(a = 0; a < NUM_RUNS; a++){
		rb = NULL;
		num = 1001;
		start = clock();
		for(b = 0; b < TEST_1; b++){
			if(num <= -1){
				num = 1000;
			}
			else{
				num--;
			}
			insert(&rb, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Red Black Descending Input %d times\n", TEST_1);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = 0; a < NUM_RUNS; a++){
		rb = NULL;
		num = 1001;
		start = clock();
		for(b = 0; b < TEST_2; b++){
			if(num <= -1){
				num = 1000;
			}
			else{
				num--;
			}
			insert(&rb, num);
		}
		end = clock();
		total_time += ((double)(end-start))/CLOCKS_PER_SEC;
	}
	avg_time = total_time/NUM_RUNS;
	fprintf(times, "Red Black Descending Input %d times\n", TEST_2);
	fprintf(times, "%f\n\n", avg_time);
	total_time = 0;

	for(a = TEST_REST; a <= TEST_END; a += TEST_INC){
		for(b = 0; b < NUM_RUNS; b++){
			rb = NULL;
			num = 1001;
			start = clock();
			for(c = 0; c < a; c++){
				if(num <= -1){
					num = 1000;
				}
				else{
					num--;
				}
				insert(&rb, num);
			}
			end = clock();
			total_time += ((double)(end-start))/CLOCKS_PER_SEC;
		}
		avg_time = total_time/NUM_RUNS;
		fprintf(times, "Red Black Descending Input %d times\n", a);
		fprintf(times, "%f\n\n", avg_time);
		total_time = 0;
	}
*/	
	
	fclose(randInput);
	fclose(repeatInput);
	fclose(altInput);
	//fclose(ascinput);
	//fclose(descinput);
	fclose(times);
	
	//free(url);
	//free(key);
	free(fileArr);
	free(rb);
        free(splay);	
	//free(trie);
	free(avl);
	free(avlNode);
	free(rbNode);
	//free(skipNode);
	free(trieNode);
	free(splayNode);
	return 0;
}
