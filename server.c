#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    char buf[BUF_SIZE + 1];
    char tail[10] = "";        
    const char *pattern = "0123456789";
    int count = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Server dang cho ket noi...\n");
    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    printf("Client da ket noi.\n");

    int n;
    while ((n = recv(client_fd, buf, BUF_SIZE, 0)) > 0) {
        buf[n] = '\0';

        char data[BUF_SIZE + 20];   
        strcpy(data, tail);
        strcat(data, buf);

        int len = strlen(data);
        for (int i = 0; i <= len - 10; i++) {
            if (strncmp(data + i, pattern, 10) == 0) {
                count++;
            }
        }

        if (len >= 9) {
            strcpy(tail, data + len - 9);
        } else {
            strcpy(tail, data);
        }

        printf("So lan xuat hien \"%s\": %d\n", pattern, count);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
