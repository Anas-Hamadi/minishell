// heredoc_handler.c
// Standalone test for heredoc parsing and file creation

#include "parse.h"

// Generate a unique temp file for heredoc content
static char *generate_tempfile(void)
{
    char template[] = "/tmp/minishell_heredoc_XXXXXX";
    int fd = mkstemp(template);
    if (fd < 0)
        return NULL;
    close(fd);
    return strdup(template);
}

// Read heredoc lines into a temp file until delimiter is matched
// expand: if true, perform variable expansion
char *read_heredoc(const char *delimiter, int expand)
{
    (void)expand; // Suppress unused parameter warning for now
    char *filename = generate_tempfile();
    if (!filename)
    {
        perror("mkstemp");
        return NULL;
    }
    int fd = open(filename, O_WRONLY);
    if (fd < 0)
    {
        perror("open temp heredoc");
        free(filename);
        return NULL;
    }

    while (1)
    {
        char *line = readline("heredoc> ");
        if (!line)
        {
            // EOF (Ctrl-D)
            break;
        }
        // Stop on exact delimiter match
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        // TODO: if (expand) perform variable expansion on 'line'
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    return filename;
}

// Test harness
int main(int argc, char **argv)
{
    const char *delim;
    int expand = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <DELIMITER> [expand]\n", argv[0]);
        return EXIT_FAILURE;
    }
    delim = argv[1];
    if (argc >= 3 && strcmp(argv[2], "expand") == 0)
        expand = 1;

    char *file = read_heredoc(delim, expand);
    if (!file)
        return EXIT_FAILURE;
    printf("Heredoc content written to: %s\n", file);
    printf("=== File Preview ===\n");

    // Show file contents
    FILE *f = fopen(file, "r");
    if (!f)
    {
        perror("fopen");
        free(file);
        return EXIT_FAILURE;
    }
    char buf[1024];
    while (fgets(buf, sizeof(buf), f))
        fputs(buf, stdout);
    fclose(f);

    // Cleanup
    unlink(file);
    free(file);
    return EXIT_SUCCESS;
}
