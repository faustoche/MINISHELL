/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:40:12 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/11 13:08:18 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_signals(int sig, int param)
{
	struct sigaction	sa;
	
	ft_bzero(&sa, sizeof(sa));
	if (param == IGNORE)
		sa.sa_handler = SIG_IGN;
	else if (param == DEFAULT)
		sa.sa_handler = SIG_DFL;
	else if (param == PROMPT)
		sa.sa_handler = new_prompt;
	else if (param == WESH)
		sa.sa_handler = sigint_parent_handler;
	else if (param == CLOSE_IN)
		sa.sa_handler = close_stdin;
	else
	{
		if (sig == SIGINT)
			g_received_signal = SIGINT;
	}
	if (sigaction(sig, &sa, NULL) == -1)
		return (-1);
	return (0);
}
void	new_prompt(int sig)
{
	if (g_received_signal != 1)
	{
		printf("\n");
		g_received_signal = sig;
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_parent_handler(int sig)
{
	g_received_signal = sig;
	printf("\n");
}

void	close_stdin(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = 130;
		printf("\n");
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
}

void	sigint_heredoc_handler(int sig)
{
	g_received_signal = sig;
	printf("\n");
}

void	set_signal_handlers(void)
{
	handle_signals(SIGINT, PROMPT);
	handle_signals(SIGQUIT, IGNORE);
	handle_signals(SIGTSTP, IGNORE);
}
