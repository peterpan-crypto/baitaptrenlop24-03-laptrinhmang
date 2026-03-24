#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <limits.h>

#define PORT 8080
#define IP "127.0.0.1"

void sendn(int s, void *buf, int len) {
    int n, sent = 0;
    while (sent < len) {
        n = send(s, (char *)buf + sent, len - sent, 0);
        if (n <= 0) exit(1);
        sent += n;
    }
}

int main() {
    int sock;
    struct sockaddr_in server;
    char cwd[PATH_MAX];
    struct dirent *e;
    struct stat st;
    DIR *d;
    uint32_t count = 0;

    getcwd(cwd, sizeof(cwd));
    d = opendir(".");

    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        if (stat(e->d_name, &st) == 0 && S_ISREG(st.st_mode)) count++;
    }

    rewinddir(d);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    uint32_t len = htonl(strlen(cwd));
    uint32_t c = htonl(count);

    sendn(sock, &len, 4);
    sendn(sock, cwd, strlen(cwd));
    sendn(sock, &c, 4);

    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        if (stat(e->d_name, &st) == 0 && S_ISREG(st.st_mode)) {
            uint32_t name_len = htonl(strlen(e->d_name));
            uint64_t size = htobe64(st.st_size);

            sendn(sock, &name_len, 4);
            sendn(sock, e->d_name, strlen(e->d_name));
            sendn(sock, &size, 8);
        }
    }

    closedir(d);
    close(sock);
    return 0;
}
