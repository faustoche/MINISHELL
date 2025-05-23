/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:40:12 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/17 10:25:19 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_prompt(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		g_received_signal = SIGINT;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	child_new_prompt(int sig)
{
	g_received_signal = sig;
	printf("\n");
}

void	close_stdin(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = sig;
		printf("\n");
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
}

int	handle_termsig(int printed_signal, int status)
{
	if (!printed_signal)
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			g_received_signal = SIGQUIT;
		}
		else if (WTERMSIG(status) == SIGINT)
			g_received_signal = SIGINT;
		printed_signal = 1;
	}
	return (printed_signal);
}

void	handle_signals(int sig, int param)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	if (param == IGNORE)
		sa.sa_handler = SIG_IGN;
	else if (param == DEFAULT)
		sa.sa_handler = SIG_DFL;
	else if (param == PROMPT)
		sa.sa_handler = new_prompt;
	else if (param == CHILD_PROMPT)
		sa.sa_handler = child_new_prompt;
	else if (param == CLOSE_IN)
		sa.sa_handler = close_stdin;
	else
		return ;
	if (sigaction(sig, &sa, NULL) == -1)
		perror("sigaction failed");
}
