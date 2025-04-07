/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/07 17:17:36 by fcrocq           ###   ########.fr       */
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
	int	result;

	if (!current || !token->next)
	{
		printf("Error: invalid redirection\n");
		free_commands(head);
		return (-1);
	}
	if (token->type == REDIR_OUT)
		result = redirection(current, token->next->value, 1, 0);
	else if (token->type == REDIR_IN)
		result = redirection(current, token->next->value, 0, 0);
	else if (token->type == REDIR_APPEND)
		result = redirection(current, token->next->value, 1, 1);
	else if (token->type == HEREDOC)
		result = handle_heredoc(current, token->next->value, head);
	else
		result = 0;
	if (result == -1)
		free_commands(head);
	return (result);
}

int	redirection(t_cmd *cmd, char *file, int out, int append)
{
	char	*tmp;

	if (!cmd || !file)
		return (-1);
	if (out)
	{
		tmp = ft_strdup(file);
		if (!tmp)
			return (-1);
		if (cmd->out)
			free(cmd->out);
		cmd->out = tmp;
		cmd->append = append;
	}
	else
	{
		tmp = ft_strdup(file);
		if (!tmp)
			return (-1);
		if (cmd->in)
			free(cmd->in);
		cmd->in = tmp;
	}
	return (0);
}
