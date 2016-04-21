#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEFAULT_NUMBER 10
#define USAGE_STRING "usage: head [-number | -n number] [filename...]"

#define NEWLINE 0x0A

static void head(int filde, int cnt) {
	char *buffer;
	size_t nread;

	while (cnt && (nread = read(filde, buffer, sizeof(char))) != NULL ) {
		write(STDOUT_FILENO, buffer, nread);
		if(*buffer == NEWLINE) {
			cnt--;
		}
	}
}

int main (int argc, char **argv) {
	int fd;
	int cnt = DEFAULT_NUMBER;
	cnt = 3;

	fd = open("lorem", O_RDONLY);
	head(fd, cnt);
	close(fd);	
	return 0;
}
