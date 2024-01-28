#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAXLEN 1024
typedef struct sockaddr SA;

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

/* ---------- my function ---------- */
void doit(int connfd);
void parseHost(char* buf, char* host, char* port);
void connectAndReturn(char* buf, int clientfd, int connfd);
void parseUri(rio_t* rio, char* buf);
void parsePath(char* path, char* ori);

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
    while(1) {
        clientLen = sizeof(clientAddr);
        connfd = Accept(listenfd, (SA*)&clientAddr, &clientLen);
        Getnameinfo((SA*)&clientAddr, clientLen, hostname, MAXLEN,
                    port, MAXLEN, 0);
        fprintf(stdout, "Accept connection from (%s %s)\n\n", hostname, port);
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
    connectAndReturn(buf, clientfd, connfd);
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

void connectAndReturn(char* buf, int clientfd, int connfd) {
    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, strlen(buf));
    printf("----\n");

    size_t n;
    //回复给客户端
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("proxy received %d bytes,then send\n", (int)n);
        Rio_writen(connfd, buf, n);
    }
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