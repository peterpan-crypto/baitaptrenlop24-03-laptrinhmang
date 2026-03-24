#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define IP "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    char *msg[] = {
        "SOICTSOICT012345678901234567890123456789012345",
        "6789SOICTSOICTSOICT012345678901234567890123456",
        "7890123456789012345678901234567890123456789012",
        "3456789SOICTSOICT01234567890123456789012345678"
    };

    for (int i = 0; i < 4; i++) {
        send(sock, msg[i], strlen(msg[i]), 0);
        printf("Da gui lan %d: %s\n", i + 1, msg[i]);
        sleep(1);
    }

    close(sock);
    return 0;
}
