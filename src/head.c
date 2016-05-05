#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_NUMBER 10
#define USAGE_STRING "usage: head [-n #] [filename...]\n"

static int error(const char *str, int err) {
    char *errstr = strerror(err);
    write(STDERR_FILENO, str, strlen(str));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, errstr, strlen(errstr));
    write(STDERR_FILENO, "\n", 1);
    return err;
}

static void head(int filde, int cnt) {
    int BUFSIZE = sysconf(_SC_PAGESIZE);
	char buffer[BUFSIZE];
	off_t nread;
    int i;

	while (cnt && (nread = read(filde, buffer, BUFSIZE)) != NULL ) {

        if (nread < 0) {
            error("head", errno);
            break;
        }
        
        for (i = 0; i < nread; ++i) {
            if(!cnt) break;
            if (buffer[i] == '\n') cnt--;
        }
        
        write (STDOUT_FILENO, buffer, i);
	}
}

static void usage() {
    write(STDERR_FILENO, USAGE_STRING, sizeof USAGE_STRING);
    exit(EXIT_FAILURE);
}

static void print_sep(char *filename) {
    write(STDOUT_FILENO, "==> ", 4);
    write(STDOUT_FILENO, filename, strlen(filename));
    write(STDOUT_FILENO, " <==\n", 5);
}

int main (int argc, char **argv) {
	int cnt = DEFAULT_NUMBER, fd = 0, ch, exitval = EXIT_SUCCESS;
    int first, print_headers = 0;
    char *ep;

    while ((ch = getopt(argc, argv, "n:")) != -1)
        switch(ch) {
        case 'n':        
            cnt = strtol(optarg, &ep, 10);
            if (*ep || cnt <= 0) {
                error(optarg, EINVAL);
                usage();
            }
            break;
        case '?':
        default:
            usage();
    }

    if((argc - optind) > 1) print_headers = 1;

    argv += optind;
    argc -= optind;

    if (*argv) {
        for (first = 1; *argv; ++argv) {
            if ((fd = open(*argv, O_RDONLY | O_LARGEFILE))==-1) {
                error(*argv, errno);
                exitval = EXIT_FAILURE;
                continue;
            } else if (print_headers) { 
                if (!first) write(STDOUT_FILENO, "\n", 1); 
                print_sep(*argv);
            }
            first = 0;

            head (fd, cnt);
            if (close(fd)) {
                error("head", errno);
                exitval = EXIT_FAILURE;
                continue; 
            }
        }
    } else {
        head(STDIN_FILENO, cnt);
    }
	return exitval;
}

