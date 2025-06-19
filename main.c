#include <parse.h>

// echo 'start     $USER' | grep 's' > output | cat << EOF | wc -l | echo " $USER" > file | echo "add  this" >> out | cat < out

// Declare your helpers
void	skip_spaces(char **cmd);
int		detect_invalid_metachar(char c);
char	*handle_word(char **cmd);
char	*expand_variable(char **cmd);
char	*handle_quote_block(char **cmd, char *quote_context);
int		get_last_exit_status(void);

int	main(void)
{
	char	input[1024];
	char	*cmd;
	char	*word;

	printf("Enter command: ");
	if (!fgets(input, sizeof(input), stdin))
		return (1);

	cmd = input;

	while (*cmd)
	{
		skip_spaces(&cmd);

		if (*cmd == '\0')
			break;

		if (detect_invalid_metachar(*cmd))
		{
			printf("Error: invalid metacharacter: %c\n", *cmd);
			return (1);
		}

		word = handle_word(&cmd);
		if (!word)
		{
			printf("Error: failed to parse word\n");
			return (1);
		}

		printf("Parsed word: [%s]\n", word);
		free(word);
	}

	return (0);
}
