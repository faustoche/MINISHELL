/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/06 12:33:44 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect(int fd, int std_fd)
{
	if (dup2(fd, std_fd) == -1)
		perror("error : dup2 failed\n");
	close(fd);
}

/* According to the token, redirect in the right direction */

int	handle_redirection(t_token *token, t_cmd *current, t_cmd *head)
{
	if (!current || !token->next)
	{
		printf("Error: invalid redirection\n");
		free_commands(head);
		return (-1);
	}
	if (token->type == REDIR_OUT)
		return (redirection(current, token->next->value, 1, 0));
	else if (token->type == REDIR_IN)
		return (redirection(current, token->next->value, 0, 0));
	else if (token->type == REDIR_APPEND)
		return (redirection(current, token->next->value, 1, 1));
	else if (token->type == HEREDOC)
		return (handle_heredoc(current, token->next->value, head));
	return (0);
}

int	redirection(t_cmd *cmd, char *file, int out, int append)
{
	if (!cmd || !file)
		return (-1);
	if (out)
	{
		if (cmd->out)
			free(cmd->out);
		cmd->out = ft_strdup(file);
		if (!cmd->out)
			return (-1);
		cmd->append = append;
	}
	else
	{
		if (cmd->in)
			free(cmd->in);
		cmd->in = ft_strdup(file);
		if (!cmd->in)
			return (-1);
	}
	return (0);
}
