 #include <sys/types.h>
 #include <sys/socket.h>
 #include <stdio.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h> 
 #define n 7

 void connectwithserver(char host[],int port){
    int dfs_client;
    int len;
    char buff[1024];
    struct sockaddr_in adresse_serveur;
    int result;

    dfs_client = socket (AF_INET, SOCK_STREAM, 0);

    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = inet_addr (host);
    adresse_serveur.sin_port = htons (port);
    len = sizeof (adresse_serveur);

    result = connect (dfs_client, (struct sockaddr *) &adresse_serveur, len);
    if (result == -1)
    {
    perror ("Erreur de connexion au serveur");
    exit (1);
    }
 

    char text[100];
    fgets(text, sizeof(text), stdin);
    if(send(dfs_client, text, sizeof(text), 0) < 0)    
    {
        printf("\nTab non envoyé\n");
    }
    else
    {
        printf("\ntab envoyé\n");
    }
    
    printf("\n");
    
    
        
    close (dfs_client);
    exit (0);
 }

 struct server  {
    char* host ;
    int port ;
};

 int main ()
 {
    struct server server1 ;
    server1.host = "127.0.0.1" ;
    server1.port = 1265 ;

    struct server server2 ;
    server2.host = "127.0.0.1" ;
    server2.port = 12345 ;
    int x ;

    do
    {
        printf("Which server do you want to connect to?\nserver 1 (%s,%d) enter ==> 1\nserver 2 (%s,%d) enter ==> 2\n",server1.host,server1.port,server2.host,server2.port);
        scanf("%d",&x);
        char c ;
        scanf("%c",&c);
        if (x == 1 || x==2)
        {
            break;
        }else
        {
            printf("your input is wrong try again\n");
        }
        
        
    } while (true);
    

    if (x == 1)
    {
        connectwithserver(server1.host,server1.port);
    }else
    {
        connectwithserver(server2.host,server2.port);
    }
    

    return 0 ;
 }
 
 
 
