#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *create_buffer(int size);
void close_file(int fd);

char *create_buffer(int size) {
    char *buffer = malloc(sizeof(char) * size);
    if (buffer == NULL) {
        perror("Error allocating buffer");
        exit(99);
    }
    return buffer;
}

void close_file(int fd) {
    if (close(fd) == -1) {
        perror("Error closing file");
        exit(100);
    }
}

int main(int argc, char *argv[]) {
    int from, to, r, w;
    char *buffer;

    if (argc != 3) {
        fprintf(stderr, "Usage: cp file_from file_to\n");
        exit(97);
    }

    from = open(argv[1], O_RDONLY);
    if (from == -1) {
        perror("Error opening source file");
        exit(98);
    }

    to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (to == -1) {
        perror("Error opening destination file");
        close_file(from);
        exit(99);
    }

    buffer = create_buffer(1024);

    while ((r = read(from, buffer, 1024)) > 0) {
        if (r == -1) {
            perror("Error reading from source file");
            free(buffer);
            close_file(from);
            close_file(to);
            exit(98);
        }

        w = write(to, buffer, r);
        if (w == -1) {
            perror("Error writing to destination file");
            free(buffer);
            close_file(from);
            close_file(to);
            exit(99);
        }
    }

    free(buffer);
    close_file(from);
    close_file(to);

    return 0;
}
