#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

void print_error(const char *msg, const char *file, int exit_code) {
    dprintf(STDERR_FILENO, "Error: %s %s\n", msg, file);
    exit(exit_code);
}

int main(int argc, char *argv[]) {
    int source_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    if (argc != 3) {
        print_error("Usage: cp file_from file_to\n", "", 97);
    }

    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        print_error("Can't read from file", argv[1], 98);
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (dest_fd == -1) {
        print_error("Can't write to file", argv[2], 99);
    }

    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            print_error("Write failed to file", argv[2], 99);
        }
    }

    if (bytes_read == -1) {
        print_error("Can't read from file", argv[1], 98);
    }

    if (close(source_fd) == -1) {
        print_error("Can't close fd", argv[1], 100);
    }

    if (close(dest_fd) == -1) {
        print_error("Can't close fd", argv[2], 100);
    }

    return 0;
}
