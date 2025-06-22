#include "parse.h"

// echo 'start     $USER' | grep 's' > output | cat << EOF | wc -l | echo " $USER" > file | echo "add  this" >> out | cat < out

static void trim_trailing(char *s)
{
    char *end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\t'))
        *end-- = '\0';
}

int	main(void)
{
	// char	*input;
	char	input[1024];
	char	*cmd;
	char	*word;

	printf("Enter command: ");
	if (!fgets(input, sizeof(input), stdin))
		return (EXIT_FAILURE);
	// input = readline("feed me a command: ");

	size_t len = strlen(input);
    if (len > 0 && (input[len - 1] == '\n'))
		input[len - 1] = '\0';
		
	cmd = input;
	while (*cmd)
	{
		skip_spaces(&cmd);

		if (*cmd == '\0')
			break;

		if (detect_invalid_metachar(*cmd))
        {
            fprintf(stderr, "Error: invalid metacharacter '%c' at position %ld\n", *cmd, cmd - input);
            return (EXIT_FAILURE);
        }

		 // Recognize pipe token
        if (*cmd == '|')
        {
            printf("Token: [|]\n");
            cmd++;
            continue;
        }

		 // Recognize redirections
        if (*cmd == '>' || *cmd == '<')
        {
            char op = *cmd++;
            if (*cmd == op)
            {
                printf("Token: [%c%c]\n", op, op);
                cmd++;
            }
            else
            {
                printf("Token: [%c]\n", op);
            }
            continue;
        }

		// // Skip allowed delimiters (pipes, redirects) -- TODO: handle these in proper parsing
        // if (is_metachar(*cmd))
        // {
        //     cmd++;  // advance past '|' '<' '>'
        //     continue;
        // }

		word = handle_word(&cmd);
		if (!word)
		{
			printf("Error: failed to parse word\n");
			return (EXIT_FAILURE);
		}

		trim_trailing(word);
		printf("Parsed word: [%s]\n", word);
		free(word);
	}

	return (EXIT_SUCCESS);
}
