/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:40:12 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/01 15:55:53 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Si SIGQUIT recu dans process enfant, SIG_DFL au lieu de SIG_INT.
cat


^\Quit (core dumped)

*/


/* $? : 
		- Besoin de valeur de retour de chaque process enfant termine (0 = reussite, 1 = erreur)
		- variable globale qui renvoie comme variable d'enviro?
*/

void	sigquit_handler(int sig) // print "^C" et rend nouvelle ligne de prompt
{
	g_received_signal = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	sigint_handler(int sig) // print "^C" et rend nouvelle ligne de prompt
{
	g_received_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line(); //signifier qu'une nouvelle ligne va commencer
	rl_redisplay(); // redessiner la ligne apres une modif par ex
}

void	set_signal_handlers()
{
	signal(SIGINT, sigint_handler); //ctrl+C : nouvelle ligne de prompt
	signal(SIGQUIT, sigquit_handler); //ctrl+barre : rien
	signal(SIGTSTP, SIG_IGN); //ctrl+Z a ignorer
}
