#define _GNU_SOURCE
#define HASHTABLE_MAX_LEN 1000
#include "cachelab.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int hits = 0, misses = 0, evictions = 0;

/* link struct */
struct Node {
    int element;
    struct Node* next;
    struct Node* pre;
};
struct Node* getNewLinkNode() {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if(newNode == NULL) {
        printf("malloc crash\n"); return NULL;
    }
    newNode->next = NULL; newNode->element = 0; newNode->pre = NULL;
    return newNode;
}
void insertFront(struct Node* head, int ele) {
    struct Node* next = head->next;
    struct Node* newNode = getNewLinkNode();
    newNode->element = ele;
    newNode->next = next;
    newNode->pre = head;
    head->next = newNode;
    if(next == NULL) return;
    next->pre = newNode;
}
void deleteNode(struct Node* address) {
    if(address == NULL) return;
    struct Node* front = address->pre;
    if(front == NULL) return;
    struct Node* back = address->next;
    front->next = back;
    if(back != NULL) back->pre = front;
    free(address);
}
// testTool : print all ele;
void printLink(struct Node* head) {
    while(head->next != NULL) {
        head = head->next;
        printf("%d ", head->element);
    }
    printf("\n");
}

// HashTable push front function
void hitAndPushFront(struct Node* head, struct Node** HashTable, int id) {
    struct Node* node = HashTable[id];
    int ele = node->element;
    deleteNode(node);
    insertFront(head, ele);
    HashTable[id] = head->next;
}


/* my struct */
struct CacheLine {  /* line struct */
    int flag;
    unsigned long long tag;
    char* words;
    int id;
};
struct CacheSet {   /* set struct */
    struct CacheLine** lines;
    struct Node* head;
    struct Node* HashTable[HASHTABLE_MAX_LEN];
};
struct Tuple {
    char oper;
    int address;
    int len;
};
struct CacheSet** cache;

/* my function */
void getInitInfo(int* s, int* e, int* b, char* t, int argc, char* argv[]);
void printHelp();
void analysisFile(char* t, int s, int b, int e);
struct Tuple analysisInputLine(char* input);
void initCache(int s, int e, int b);
void LRUCache(int address, int s, int b, int e);


int main(int argc, char* argv[])
{
    int s, e, b; char t[1000];
    getInitInfo(&s, &e, &b, t, argc, argv);
    initCache(s, e, b);
    analysisFile(t, s, b, e);
    printSummary(hits, misses, evictions);
    return 0;
}


/* print help info */
void printHelp() {
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
            "Options:\n"
            "  -h         Print this help message.\n"
            "  -v         Optional verbose flag.\n"
            "  -s <num>   Number of set index bits.\n"
            "  -E <num>   Number of lines per set.\n"
            "  -b <num>   Number of block offset bits.\n"
            "  -t <file>  Trace file.\n\n"
            "Examples:\n"
            "  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

/* get the optional choice and required infos*/
void getInitInfo(int* s, int* e, int* b, char* t, int argc, char* argv[]) {
    char opt;
    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
        case 'h' :
            printHelp();
            break;
        case 's' :
            (*s) = atoi(optarg);
            break;
        case 'E' :
            (*e) = atoi(optarg);
            break;
        case 'b' :
            (*b) = atoi(optarg);
            break;
        case 't' :
            strcpy(t, optarg);
            break;
        default :
            printHelp();
            break;
        }
    }
}

/* analysis the file content */
void analysisFile(char* t, int s, int b, int e) {
    FILE* file = fopen(t, "r");
    if(file == NULL) {
        printf("file can not open");
        return;
    }
    char content[50];
    while (! feof(file)) {
        fgets(content, 50, file);
        if(content[0] == '\0') break;
        struct Tuple result = analysisInputLine(content);
        //printf("%c\n", result.oper);
        switch (result.oper) {
        case 'I' :
            break;
        case 'L' :
            LRUCache(result.address, s, b, e);
            break;
        case 'M' :
            LRUCache(result.address, s, b, e);
        case 'S' :
            LRUCache(result.address, s, b, e);
            break;
        }
        content[0] = '\0';
    }
}

/* analysis the input line */
struct Tuple analysisInputLine(char* input) {
    struct Tuple result;
    int i = 0;
    while(*input == ' ') input ++;
    result.oper = *input; input += 2;
    char address[20];
    while(*input != ',') {
        address[i++] = *input;
        input ++;
    }
    address[i] = '\0'; input ++;
    sscanf(address, "%x", &result.address);
    //printf("%s\n",address);
    char num[20]; i = 0;
    while(*input != '\n') {
        num[i ++] = *input;
        input ++;
    }
    num[i] = '\0';
    result.len = atoi(num);
    return result;
}

/* init cache memory space */
void initCache(int s, int e, int b) {
    s = (1 << s) + 1; b = (1 << b) + 1;
    /* create s cacheSet */
    cache = (struct CacheSet**)malloc(sizeof(struct cacheSet*) * s);
    for(int i = 0 ; i < s ; i ++) {
        cache[i] = (struct CacheSet*)malloc(sizeof(struct CacheSet));
        cache[i]->lines = (struct CacheLine**)malloc(sizeof(struct CacheLine*) * e);
        cache[i]->head = getNewLinkNode();
        for(int j = 0 ; j < HASHTABLE_MAX_LEN ; j ++)
            cache[i]->HashTable[j] = NULL;
        for(int j = 0 ; j < e ; j ++) {
            cache[i]->lines[j] = (struct CacheLine*)malloc(sizeof(struct CacheLine));
            cache[i]->lines[j]->flag = 0;
            cache[i]->lines[j]->tag = 0;
            cache[i]->lines[j]->words = (char*)malloc(sizeof(char) * b);
            cache[i]->lines[j]->id = j;
        }
    }
}

/* -------------- Core Code And LRU Function -------------- */

void parseAddress(unsigned int address, int s, int b,
                    unsigned long long* tag, int* setIndex, int* offset) {
    *tag = address >> (s + b);
    *setIndex = ((~(0xffffffff << (s + b))) & address) >> b;
    *offset = (~(0xffffffffffff << b)) & address;
}

void LRUCache(int address, int s, int b, int e) {
    
    int setIndex, offset;
    unsigned long long tag;
    parseAddress(address, s, b, &tag, &setIndex, &offset);
    s = (1 << s) + 1;
    //printf("tag=%lld , setIndex=%d\n", tag, setIndex);
    //printf("%x\n", address);
    struct CacheSet* set = cache[setIndex];
    struct CacheLine* empty = NULL;
    for(int i = 0 ; i < e ; i ++) {
        if(set->lines[i]->flag == 0) {
            empty = set->lines[i]; continue;
        }
        if(set->lines[i]->tag != tag) continue;
        hits++;
        hitAndPushFront(set->head, set->HashTable, set->lines[i]->id);
        //printf("hit\n");
        return;
    }
    //printf("miss\n");
    misses ++;
    if(empty != NULL) {
        int id = empty->id;
        empty->flag = 1; empty->tag = tag;
        insertFront(set->head, id);
        set->HashTable[id] = set->head->next;
        return;
    }
    //printf("eviction\n");
    evictions ++;
    struct Node* node = set->head;
    while(node->next != NULL) node = node->next;
    struct CacheLine* back = set->lines[node->element];
    int deleteId = back->id;
    deleteNode(set->HashTable[deleteId]);
    set->HashTable[deleteId] = NULL;
    insertFront(set->head, deleteId);
    set->HashTable[deleteId] = set->head->next;
    back->flag = 1; back->tag = tag;
}
