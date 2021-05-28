#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int sockfd, n;
    char chc[5];
    char sendline[100]; // line to contain write information
    char recvline[500]; // line to contain received information
    int input;
    int counter = 1;
    char stname[50],stnum[50],sn[5],tn[5],stat[1000];
    struct sockaddr_in servaddr; //create a socket object

    //creating the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // initialize socket to 
    bzero(&servaddr, sizeof (servaddr)); //Initialize the servaddr array

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080); //host to network short

    // Conversion from  text address to packed address
    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

    connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)); //make connection
    int num;
    //Working Loop
    while (1) {
        bzero(sendline, 100); //clear the line
        bzero(recvline, 100); //clear the line
        printf("* Welcome to RUAS Library *\n");
        printf(" MENU \n");

        printf(" 1. Book Search \n");
        printf(" 2. Issue a Book \n");
        printf(" 3. Return a Book \n");
        printf(" 4. Status \n");
        printf(" 5. Exit  \n");
        printf("Press the Option : ");
        fgets(chc,5,stdin);
        input=atoi(chc);
        write(sockfd, chc, strlen(chc)); // write to local server 
        switch (input) {
            case 1:
                printf("* Books List **\n");
                printf("Note : To Check the Availability of the book Please go to Menu option status(4)\n");
                printf("\n");
                printf("1. Java Programming Language 8 by Herbert Schildt \n");
                printf("2. Network Programming by W. Richard Stevens \n");
                printf("3. Digital Logic and Computer Design by M. Morris Mano \n");
                printf("4. Discrete mathematics and its applications by Kenneth H. Rosen \n");
                printf("5. Introduction to Algorithms by Thomas H. Cormen \n");
                printf("********\n");
                break;

            case 2:
                printf("* Issue Book * \n");
                printf("Enter Your Name :");
                fgets(stname, 50, stdin); // accept the second input through stdin
                write(sockfd, stname, strlen(stname));



                printf("Enter Your Registration Number: ");
                fgets(stnum, 50, stdin); // accept the second input through stdin
                write(sockfd, stnum, strlen(stnum));


                printf("Enter Book number : ");
                fgets(sn, 5, stdin); // accept the second input through stdin
                write(sockfd, sn, strlen(sn)); // write to local server 
                read(sockfd, recvline, 500); // read results from server
                printf("\n");
                printf("From Server: %s\n", recvline); //print the results  

                break;

            case 3:

                printf("Enter Book number : ");
                fgets(tn, 5, stdin); // accept the second input through stdin
                write(sockfd, tn, strlen(tn)); // write to local server 
                read(sockfd, recvline, 100); // read results from server
                printf("\n");
                printf("From Server: %s\n", recvline); //print the results  

                break;


            case 4:
                printf("* The Waiting List * \n");
                int count;
                char cnt[50],head[100];
                read(sockfd,head,100);
                read(sockfd,cnt,50);
                count=atoi(cnt);
                printf("%s\n",head);
                for(int i=0;i<count;i++){
                read(sockfd, stat, 1000); // read results from server
                printf("\n");
                printf("From Server: %s\n", stat); //print the results
                }
                break;

            case 5:
                exit(0);
                break;

        }



    }

}