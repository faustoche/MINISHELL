/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/27 21:15:27 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the command */

void	free_commands(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->args)
		{
			while (tmp->args[i])
				free(tmp->args[i++]);
			free(tmp->args);
		}
		if (tmp->in)
			free(tmp->in);
		if (tmp->out)
			free(tmp->out);
		free(tmp);
	}
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	quit_minislay(char *line, t_cmd *cmd, t_token *token, t_env *env)
{
	free(cmd);
	free(line);
	while (token->next)
	{
		free(token->next);
		token = token->next;
	}
	free_env_list(env);
	clear_history();
}

void	clean_exit(t_token *tokens, char *input, t_cmd *commands)
{
	if (tokens)
		free_token_list(tokens);
	if (input)
		free(input);
	if (commands)
		free_commands(commands);
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
