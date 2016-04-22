#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define DEFAULT_NUMBER 10
#define USAGE_STRING "usage: head [-n#] [-#] [filename...]\n"

#define NEWLINE 0x0A
#define SEP ": "

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

static void usage() {
    write(STDERR_FILENO, USAGE_STRING, sizeof USAGE_STRING);
}

static void print_newline() {
    write(STDOUT_FILENO, "\n", 1);
}

static int error(const char *str, int err) {
    char *errstr = strerror(err);
    write(STDERR_FILENO, str, strlen(str));
    write(STDERR_FILENO, SEP, sizeof SEP);
    write(STDERR_FILENO, errstr, strlen(errstr));
    write(STDERR_FILENO, "\n", 1);
    return err;
}

static void print_sep(char *filename) {
    write(STDOUT_FILENO, "==> ", 4);
    write(STDOUT_FILENO, filename, strlen(filename));
    write(STDOUT_FILENO, " <==\n", 5);
}

int main (int argc, char **argv) {
	int cnt = DEFAULT_NUMBER, fd, ch;
    int index;
    int first = 1, print_headers = 0;
    while ((ch = getopt(argc, argv, "n:")) != -1)
        switch(ch) {
        case 'n':
            if(optarg == NULL) {
                return error(optarg, errno);
            }
            if (atoi(optarg) < 0) {
                return error(optarg, EINVAL);
            }
            cnt = atoi(optarg);
            break;
        case '?':
        default:
            usage();
            return 1;
    }

    if((argc - optind) > 1) print_headers = 1;

    for (index = optind; index < argc; index++) {
        fd = open(argv[index], O_RDONLY);
        if(fd == -1) {
            return error(argv[index], errno);
        }

        if(print_headers) {
            print_sep(argv[index]);
        }

        head (fd, cnt);
        close (fd);
    }
	return 0;
}
