#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <pthread.h>
#define n 7

int numg = 0 ;
pthread_mutex_t lock;
int num_Words_other = 0 ;

int dfs_other_server = -1;
pthread_mutex_t lock2;

struct server  {
    char* host ;
    int port ;
};


int numWord(char t[]){
    int num = 0 ;
    int k = strlen(t) ;
    for (int i = 1; i < k; i++)
    {
        if ((t[i] == ' '||t[i]==','||t[i]=='.'||t[i]==';'||t[i]=='\n' )&&(t[i-1] != ' '&&t[i-1]!=','&&t[i-1]!='.'&&t[i-1]!=';'))
        {
            num ++;
        }
        
    }

    return num ;
    
}

void* recvformClient(void* arg) {

    int dfs_client = *(int*)arg;
    free(arg);
    char text[100];
    int num ;

    int read_size = recv(dfs_client, text, sizeof(text), 0);
    text[read_size] = '\0';
    

    pthread_mutex_lock(&lock2); 
    bool test = (dfs_client == dfs_other_server) ;
    pthread_mutex_unlock(&lock2);
     
    if (test)
    {
        num = atoi(text);
        
        pthread_mutex_lock(&lock);
        num_Words_other += num;
        numg += num;
        pthread_mutex_unlock(&lock);
        printf("the number of the words from the other server : %d\n",num_Words_other);
        printf("\t\tthe number of the words global is : %d\n",numg);
        return NULL ;
    }
    
    if(strcmp(text ,"i am the second server") == 0){
        pthread_mutex_lock(&lock2); 
        dfs_other_server = dfs_client ;
        pthread_mutex_unlock(&lock2);
        printf("we are connected with the other server\n");
        return NULL ;
    }
    printf("Tableau recevé:    \n");
    printf("the text is :\t%s\n",text);
    num = numWord(text);

    pthread_mutex_lock(&lock);
    numg += num;
    pthread_mutex_unlock(&lock);

    char str[5];
    snprintf(str, sizeof(str), "%d", num);
    send(dfs_other_server,str,strlen(str)+1,0);

    printf("the number of the words local is : %d\n",num);
    printf("\t\tthe number of the words global is : %d\n",numg);
    close (dfs_client);

    return NULL;
}

int main(){

     
    int dfs_serveur, dfs_client, read_size;
    int long_adr_serveur, long_adr_client;
    char buff [4];
    struct sockaddr_in adresse_serveur;
    struct sockaddr_in adresse_client;
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock2, NULL);

    dfs_serveur = socket (AF_INET, SOCK_STREAM, 0);
    if (dfs_serveur == -1)
    {
        perror ("socket error");
        exit (1);
    }

    
    struct server server1 ;
    server1.host = "127.0.0.1" ;
    server1.port = 1265 ;

    struct server server2 ;
    server2.host = "127.0.0.1" ;
    server2.port = 12345 ;

    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = inet_addr(server1.host);
    adresse_serveur.sin_port = htons(server1.port);

    long_adr_serveur = sizeof (adresse_serveur);
    if (bind(dfs_serveur, (struct sockaddr *) &adresse_serveur,long_adr_serveur) == -1)
    {
        perror ("bind error");
        exit (1);
    }

    listen (dfs_serveur, 5);

    
    int max_clients = 10 ;
   
    
    int i = 0 ;
    while (1){
    
        long_adr_client = sizeof (adresse_client);
        dfs_client = accept (dfs_serveur, (struct sockaddr *) &adresse_client,&long_adr_client);

        int* pclient = malloc(sizeof(int));  
        *pclient = dfs_client;
        pthread_t thread ;

        pthread_create(&thread, NULL, recvformClient, pclient); 
        
        pthread_detach(thread);
            
    }

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock2);

    return 0 ;
}


