/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:51:57 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/13 20:48:59 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	cwd[PATH_MAX];
	size_t	i;

	i = 1;
	while (i < cmd->nb_arg && cmd->args[i][0] == '-')
	{
		if (cmd->args[i][0] == '-')
		{
			printf("minislay: pwd: %s: invalid option\n", cmd->args[i]);
			return;
		}
		i++;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}
