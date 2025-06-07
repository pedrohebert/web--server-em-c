#include <sys/types.h>      // tipos de dados como socklen_t
#include <sys/socket.h>     // funções socket(), bind(), listen(), accept()
#include <netinet/in.h>     // estrutura sockaddr_in
#include <arpa/inet.h>      // funções de conversão de endereços (inet_addr etc)
#include <unistd.h>         // close(), read(), write()
#include <string.h>      // funções de manipulação de strings (strlen, strcpy etc)
#include <stdio.h>      // funções de entrada e saída (printf, scanf etc)
#include <stdlib.h>     // funções de alocação de memória (malloc, free etc)
#include <ctype.h>


#define PORT 8080

int fatorial(int n);

int main(){
    int server, client;
    struct sockaddr_in endereços;
    char buffer[1024] = {0};

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == 0){
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    endereços.sin_family = AF_INET;
    //Define o tipo de endereço.
    //AF_INET significa IPv4 (Address Family: INET).
    //Outros valores possíveis:
    //AF_INET6 → IPv6
    //AF_UNIX → comunicação local entre processos (não é TCP/IP)
    endereços.sin_addr.s_addr = INADDR_ANY;
    // define em qual ip o socket (server) vai escutar
    // INADDR_ANY significa que o socket vai escutar de todas as fontes possiveis 127.0.0.1 (localhost) e 192.168.0.10 (rede)
    // para escutar apena um esdereço ip especifico usa: inet_addr("127.0.0.1") 
    endereços.sin_port = htons(PORT);
    // diz a porta que sera usada para a comunicação

    if (bind(server,(struct sockaddr *)&endereços, sizeof(endereços)) < 0){
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    
    listen(server, 3);

    int num;

    socklen_t tamanho = sizeof(endereços);
    client = accept(server, (struct sockaddr *)&endereços, &tamanho);
    if (client < 0){
        perror("erro no accept");
        exit(EXIT_FAILURE);
    }
    do{
        strcpy(buffer, "\nola, gidite um valor inteiro: ");
        send(client, buffer, strlen(buffer), 0);
        
        read(client, buffer, 1024);
    
        if (isdigit(buffer[0])){
            for (int i = 0; isdigit(buffer[i]); i++){
                num = num*10*i + buffer[i] - '0';
            }

            num = fatorial(num);
            
            sprintf(buffer, "%d", num);
            send(client, buffer, strlen(buffer), 0);
        }else{
            FILE* saida;

            saida = fopen(".\\teste.html", "r");
            send(client, saida, sizeof(saida), 0);
        }



    }while(strcmp(buffer, "exit\n") != 0);

    close(client);
    close(server);
return 0;
}

int fatorial(int n){
    if (n <=1){
        return 1;
    }
    
    return n * fatorial(n-1);
}