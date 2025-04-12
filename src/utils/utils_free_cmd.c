/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/12 15:56:54 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the command */

void ft_memdel(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	free_commands(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next;
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
		if (cmd->in)
			ft_memdel(cmd->in);
		if (cmd->out)
			ft_memdel(cmd->out);
		if (cmd->heredoc_eof)
			ft_memdel(cmd->heredoc_eof);
		if (cmd->exit_status)
			ft_memdel(cmd->exit_status);
		ft_memdel(cmd);
		cmd = tmp;
	}
}

void	free_tab(char **input)
{
	int	i;

	i = 0;
	if (!input)
		return ;
	while (input[i])
	{
		free(input[i]);
		i++;
	}
	free(input);
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

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}
