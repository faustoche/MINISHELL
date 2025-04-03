/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/03 15:03:29 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the command */

void free_commands(t_cmd *cmd)
{
    t_cmd *tmp;
    int i;

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
                free(cmd->args[i]);
                i++;
            }
            free(cmd->args);
        }
        free(cmd);
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

void	quit_minislay(char *line, t_cmd *cmd, t_token *token, t_env *env)
{
	t_token	*tmp;
	
	free(line);
	if (cmd)
		free_commands(cmd);
	while (token)
	{
		tmp = token->next;
		free(token);
		token = tmp;
	}
	free_env_list(&env);
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


//fonction qui imprime la taille de la liste 
