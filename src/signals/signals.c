/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:40:12 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/08 13:54:59 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* $? : 
		- Besoin de valeur de retour de chaque process enfant termine (0 = reussite, 1 = erreur)
		- variable globale qui renvoie comme variable d'enviro?
*/

void	sigint_handler(int sig) // print "^C" et rend nouvelle ligne de prompt
{
	g_received_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line(); //signifier qu'une nouvelle ligne va commencer
	rl_redisplay(); // redessiner la ligne apres une modif par ex
}

void	sigint_parent_handler(int sig)
{
	g_received_signal = sig;
	printf("\n");
}

void	set_signal_handlers()
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;
	struct sigaction	sa_sigtstp;

	sa_sigint.sa_handler = sigint_handler; // peut aussi etre defini sur SIG_IGN ou SIG_DFL
	sa_sigint.sa_flags = 0; // aucun flag particulier
	sigemptyset(&sa_sigint.sa_mask); // aucun signal a bloquer au depart
	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigtstp.sa_handler = SIG_IGN;
	sa_sigtstp.sa_flags = 0;
	sigemptyset(&sa_sigtstp.sa_mask);
	if (sigaction(SIGINT, &sa_sigint, NULL) == -1) //ctrl+C : nouvelle ligne de prompt
		exit(EXIT_FAILURE);
	if (sigaction(SIGQUIT, &sa_sigquit, NULL) == -1)  //ctrl+barre : rien dans process parent, Quit(core dumped) dans enfant
		exit(EXIT_FAILURE);
	if (sigaction(SIGTSTP, &sa_sigtstp, NULL) == -1)  //ctrl+Z a ignorer
		exit(EXIT_FAILURE);
}
