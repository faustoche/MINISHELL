/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priorities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/10 20:17:47 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
&&

- && ne peut pas être en début de ligne ou après un autre opérateur
- Récupère le dernier bloc de commandes
- Indique que le prochain élément logique après ce bloc est AND_AND (&&).
- Réinitialise pour indiquer qu'un nouveau bloc doit être créé après cet opérateur.
- Incrémente index de 2 (car && a deux caractères).
- Ignore les espaces après && avec space_processor.
*/

/*
||

- Vérifie si une commande précède || (sinon, erreur).
- Récupère le dernier bloc et assigne OR_OR pour le prochain élément logique.
- Réinitialise et passe au prochain élément après ||.
*/


void	handle_priorities(t_token *token_list)
{
	int	i;

	i = 0;
	// tant que l'input[i]
		// if ()
			// alors creation d'une sous-expression
		// if (&& ou ||)
			// ce qui est avant = cmd->left
			// ce qui est apres = cmd->right
			// operator == token->type
		// i++
	// return ()
}

void	execute_priorities(t_cmd *cmd)
{
	// if (command groupée)
		// executer sous expression
	// else if (token->type == TOKEN_AND)
		// if (execute_cmd->left == succes)
			// return (execute command->right)
		// else
			// return (echec)
	// else if (token->type == TOKEN_OR)
		// if (executecmd(cmd->left) == echec)
			// return (execute cmd->right)
		// else
			// return (success)
	// sinon
		// execute commande normalement
}

void	create_prio_expression()
{
	
}



/*
Pipe ??

Vérifie que | n'est pas mal placé (ex: en début de ligne).
Si utils->in_block == 0, affiche une erreur.
Récupère le dernier t_block et assigne MY_PIPE (indique un pipeline).
Réinitialise utils->in_block pour forcer un nouveau bloc après |.
Avance index d'un seul caractère (+1 au lieu de +2 pour && et ||).
Ignore les espaces après |.
Vérification spéciale : si la fin de la chaîne (\0) est atteinte après |, cela signifie qu’il manque une commande après le pipeline → affiche une erreur.
*/

/*
Gère l’ouverture de parenthèses ( pour les sous-shells.

Fonctionnement :
Vérifie que ( n’apparaît pas au milieu d'une commande (utils->in_block == 1), sinon erreur.
Crée un nouveau bloc (next_block_creator).
Récupère ce dernier bloc et le configure en tant que sous-shell (SUBSH).
Initialise exec_argv avec une valeur spéciale ((void *)1), probablement pour signaler que ce n’est pas une commande simple.
Met à jour utils->parsh_block (pointeur vers l’ancien bloc de parsing) et p_p_block_arr pour gérer l’imbrication des sous-shells.
Désactive utils->in_block et passe à l’élément suivant.
5. parenthesis_close
Gère la fermeture de parenthèses ).

Fonctionnement :
Revient au bloc parent du sous-shell (utils->parsh_block).
Met à jour le pointeur p_p_block_arr pour sortir du niveau courant.
Active utils->in_block = 1, indiquant qu’une commande est complète et que de nouveaux opérateurs (&&, ||, etc.) peuvent suivre.
Résumé général
Fonction	Opérateur	Rôle principal
logical_and_processor	&&	Exécute la commande suivante si la précédente réussit.
logical_or_processor	`	
pipe_processor	`	`
parenthesis_open	(	Démarre un sous-shell.
parenthesis_close	)	Termine un sous-shell et revient au niveau précédent.
Ces fonctions assurent la structuration des commandes dans un shell minimaliste en gérant correctement les erreurs de syntaxe et la hiérarchie des commandes imbriquées.
*/