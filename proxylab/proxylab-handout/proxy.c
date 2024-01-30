#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAXLEN 1024
#define SBUFSIZE 32
#define NTHREADS 8
#define MAX_CACHE_LINE 32
typedef struct sockaddr SA;

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

/* sbuf struct and functions */
typedef struct {
    int* buf;
    int n;
    int front;
    int rear;
    sem_t mutex;
    sem_t slots;
    sem_t items;
} sbuf_t;

void sbuf_init(sbuf_t* sp, int n);
void sbuf_deinit(sbuf_t* sp);
void sbuf_insert(sbuf_t* sp, int item);
int sbuf_remove(sbuf_t* sp);

/* link struct and functions */
struct Node {
    int element;
    struct Node* next;
    struct Node* pre;
};

struct Node* getNewLinkNode();
void insertFront(struct Node* head, int ele);
void deleteNode(struct Node* address);

/* Cache struct and functions */
struct CacheLine {
    int flag;
    unsigned long long int hash;
    char url[MAXLEN];
    char words[MAX_OBJECT_SIZE];
    struct Node* addr;
};
struct Cache {
    struct CacheLine lines[MAX_CACHE_LINE];
    struct Node* head;
}cache;

void cache_init(struct Cache* cache);
char* readCache(char* url);
void writeCache(char* url, char* content);
unsigned long long int BKDRHash(char* put);
void writeLine(char* url, char* content,
               int i, unsigned long long int H);


/* ---------- my function ---------- */
void doit(int connfd);
void parseHost(char* buf, char* host, char* port);
void connectAndReturn(char* buf, int clientfd, int connfd, char* url);
void parseUri(rio_t* rio, char* buf);
void parsePath(char* path, char* ori);
void* thread(void* vargp);

/* my global var */
sbuf_t sbuf;

int main(int argc, char** argv)
{
    int listenfd, connfd;
    char hostname[MAXLEN], port[MAXLEN], listenPort[MAXLEN];
    socklen_t clientLen;
    struct sockaddr_storage clientAddr;

    /* 
        checke if input specified port,
        if not, call './port-for-user.pl' get a useable port
    */
    if(argc != 2) {
        fprintf(stderr, "[Warning] : usage: %s <port>\n", argv[0]);
        int fd[2];
        if(pipe(fd) == -1) {
            fprintf(stderr, "[Error] : pipe error\n");
            exit(1);
        }
        pid_t pid;
        if((pid = fork()) == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            system("./port-for-user.pl");
            exit(0);
        }
        wait(NULL);
        close(fd[1]);
        rio_readn(fd[0], listenPort, MAXLEN);
    }else strcpy(listenPort, argv[1]);

    
    /* listen to connect */
    fprintf(stdout, "open proxy port is %s....\n", listenPort);
    listenfd = Open_listenfd(listenPort);

    cache_init(&cache); /* cache init */

    /* prethreading */
    sbuf_init(&sbuf, SBUFSIZE);
    int i;
    pthread_t tid;
    for(i = 0 ; i < NTHREADS ; i++)
        Pthread_create(&tid, NULL, thread, NULL);

    while(1) {
        clientLen = sizeof(clientAddr);
        connfd = Accept(listenfd, (SA*)&clientAddr, &clientLen);
        sbuf_insert(&sbuf, connfd);
        Getnameinfo((SA*)&clientAddr, clientLen, hostname, MAXLEN,
                    port, MAXLEN, 0);
        fprintf(stdout, "Accept connection from (%s %s)\n\n", hostname, port);
    }
}

void* thread(void* vargp) {
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf);
        doit(connfd);
        Close(connfd);        
    }
}

void doit(int connfd) {
    char buf[MAX_OBJECT_SIZE];
    char method[MAXLEN],url[MAXLEN], version[MAXLEN];
    rio_t rio;

    /* get useable infos */
    Rio_readinitb(&rio, connfd);
    Rio_readlineb(&rio, buf, MAX_OBJECT_SIZE);
    printf("Require headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, url, version);

    /* read from cache */
    char* cacheWord;
    if((cacheWord = readCache(url)) != NULL) {
        Rio_writen(connfd, cacheWord, strlen(cacheWord));
        return;
    }

    parseUri(&rio, buf);
    printf("%s", buf);

    /* parse host infos */
    char host[MAXLEN], port[MAXLEN];
    parseHost(buf, host, port);

    /* build return host*/
    buf[0] = '\0'; char path[MAXLEN];
    parsePath(path, url);
    sprintf(buf, "%s %s HTTP/1.0\r\n", method, path);
    sprintf(buf, "%sHost: %s:%s\r\n", buf, host, port);
    sprintf(buf, "%s%s", buf, user_agent_hdr);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sProxy-Connection: close\r\n\r\n", buf);
    

    /* send info to service */
    int clientfd;
    clientfd = Open_clientfd(host, port);
    connectAndReturn(buf, clientfd, connfd, url);
}

void parseHost(char* buf, char* host, char* port) {
    char* ptr = buf + 6; // skip 'Host: '
    int i = 0;
    while((*ptr) != ':') {
        host[i ++] = (*ptr); ptr ++;
    }
    host[i] = '\0'; i = 0;
    ptr ++; // skip ':'
    while((*ptr) != '\r') {
        port[i ++] = (*ptr); ptr ++;
    }
    port[i] = '\0';
}

void connectAndReturn(char* buf, int clientfd, int connfd, char* url) {
    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, strlen(buf));
    char content[MAX_OBJECT_SIZE];

    size_t n;
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("proxy received %d bytes,then send\n", (int)n);
        Rio_writen(connfd, buf, n);
        sprintf(content, "%s%s", content, buf);
    }
    writeCache(url, content);
    Close(clientfd);
}

void parseUri(rio_t* rio, char* buf) {
    while(Rio_readlineb(rio, buf, MAX_OBJECT_SIZE) > 0) {
        if(strncmp("Host", buf, 4) == 0) {
            return;
        }
    }
}

void parsePath(char* path, char* ori) {
    char* tem = strstr(ori, "//");
    tem += 2;
    tem = strstr(tem, "/");
    strcpy(path, tem);
}


/* sbuf funtions */
void sbuf_init(sbuf_t* sp, int n){
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n; /* Buffer holds max of n items */
    sp->front = sp->rear = 0; /* Empty buffer iff front == rear */ 
    Sem_init(&sp->mutex, 0, 1); /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n); /* Initially, buf has n empty slots */ 
    Sem_init(&sp->items, 0, 0); /* Initially, buf has zero data items */
}

void sbuf_deinit(sbuf_t* sp) {
    Free(sp->buf);
}

void sbuf_insert(sbuf_t* sp, int item) {
    P(&sp->slots); /* Wait for available slot */ 
    P(&sp->mutex); /* Lock the buffer */ 
    sp->buf[(++sp->rear)%(sp->n)] = item; /* Insert the item */ 
    V(&sp->mutex); /* Unlock the buffer */ 
    V(&sp->items); /* Announce available item */
}

int sbuf_remove(sbuf_t* sp) {
    int item; 
    P(&sp->items); /* Wait for available item */ 
    P(&sp->mutex); /* Lock the buffer */ 
    item = sp->buf[(++sp->front)%(sp->n)]; /* Remove the item */ 
    V(&sp->mutex); /* Unlock the buffer */ 
    V(&sp->slots); /* Announce available slot */ 
    return item;
}

/* link function */
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

/* Cache functions */
void cache_init(struct Cache* cache) {
    cache->head = getNewLinkNode();
    int i;
    for(i = 0 ; i < MAX_CACHE_LINE ; i ++) {
        cache->lines[i].flag = 0;
        cache->lines[i].addr = NULL;
    }
}

char* readCache(char* url) {
    unsigned long long int H = BKDRHash(url);
    int i;
    for(i = 0 ; i < MAX_CACHE_LINE ; i ++) {
        if(cache.lines[i].flag == 0) continue;
        if(cache.lines[i].hash != H) continue;
        if(strcmp(url, cache.lines[i].url) != 0) continue;
        deleteNode(cache.lines[i].addr);
        insertFront(cache.head, i);
        cache.lines[i].addr = cache.head->next;
        return cache.lines[i].words;
    }
    return NULL;
}

void writeCache(char* url, char* content) {
    unsigned long long int H = BKDRHash(url);
    int i;
    for(i = 0 ; i < MAX_CACHE_LINE ; i ++) {
        if(cache.lines[i].flag == 0) {
            writeLine(url, content, i, H);
            return;
        }
    }
    struct Node* tem = cache.head;
    while(tem->next != NULL) tem = tem->next;
    int ele = tem->element;
    deleteNode(tem);
    writeLine(url, content, ele, H);
}

void writeLine(char* url, char* content,
               int i, unsigned long long int H) {
    cache.lines[i].flag = 1;
    cache.lines[i].hash = H;
    strcpy(cache.lines[i].url, url);
    strcpy(cache.lines[i].words, content);
    insertFront(cache.head, i);
    cache.lines[i].addr = cache.head->next;
}

unsigned long long int BKDRHash(char* put){
    unsigned long long int P = 131, H = 0;
    int temp, len = strlen(put);
    for(temp = 0 ; temp < len ; temp++)
        H = H * P + put[temp];
    return H;
}