/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_cmd1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 08:42:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the command */

void	ft_memdel(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	free_cmd_var(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->in)
		ft_memdel(cmd->in);
	if (cmd->out)
		ft_memdel(cmd->out);
	if (cmd->heredoc_eof)
		ft_memdel(cmd->heredoc_eof);
}

void	free_single_command(t_cmd *cmd, int **last_exit_status_ptr)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			ft_memdel(cmd->args[i]);
			i++;
		}
		ft_memdel(cmd->args);
	}
	free_cmd_var(cmd);
	if (cmd->exit_status)
	{
		if (!*last_exit_status_ptr || *last_exit_status_ptr != cmd->exit_status)
		{
			*last_exit_status_ptr = cmd->exit_status;
			free(cmd->exit_status);
		}
	}
	ft_memdel(cmd);
}

void	free_commands(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		*last_exit_status;

	last_exit_status = NULL;
	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next;
		free_single_command(cmd, &last_exit_status);
		cmd = tmp;
	}
}

char	*ft_realloc(char *str, size_t size)
{
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = malloc(size);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	if (!str)
	{
		new_str[0] = '\0';
		return (new_str);
	}
	ft_strncpy(new_str, str, size - 1);
	new_str[size - 1] = '\0';
	free(str);
	return (new_str);
}
