#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

void recvn(int s, void *buf, int len) {
    int n, r = 0;
    while (r < len) {
        n = recv(s, (char *)buf + r, len - r, 0);
        if (n <= 0) exit(1);
        r += n;
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);

    uint32_t len, count, name_len;
    uint64_t size;

    recvn(client_fd, &len, 4);
    len = ntohl(len);

    char *dirname = malloc(len + 1);
    recvn(client_fd, dirname, len);
    dirname[len] = 0;

    recvn(client_fd, &count, 4);
    count = ntohl(count);

    printf("%s\n", dirname);

    for (uint32_t i = 0; i < count; i++) {
        recvn(client_fd, &name_len, 4);
        name_len = ntohl(name_len);

        char *name = malloc(name_len + 1);
        recvn(client_fd, name, name_len);
        name[name_len] = 0;

        recvn(client_fd, &size, 8);
        size = be64toh(size);

        printf("%s - %llu bytes\n", name, (unsigned long long)size);
        free(name);
    }

    free(dirname);
    close(client_fd);
    close(server_fd);
    return 0;
}
