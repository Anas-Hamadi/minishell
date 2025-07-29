#include "minishell.h"

volatile sig_atomic_t g_signal_num = 0;

void signal_handler_interactive(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	g_signal_num = sig;
}

void signal_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_signal_num = sig;
		// Don't call readline functions in heredoc context
	}
}

void signal_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "^\\Quit (core dumped)\n", 20);
	}
	g_signal_num = sig;
}

void setup_signals_interactive(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler_interactive;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN); // Ignore Ctrl-\ in interactive mode
}

void setup_signals_heredoc(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // No restart for heredoc

	sigaction(SIGINT, &sa, NULL); // Ctrl-C should interrupt
	signal(SIGQUIT, SIG_IGN);	  // Ignore Ctrl-\ in heredoc
}

void setup_signals_child(void)
{
	struct sigaction sa;

	// Setup custom handler for child processes
	sa.sa_handler = signal_handler_child;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

// void check_signal_interactive(void)
// {
// 	if (g_signal_num == SIGINT)
// 	{
// 		// Signal already handled in signal_handler_interactive
// 		// Just reset the flag
// 		g_signal_num = 0;
// 	}
// }
