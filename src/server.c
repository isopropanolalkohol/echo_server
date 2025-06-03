// echo_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       // für close()
#include <arpa/inet.h>    // für inet_ntoa, htons, sockaddr_in
#include <sys/socket.h>   // für socket-Funktionen

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 1. Socket erstellen
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Serveradresse vorbereiten
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // akzeptiere jede IP
    server_addr.sin_port = htons(PORT);        // Port in Netzwerk-Byte-Reihenfolge

    // 3. Binden
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. Lauschen (Wartet auf Verbindungen)
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Echo-Server läuft auf Port %d...\n", PORT);

    // 5. Verbindung akzeptieren
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Verbindung von %s:%d akzeptiert\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // 6. Daten empfangen und zurücksenden
    ssize_t bytes_received;
    while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        send(client_fd, buffer, bytes_received, 0);  // Echo
    }

    printf("Verbindung beendet.\n");
    close(client_fd);
    close(server_fd);
    return 0;
}

