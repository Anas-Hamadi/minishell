#include "minishell.h"

static char **g_temp_files = NULL;
static int g_temp_count = 0;
static int g_temp_capacity = 0;

void	add_temp_file(const char *filename)
{
	char	**new_array;
	int		new_capacity;

	if (!filename)
		return;

	if (g_temp_count >= g_temp_capacity)
	{
		new_capacity = g_temp_capacity == 0 ? 4 : g_temp_capacity * 2;
		new_array = realloc(g_temp_files, sizeof(char *) * new_capacity);
		if (!new_array)
			return;
		g_temp_files = new_array;
		g_temp_capacity = new_capacity;
	}

	g_temp_files[g_temp_count] = strdup(filename);
	if (g_temp_files[g_temp_count])
		g_temp_count++;
}

void	cleanup_temp_files(void)
{
	int	i;

	if (!g_temp_files)
		return;

	i = 0;
	while (i < g_temp_count)
	{
		if (g_temp_files[i])
		{
			unlink(g_temp_files[i]);
			free(g_temp_files[i]);
		}
		i++;
	}

	free(g_temp_files);
	g_temp_files = NULL;
	g_temp_count = 0;
	g_temp_capacity = 0;
}

void	remove_temp_file(const char *filename)
{
	int	i;

	if (!filename || !g_temp_files)
		return;

	i = 0;
	while (i < g_temp_count)
	{
		if (g_temp_files[i] && strcmp(g_temp_files[i], filename) == 0)
		{
			unlink(g_temp_files[i]);
			free(g_temp_files[i]);
			g_temp_files[i] = NULL;
			break;
		}
		i++;
	}
}
