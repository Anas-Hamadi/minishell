#include "minishell.h"

int g_signal_num = 0;

void signal_handler(int sig)
{
	g_signal_num = sig;
}

void setup_signals_interactive(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN); // Ignore Ctrl-\ in interactive mode
}

void setup_signals_heredoc(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // No restart for heredoc

	sigaction(SIGINT, &sa, NULL); // Ctrl-C should interrupt
	signal(SIGQUIT, SIG_IGN);	  // Ignore Ctrl-\ in heredoc
}

void setup_signals_child(void)
{
	// Reset to default behavior for child processes
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void check_signal_interactive(void)
{
	if (g_signal_num == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_num = 0;
	}
}
