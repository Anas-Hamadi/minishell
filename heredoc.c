#include "parse.h"

static int  g_hdoc_count = 0;

static char *heredoc_generate_filename(void)
{
    char *fn = malloc(64);
    if (!fn) return NULL;
    snprintf(fn, 64, "/tmp/.heredoc_%d_%d", (int)getpid(), g_hdoc_count++);
    return fn;
}

int handle_heredoc(const char *delimiter, int expand, char **out_filename)
{
    char *fname = heredoc_generate_filename();
    char *line;
    int   fd;

    if (!fname || !delimiter || !out_filename)
        return -1;

    fd = open(fname, O_CREAT|O_WRONLY|O_TRUNC, 0600);
    if (fd < 0) { free(fname); return -1; }

    while (1)
    {
        line = readline("> ");
        if (!line)       /* user pressed Ctrl+D */
            break;
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        if (expand)
        {
			line = handle_hd_line(&line);
		}
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    *out_filename = fname;
    return 0;
}