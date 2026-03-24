#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main() {
    int sockfd, n;
    char buf[BUF_SIZE];
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        printf("Nhap tin nhan: ");
        fgets(buf, BUF_SIZE, stdin);

        int len = strlen(buf);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
            len--;
        }

        sendto(sockfd, buf, len, 0,
               (struct sockaddr *)&server_addr, server_len);

        n = recvfrom(sockfd, buf, BUF_SIZE - 1, 0,
                     (struct sockaddr *)&server_addr, &server_len);
        if (n > 0) {
            buf[n] = '\0';
            printf("Server echo: %s\n", buf);
        }
    }

    close(sockfd);
    return 0;
}
