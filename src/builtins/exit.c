/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:14:36 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/19 11:51:57 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
- le code de sortie doit obligatoirement être un chiffre car c'est le code
	retour que exit attend
- Pourquoi atoi ? Les codes de sortie linux sont limités à 8 bits (0-255)
- Si l'utilisateur entre une valeur en dehors de ça (exemple : exit 1000)
- Le shell doit ramener cette valeur dans la plage 0-255
- 1000 % 256 = 232 donc exit 1000 = exit 232
*/

void	ft_exit(t_cmd *cmd)
{
	int		exit_code;

	printf("Adieu 🪦\n");
	exit_code = 0;
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]))
		{
			printf("minislay : exit: %s: numeric argument required\n", cmd->args[1]);
			exit(255);
		}
		else if (cmd->args[2])
		{
			printf("minislay : exit: too many arguments\n");
			return ;
		}
		else
			exit_code = ft_atoi(cmd->args[1]) % 256;
	}
	exit(exit_code);
	// ici ajouter une fonction pour quitter et clean le programme
}
