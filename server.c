#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define TODAY 3
#define MAX 100
#define MAXLIMIT 100
#define LIMIT 30

struct book
{
    char bookname[100];
    int issue_day;
    int return_day;
    int count;
} books[MAX];

struct waitinglist
{
    char wname[100];
    char wid[100];
    int bookno;
    int notify_on;
};

struct issuedlist
{
    char isname[100];
    char isid[100];
    int bookno;
    int return_day;
};

int icount = 0;
int wcount = 0;

int main()
{
    struct waitinglist wait[100];
    struct issuedlist done[100];

    char name[100];
    char reg[100];

    for (int i = 0; i < MAX; i++)
    {
        books[i].count = 1;
    }
    strcpy(books[0].bookname, "Java Programming Language 8 by Herbert Schildt ");
    strcpy(books[1].bookname, "Network Programming by W. Richard Stevens ");
    strcpy(books[2].bookname, "Digital Logic and Computer Design by M. Morris Mano ");
    strcpy(books[3].bookname, "Discrete mathematics and its applications by Kenneth H. Rosen ");
    strcpy(books[4].bookname, "Introduction to Algorithms by Thomas H. Cormen ");

    char str[500]; // string to contain response
    char chc[5], sub[10], status[1000];

    int listen_fd, comm_fd; //connection file descriptor

    struct sockaddr_in servaddr; // socket object address

    listen_fd = socket(AF_INET, SOCK_STREAM, 0); //create a socket

    bzero(&servaddr, sizeof(servaddr)); //clear the address

    servaddr.sin_family = AF_INET;                //automatic
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); //use IP address as server address
    servaddr.sin_port = htons(8080);              //Same as in Client

    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    // bind socket to localhost
    listen(listen_fd, 10); //passive mode- hear from client
    comm_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
    //accept the connection from first client
    while (1)
    {
        printf("Server Started ....... Listening Now.....");
        bzero(str, 100);        //clear the response buffer
        read(comm_fd, chc, 5);  // read again from connection
        int choice = atoi(chc); // extract number 2 from string

        if (choice == 2)
        {

            read(comm_fd, name, 100); // read again from connection
            name[strlen(name) - 1] = '\0';

            read(comm_fd, reg, 100); // read again from connection
            reg[strlen(reg) - 1] = '\0';

            read(comm_fd, str, 100); // read again from connection
            int index = atoi(str);   // extract number 2 from string
            if (index > 100)
                break;
            if (books[index].count > 0)
            {
                books[index].issue_day = TODAY;
                books[index].return_day = (TODAY + LIMIT) % 30;
                books[index].count--;
                done[icount].bookno = index;
                strcpy(done[icount].isname, name);
                strcpy(done[icount].isid, reg);
                done[icount].return_day = books[index].return_day;
                icount++;

                sprintf(str, "The Book : %s is Issued, please return it by %d of next month", books[index].bookname, books[index].return_day); //create respons
                write(comm_fd, str, strlen(str));
            }
            else
            {

                wait[wcount].bookno = index;
                strcpy(wait[wcount].wname, name);
                strcpy(wait[wcount].wid, reg);
                wait[wcount].notify_on = books[index].return_day;
                wcount++;
                sprintf(str, "The Book : %s is unavailable at the moment, please come back on %d of next month", books[index].bookname, books[index].return_day); //create response
                write(comm_fd, str, strlen(str) + 1);
            }
        }

        else if (choice == 3)
        {
            read(comm_fd, sub, 10); // read again from connection
            int index = atoi(sub);
            books[index].count++;
            sprintf(str, "Book Successfully Returned");
            write(comm_fd, str, strlen(str) + 1);
        }

        else if (choice == 4)
        {
            bzero(status, 1000);
            char count[50], head[100];
            sprintf(head, "%15s %15s %15s %15s %15s\n", "sr no", "Name", "ID", "bookno", "notify_on");
            write(comm_fd, head, strlen(head));
            sprintf(count, "%d", wcount);
            write(comm_fd, count, strlen(count));
            for (int i = 0; i < wcount; i++)
            {
                sprintf(status, "%15d %15s %15s %15d %15d\n", i, wait[i].wname, wait[i].wid, wait[i].bookno, wait[i].notify_on);
                write(comm_fd, status, strlen(status));
            }
        }
        else
        {
            exit(0);
        }
    }
    printf("Issued list Today \n");
    printf("%15s %15s %15s %15s %15s\n", "sr no", "Name", "ID", "bookno", "return_on");
    for (int i = 0; i < wcount; i++)
    {
        printf("%15d %15s %15s %15d %15d\n", i, done[i].isname, done[i].isid, done[i].bookno, done[i].return_day);
    }
    printf("Waiting list Today \n");
    printf("%15s %15s %15s %15s %15s\n", "sr no", "Name", "ID", "bookno", "notify_on");
    for (int i = 0; i < wcount; i++)
    {
        printf("%15d %15s %15s %15d %15d\n", i, wait[i].wname, wait[i].wid, wait[i].bookno, wait[i].notify_on);
    }
}
