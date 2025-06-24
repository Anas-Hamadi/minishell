#include "parse.h"

// echo 'start     $USER' | grep 's' > output | cat << EOF | wc -l | echo " $USER" > file | echo "add  this" >> out | cat < out

int main(void)
{
    char    *input;
    t_cmdnode *cmd_list;

    // Enable command history
    using_history();

    while (1)
    {
        // Display prompt and read a line
        input = readline("minishell$ ");
        if (!input)  // EOF (Ctrl-D)
            break;

        // Skip empty commands
        if (*input)
            add_history(input);
        else
        {
            free(input);
            continue;
        }

        // Parse the command line into structures
        cmd_list = parse_command_line(input);
        if (cmd_list)
        {
            // Print the parsed structure for debugging
            print_cmd_structure(cmd_list);
            
            // TODO: Here you would execute the command structure
            // execute_pipeline(cmd_list);
            
            // Free the parsed structures
            free_cmd_list(cmd_list);
        }

        free(input);
    }

    printf("exit\n");
    return (EXIT_SUCCESS);
}
