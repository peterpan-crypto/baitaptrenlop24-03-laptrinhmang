#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd, n;
    char buf[BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("UDP server dang chay cong %d...\n", PORT);

    while (1) {
        n = recvfrom(sockfd, buf, BUF_SIZE - 1, 0,
                     (struct sockaddr *)&client_addr, &client_len);
        if (n > 0) {
            buf[n] = '\0';
            printf("Nhan tu client: %s\n", buf);

            sendto(sockfd, buf, n, 0,
                   (struct sockaddr *)&client_addr, client_len);
        }
    }

    close(sockfd);
    return 0;
}
