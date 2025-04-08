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

/*
Si SIGQUIT recu dans process enfant, SIG_DFL au lieu de SIG_INT.
cat


^\Quit (core dumped)

*/


/* $? : 
		- Besoin de valeur de retour de chaque process enfant termine (0 = reussite, 1 = erreur)
		- variable globale qui renvoie comme variable d'enviro?
*/

void	sigquit_child_handler(int sig) // si le pid est celui du parent, return. Sinon ...
{
	dprintf(2, "%s, %d\n", __FILE__, __LINE__);
	g_received_signal = sig;
	printf("^\\Quit (core dumped)\n");
	dprintf(2, "%s, %d\n", __FILE__, __LINE__);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_handler(int sig) // print "^C" et rend nouvelle ligne de prompt
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line(); //signifier qu'une nouvelle ligne va commencer
	rl_redisplay(); // redessiner la ligne apres une modif par ex
}

void	set_signal_handlers()
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit_parent;
	struct sigaction	sa_sigtstp;

	//Initialisation de la structure sigaction.
	sa_sigint.sa_handler = sigint_handler; // peut aussi etre defini sur SIG_IGN ou SIG_DFL
	sa_sigint.sa_flags = 0; // aucun flag particulier
	sigemptyset(&sa_sigint.sa_mask); // aucun signal a bloquer au depart

	sa_sigquit_parent.sa_handler = SIG_IGN;
	sa_sigquit_parent.sa_flags = 0;
	sigemptyset(&sa_sigquit_parent.sa_mask);

	sa_sigtstp.sa_handler = SIG_IGN;
	sa_sigtstp.sa_flags = 0;
	sigemptyset(&sa_sigtstp.sa_mask);


	//Attacher gestionnaire a chaque signal

	if (sigaction(SIGINT, &sa_sigint, NULL) == -1) //ctrl+C : nouvelle ligne de prompt
	{
		perror("Error handling SIGINT");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGQUIT, &sa_sigquit_parent, NULL) == -1)  //ctrl+barre : rien dans process parent, Quit(core dumped) dans enfant
	{
		perror("Error handling SIGQUIT");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTSTP, &sa_sigtstp, NULL) == -1)  //ctrl+Z a ignorer
	{
		perror("Error handling SIGTSTP");
		exit(EXIT_FAILURE);
	}
	// signal(SIGINT, sigint_handler); 
	// signal(SIGQUIT, sigquit_handler);
	// signal(SIGTSTP, SIG_IGN);
}
