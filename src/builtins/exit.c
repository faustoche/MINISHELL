/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:14:36 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/19 08:44:03 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_cmd *cmd)
{
	char	*msg;
	int		exit_code;
	
	msg = "exit\nAdieu\n";
	write(1, msg, ft_strlen(msg));
	exit_code = 0; // code de sortie par défault
	if (cmd->args[1])
	{
		if (is_numeric(cmd->args[1]))
		{
			printf("minislay: trop de chiffres, verifier vrai message"); // le code de sortie doit obligatoirement etre un chiffre car c'est ce que exit attned
			exit(255); // l'exit code de base
		}
		else if (cmd->args[2])
		{
			printf("erreur trop d'arguments");
			return ;
		}
		else
			exit_code = ft_atoi(cmd->args[1]) % 256;
	}
	exit(exit_code);
}

// pourquoi atoi ? les codes de sortie linux sont limités à 8 bits (0-255)
// si l'utilisateur entre une valeur en dehors de ca (exit 1000)
// LE SHELL DOIT RAmener cette valeur dans la plage 0-255
// 1000 % 256 = 232 donc exit 1000 = exit 232