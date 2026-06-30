#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

//Protocol definition, Data exchange format: "username:password"

// server side code
void server_process(int read_fd, int write_fd) {
    char buffer[BUFFER_SIZE];
   
    while (read(read_fd, buffer, BUFFER_SIZE) > 0) {
        printf("[Server] Received data: %s\n", buffer);
       
        // Protocol format (username:password)
        char *username = strtok(buffer, ":");
        char *password = strtok(NULL, ":");
        char response[BUFFER_SIZE];
       
        // Authentication for basic security
        if (username != NULL && password != NULL) {
            // authenticating 2 statis users
            if ((strcmp(username, "user1") == 0 && strcmp(password, "pass1") == 0) ||
                (strcmp(username, "user2") == 0 && strcmp(password, "pass2") == 0)) {
                strcpy(response, "AUTH_SUCCESS: Welcome to the server!");
            } else {
                strcpy(response, "AUTH_FAILED: Invalid credentials.");
            }
        } else {
            strcpy(response, "AUTH_FAILED: Malformed protocol request.");
        }
       
        // Sending results back to client side
        write(write_fd, response, strlen(response) + 1);
    }
}

// Client side code
void client_process(int read_fd, int write_fd, const char* user, const char* pass) {
    char buffer[BUFFER_SIZE];
   
    // Format data according to our protocol
    snprintf(buffer, BUFFER_SIZE, "%s:%s", user, pass);
   
    printf("\n[Client %s] Attempting to connect & authenticate...\n", user);
    write(write_fd, buffer, strlen(buffer) + 1); // Send to server

    read(read_fd, buffer, BUFFER_SIZE); // Read server response
    printf("[Client %s] Server Response: %s\n", user, buffer);
}

int main() {
    // creating 2 pipes for two way communication
    int pipe_c2s[2]; // Client sending to Server
    int pipe_s2c[2]; // Server sending to Client

    if (pipe(pipe_c2s) == -1 || pipe(pipe_s2c) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork(); // Create a new process

    if (pid > 0) {
        // parent process (server)
        close(pipe_c2s[1]); // Server doesn't write to client's send-pipe
        close(pipe_s2c[0]); // Server doesn't read from its own reply-pipe
       
        printf("[Server] Booting up... Listening for authentication requests...\n");
        server_process(pipe_c2s[0], pipe_s2c[1]);
       
        close(pipe_c2s[0]);
        close(pipe_s2c[1]);
        wait(NULL); // Wait for client to finish
        printf("\n[Server] Shutting down.\n");
       
    } else if (pid == 0) {
        // child process (multiple clients)
        close(pipe_c2s[0]);
        close(pipe_s2c[1]);
       
        // Test 1: Valid User 1
        client_process(pipe_s2c[0], pipe_c2s[1], "user1", "pass1");
        sleep(1); // Small pause for console readability
       
        // Test 2: Valid User 2
        client_process(pipe_s2c[0], pipe_c2s[1], "user2", "pass2");
        sleep(1);
       
        // Test 3: Unauthorized Hacker
        client_process(pipe_s2c[0], pipe_c2s[1], "hacker", "wrongpass");
       
        close(pipe_c2s[1]);
        close(pipe_s2c[0]);
        exit(0);
    }
    return 0;
}
