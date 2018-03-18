#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0;
    ssize_t valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
    // which is meant to be, and rest is defined below

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts an IP address in numbers-and-dots notation into either a
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    struct stat st = {0};

    if (stat("downloaded_files", &st) == -1) {
        mkdir("downloaded_files", 0700);
    }

    printf("Commands\n--------\n1) `listfiles` or `l`\n2) `download <filename>` or `d <filename>`\n3) `exit` or `e`\n\n");

    char *command = (char *) malloc(1000 * sizeof(char));
    char filename[1024], tfile[1024];
    FILE *file = NULL;
    size_t commandSize = 1000;


    while (true) {
        memset(buffer, 0, sizeof(buffer));
        printf("SERVER $ ");
        getline(&command, &commandSize, stdin);
        printf("\n");
        command[strlen(command) - 1] = 0;
        if (command[0] == 'e') {
            send(sock, command, strlen(command), 0);  // send the message.
            break;
        } else if (command[0] == 'd') {
            int i=0, j;
            for (i = 0; i < 1024; ++i) {
                if (command[i] == ' ')
                    break;
            }
            while (i < strlen(command)) {
                char tmpcmd[1024];
                memset(tfile, 0, sizeof(tfile));
                memset(tmpcmd, 0, sizeof(tmpcmd));
                memset(buffer, 0, sizeof(buffer));

                while (i < strlen(command) && command[i] == ' ')
                    i++;
                for (j = i; j < strlen(command) && command[j] != 0 && command[j] != '\n' && command[j] != ' '; ++j) {
                    tfile[j - i] = command[j];
                }

                strncpy(tmpcmd, "download ", 9);
                strcat(tmpcmd, tfile);

                send(sock, tmpcmd, strlen(tmpcmd), 0);  // send the message.
                valread = read(sock, buffer, 1024);  // receive message back from server, into the buffer
                printf("%s\n", buffer);

                if (buffer[0] == '2') {
                    memset(filename, 0, sizeof(filename));
                    strncpy(filename, "downloaded_files/", 17);
                    strcat(filename, tfile);
                    file = fopen(filename, "w");
                    int j = 3, ct = 0;
                    char siz[50];
                    memset(siz, 0, 50);
                    while (j < 1024 && (buffer[j] < '0' || buffer[j] > '9')) {
                        j++;
                    }
                    while (buffer[j] >= '0' && buffer[j] <= '9') {
                        siz[ct++] = buffer[j++];
                    }
                    int sz = atoi(siz);
                    for (int k = 1; k <= sz; ++k) {
                        memset(buffer, 0, 1024);
                        valread = read(sock, buffer, 1000);
                        fprintf(file, buffer);
                    }
                    fclose(file);
                    printf("Download Complete.\n");
                }
                printf("\n");
                i = j;
            }
        } else {
            send(sock, command, strlen(command), 0);  // send the message.
            valread = read(sock, buffer, 1024);  // receive message back from server, into the buffer
            printf("%s\n", buffer);
        }
    }
    return 0;
}
