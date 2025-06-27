#include "parse.h"

// echo 'start     $USER' | grep 's' > output | cat << EOF | wc -l | echo " $USER" > file | echo "add  this" >> out | cat < out

static void trim_trailing(char *s)
{
    char *end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\t'))
        *end-- = '\0';
}

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
	bool	expand_in_hd = 1;

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

        cmd = input;
        while (*cmd)
        {
            skip_spaces(&cmd);
            if (!*cmd)
                break;

            // Error on unsupported metacharacters
            if (detect_invalid_metachar(*cmd))
            {
                fprintf(stderr, "Error: invalid char '%c' at pos %ld\n",
                        *cmd, cmd - input);
                break;
            }
			
			// heredoc?
			if (*cmd == '<' && *(cmd+1) == '<')
			{
				int   expand = 1;
				char  quote = 0;
				char *delim;
				char *tmpfile;

				cmd += 2;
				skip_spaces(&cmd);
				if (!*cmd)
				{
					fprintf(stderr, "syntax error: expected heredoc delimiter\n");
					break;
				}

				// quoted delimiter disables expansion
				if (*cmd=='\''||*cmd=='"')
				{
					// dont expand if delimiter is/has a var
					// 👇in handle_quote_block, needs a param to let it know to not expand var
					delim = handle_quote_block(&cmd, &quote, 1);
					/* if (handle_quote_block(&cmd, &quote))
						expand = 0;
					*/
					
					expand = !(quote == '"' || quote == '\'');
				}
				else
				{
					delim = handle_word(&cmd, 1, &expand_in_hd);
					expand = expand_in_hd;
				}
					// ☝️in handle_word, needs a param to let it know to not expand var

				if (!delim)
				{
					fprintf(stderr, "syntax error: bad heredoc delimiter\n");
					break;
				}
				if (handle_heredoc(delim, expand, &tmpfile) < 0)
				{
					perror("heredoc");
					free(delim);
					break;
				}

				// record as an input redirection into your cmdnode
				printf("HEREDOC → infile=%s (delim='%s', expand=%d)\n",
					tmpfile, delim, expand);
				free(delim);
				free(tmpfile);
				continue;
			}

			// simple single/double char redirs (>, >>, <)
			if (*cmd == '>' || *cmd == '<')
			{
				char op = *cmd++;
				int  type = (op=='>' ? R_OUT : R_IN);

				// append?
				if (*cmd == op)
				{
					type = R_APPEND;
					cmd++;
				}

				skip_spaces(&cmd);
				if (!*cmd)
				{
					fprintf(stderr, "syntax error: expected filename after '%c'\n",
							(type==R_APPEND? '>' : op));
					break;
				}

				// filename may be quoted or unquoted
				char quote = 0;
				char *fname;
				if (*cmd=='\''||*cmd=='"')
					fname = handle_quote_block(&cmd, &quote, 0);
				else
					fname = handle_word(&cmd, 0, &expand_in_hd);

				if (!fname)
				{
					fprintf(stderr, "syntax error: bad filename\n");
					break;
				}

				printf("REDIR[%d] → %s\n", type, fname);
				free(fname);
				continue;
			}

            // // Handle redirections
            // if (*cmd == '>' || *cmd == '<')
            // {
            //     char op = *cmd++;
            //     if (*cmd == op)
            //     {
            //         printf("Token: [%c%c]\n", op, op);
            //         cmd++;
            //     }
            //     else
            //         printf("Token: [%c]\n", op);
            //     continue;
            // }

			// Handle pipes
            if (*cmd == '|')
            {
                printf("Token: [|]\n");
                cmd++;
                continue;
            }

            // Parse a word
            word = handle_word(&cmd, 0, &expand_in_hd);
            if (!word)
            {
                fprintf(stderr, "Parse error at pos %ld\n", cmd - input);
                break;
            }
            trim_trailing(word);
            printf("Word: [%s]\n", word);
            free(word);
        }

        free(input);
    }

    printf("exit\n");
    return (EXIT_SUCCESS);
}


// t_cmdnode main_func(void)
// {
//     char    *input;
//     char    *cmd;
//     char    *word;
// 	bool	expand_in_hd = 1;

//     // Enable command history
//     using_history();

//     while (1)
//     {
//         // Display prompt and read a line
//         input = readline("minishell$ ");
//         if (!input)  // EOF (Ctrl-D)
//             break;

//         // Skip empty commands
//         if (*input)
//             add_history(input);
//         else
//         {
//             free(input);
//             continue;
//         }

//         cmd = input;
//         while (*cmd)
//         {
//             skip_spaces(&cmd);
//             if (!*cmd)
//                 break;

//             // Error on unsupported metacharacters
//             if (detect_invalid_metachar(*cmd))
//             {
//                 fprintf(stderr, "Error: invalid char '%c' at pos %ld\n",
//                         *cmd, cmd - input);
//                 break;
//             }
			
// 			// heredoc?
// 			if (*cmd == '<' && *(cmd+1) == '<')
// 			{
// 				int   expand = 1;
// 				char  quote = 0;
// 				char *delim;
// 				char *tmpfile;

// 				cmd += 2;
// 				skip_spaces(&cmd);
// 				if (!*cmd)
// 				{
// 					fprintf(stderr, "syntax error: expected heredoc delimiter\n");
// 					break;
// 				}

// 				// quoted delimiter disables expansion
// 				if (*cmd=='\''||*cmd=='"')
// 				{
// 					// dont expand if delimiter is/has a var
// 					// 👇in handle_quote_block, needs a param to let it know to not expand var
// 					delim = handle_quote_block(&cmd, &quote, 1);
// 					expand = !(quote == '"' || quote == '\'');
// 				}
// 				else
// 				{
// 					delim = handle_word(&cmd, 1, &expand_in_hd);
// 					expand = expand_in_hd;
// 				}
// 					// ☝️in handle_word, needs a param to let it know to not expand var

// 				if (!delim)
// 				{
// 					fprintf(stderr, "syntax error: bad heredoc delimiter\n");
// 					break;
// 				}
// 				if (handle_heredoc(delim, expand, &tmpfile) < 0)
// 				{
// 					perror("heredoc");
// 					free(delim);
// 					break;
// 				}

// 				// record as an input redirection into your cmdnode
// 				printf("HEREDOC → infile=%s (delim='%s', expand=%d)\n",
// 					tmpfile, delim, expand);
// 				free(delim);
// 				free(tmpfile);
// 				continue;
// 			}

// 			// simple single/double char redirs (>, >>, <)
// 			if (*cmd == '>' || *cmd == '<')
// 			{
// 				char op = *cmd++;
// 				int  type = (op=='>' ? R_OUT : R_IN);

// 				// append?
// 				if (*cmd == op)
// 				{
// 					type = R_APPEND;
// 					cmd++;
// 				}

// 				skip_spaces(&cmd);
// 				if (!*cmd)
// 				{
// 					fprintf(stderr, "syntax error: expected filename after '%c'\n",
// 							(type==R_APPEND? '>' : op));
// 					break;
// 				}

// 				// filename may be quoted or unquoted
// 				char quote = 0;
// 				char *fname;
// 				if (*cmd=='\''||*cmd=='"')
// 					fname = handle_quote_block(&cmd, &quote, 0);
// 				else
// 					fname = handle_word(&cmd, 0, &expand_in_hd);

// 				if (!fname)
// 				{
// 					fprintf(stderr, "syntax error: bad filename\n");
// 					break;
// 				}

// 				printf("REDIR[%d] → %s\n", type, fname);
// 				free(fname);
// 				continue;
// 			}

// 			// Handle pipes
//             if (*cmd == '|')
//             {
//                 printf("Token: [|]\n");
//                 cmd++;
//                 continue;
//             }

//             // Parse a word
//             word = handle_word(&cmd, 0, &expand_in_hd);
//             if (!word)
//             {
//                 fprintf(stderr, "Parse error at pos %ld\n", cmd - input);
//                 break;
//             }
//             trim_trailing(word);
//             printf("Word: [%s]\n", word);
//             free(word);
//         }

//         free(input);
//     }

//     printf("exit\n");
//     return (EXIT_SUCCESS);
// }