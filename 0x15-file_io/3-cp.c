#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

char *create_buffer(char *file);
void close_file(int fd);

char *create_buffer(char *file) {
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    if (buffer == NULL) {
        dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file);
        exit(99);
    }
    return buffer;
}

void close_file(int fd) {
    if (close(fd) == -1) {
        dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
        exit(100);
    }
}

int main(int argc, char *argv[]) {
    int from, to, r, w;
    char *buffer;

    if (argc != 3) {
        dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
        exit(97);
    }

    buffer = create_buffer(argv[2]);
    from = open(argv[1], O_RDONLY);
    if (from == -1) {
        dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", argv[1]);
        free(buffer);
        exit(98);
    }

    to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (to == -1) {
        dprintf(STDERR_FILENO, "Error: Can't write to %s\n", argv[2]);
        free(buffer);
        close_file(from);
        exit(99);
    }

    do {
        r = read(from, buffer, BUFFER_SIZE);
        if (r == -1) {
            dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", argv[1]);
            free(buffer);
            close_file(from);
            close_file(to);
            exit(98);
        }
        w = write(to, buffer, r);
        if (w == -1) {
            dprintf(STDERR_FILENO, "Error: Can't write to %s\n", argv[2]);
            free(buffer);
            close_file(from);
            close_file(to);
            exit(99);
        }
    } while (r > 0);

    free(buffer);
    close_file(from);
    close_file(to);

    return 0;
}
