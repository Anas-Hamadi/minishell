#include "minishell.h"

void add_temp_file(t_shell *shell, const char *filename)
{
	char **new_array;
	int new_capacity;

	if (!filename || !shell)
		return;

	if (shell->temp_count >= shell->temp_capacity)
	{
		new_capacity = shell->temp_capacity == 0 ? 4 : shell->temp_capacity * 2;
		new_array = realloc(shell->temp_files, sizeof(char *) * new_capacity);
		if (!new_array)
			return;
		shell->temp_files = new_array;
		shell->temp_capacity = new_capacity;
	}

	shell->temp_files[shell->temp_count] = strdup(filename);
	if (shell->temp_files[shell->temp_count])
		shell->temp_count++;
}

void cleanup_temp_files(t_shell *shell)
{
	int i;

	if (!shell || !shell->temp_files)
		return;

	i = 0;
	while (i < shell->temp_count)
	{
		if (shell->temp_files[i])
		{
			unlink(shell->temp_files[i]);
			free(shell->temp_files[i]);
		}
		i++;
	}

	free(shell->temp_files);
	shell->temp_files = NULL;
	shell->temp_count = 0;
	shell->temp_capacity = 0;
}

void remove_temp_file(t_shell *shell, const char *filename)
{
	int i;

	if (!filename || !shell || !shell->temp_files)
		return;

	i = 0;
	while (i < shell->temp_count)
	{
		if (shell->temp_files[i] && strcmp(shell->temp_files[i], filename) == 0)
		{
			unlink(shell->temp_files[i]);
			free(shell->temp_files[i]);
			shell->temp_files[i] = NULL;
			break;
		}
		i++;
	}
}
