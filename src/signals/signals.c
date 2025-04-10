/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:40:12 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/09 09:16:07 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* $? : 
		- Besoin de valeur de retour de chaque process enfant termine 
		(0 = reussite, 1 = erreur)
		- variable globale qui renvoie comme variable d'enviro?
*/

void	sigint_handler(int sig)
{
	g_received_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_parent_handler(int sig)
{
	g_received_signal = sig;
	//printf("\n");
}

void	set_signal_handlers(void)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;
	struct sigaction	sa_sigtstp;

	sa_sigint.sa_handler = sigint_handler;
	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);

	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);

	sa_sigtstp.sa_handler = SIG_IGN;
	sa_sigtstp.sa_flags = 0;
	sigemptyset(&sa_sigtstp.sa_mask);

	if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
		exit(EXIT_FAILURE);

	if (sigaction(SIGQUIT, &sa_sigquit, NULL) == -1)
		exit(EXIT_FAILURE);

	if (sigaction(SIGTSTP, &sa_sigtstp, NULL) == -1)
		exit(EXIT_FAILURE);
}
