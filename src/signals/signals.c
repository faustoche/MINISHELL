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

static void	sigint_handler() // print "^C" et rend nouvelle ligne de prompt
{
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line(); //signifier qu'une nouvelle ligne va commencer
	rl_redisplay(); // redessiner la ligne apres une modif par ex
}

/* si process enfant en cours, ne fait rien (ex: "sleep 5").
	si process enfant qui attend arg, exit l'enfant et retourne ds parent (rend le prompt) (ex : cat)
	si process parent (et ligne vide), exit minishell */
static void	eof_handler() // si prompt vide exit le shell
{
	printf("exit\n");
	exit(0);
}

void	check_signals()
{
	signal(SIGINT, sigint_handler); //ctrl+C : nouvelle ligne de prompt
	signal(EOF, eof_handler); //Ctrl+D : exit minislay
	signal(SIGQUIT, SIG_IGN); //ctrl+barre : rien
	signal(SIGTSTP, SIG_IGN); //ctrl+Z a ignorer
}
