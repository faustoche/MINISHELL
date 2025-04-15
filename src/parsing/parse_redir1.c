/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 10:09:44 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirection_to_cmd(t_cmd **cmd, char *file, int out, int append)
{
	char	*tmp;

	tmp = ft_strdup(file);
	if (!tmp)
		return (-1);
	if (out)
	{
		if ((*cmd)->out)
			free((*cmd)->out);
		(*cmd)->out = tmp;
		(*cmd)->append = append;
	}
	else
	{
		if ((*cmd)->in)
			free((*cmd)->in);
		(*cmd)->in = tmp;
	}
	return (0);
}

int	redirection(t_cmd **cmd, char *file, int out, int append)
{
	int	fd;

	if (!cmd || !file)
		return (-1);
	if (out)
	{
		if (append)
			fd = open_file(file, REDIR_APPEND, (*cmd)->exit_status);
		else
			fd = open_file(file, REDIR_OUT, (*cmd)->exit_status);
	}
	else
		fd = open_file(file, REDIR_IN, (*cmd)->exit_status);
	if (fd == -1)
		return (-1);
	close(fd);
	if (apply_redirection_to_cmd(cmd, file, out, append) == -1)
		return (-1);
	return (0);
}

static int	if_init_command(t_cmd **curr, t_cmd **head, t_env *env)
{
	*curr = init_command();
	if (!(*curr))
		return (-1);
	(*curr)->env_list = env;
	if (!(*head))
		*head = *curr;
	if (init_args(*curr) == -1)
		return (-1);
	return (0);
}

int	process_redir_token(t_token **tok, t_cmd **cur, t_cmd **hd, t_env *env)
{
	if (!(*cur))
	{
		if (if_init_command(cur, hd, env) == -1)
			return (-1);
	}
	if (!(*tok)->next || ((*tok)->next->type != TOKEN_ARGUMENT
			&& (*tok)->next->type != TOKEN_COMMAND))
	{
		printf(ERR_SYNTAX);
		free_commands(*hd);
		return (-1);
	}
	if (handle_redirection(*tok, *cur, *hd) == -1)
	{
		*hd = NULL;
		*cur = NULL;
		return (-1);
	}
	*tok = (*tok)->next;
	if (*tok)
		*tok = (*tok)->next;
	return (0);
}

// int	process_redir_token(t_tn **tok, t_cmd **curr, t_cmd **head, t_env *env)
// {
// 	if (!(*curr))
// 		if (if_init_command(curr, head, env) == -1)
// 			return (-1);
// 	if (!(*tok)->next || ((*tok)->next->type != TOKEN_ARGUMENT
// 			&& (*tok)->next->type != TOKEN_COMMAND))
// 	{
// 		(printf(ERR_SYNTAX), free_commands(*head));
// 		return (-1);
// 	}
// 	if (handle_redirection(*tok, *curr, *head) == -1)
// 	{
// 		if (*head)
// 			free_commands(*head);
// 		*head = NULL;
// 		*curr = NULL;
// 		return (-1);
// 	}
// 	*tok = (*tok)->next;
// 	if (*tok)
// 		*tok = (*tok)->next;
// 	return (0);
// }

// int	process_redir_token(t_ton **tok, t_cmd **curr, t_cmd **head, t_env *env)
// {
// 	if (!(*curr))
// 	{
// 		*curr = init_command();
// 		if (!(*curr))
// 			return (-1);
// 		(*curr)->env_list = env;
// 		if (!(*head))
// 			*head = *curr;
// 		if (init_args(*curr) == -1)
// 			return (-1);
// 	}
// 	if (!(*tok)->next || ((*tok)->next->type != TOKEN_ARGUMENT
// 			&& (*tok)->next->type != TOKEN_COMMAND))
// 	{
// 		(printf(ERR_SYNTAX), free_commands(*head));
// 		return (-1);
// 	}
// 	if (handle_redirection(*tok, *curr, *head) == -1)
// 	{
// 		if (*head)
// 			free_commands(*head);
// 		*head = NULL;
// 		*curr = NULL;
// 		return (-1);
// 	}
// 	*tok = (*tok)->next;
// 	if (*tok)
// 		*tok = (*tok)->next;
// 	return (0);
// }

/* According to the token, redirect in the right direction */

// static int redirection(t_cmd **cmd, char *file, int out, int append)
// {
//     char *tmp;
//     int fd;

//     if (!cmd || !file)
//         return (-1);
//     if (out)
//     {
//         if (append)
//             fd = open_file(file, REDIR_APPEND, (*cmd)->exit_status);
//         else
//             fd = open_file(file, REDIR_OUT, (*cmd)->exit_status);
//     }
//     else
//         fd = open_file(file, REDIR_IN, (*cmd)->exit_status);
//     if (fd == -1)
//         return (-1);
//     close(fd);
//     tmp = ft_strdup(file);
//     if (!tmp)
//         return (-1);
//     if (out)
//     {
//         if ((*cmd)->out)
//             free((*cmd)->out);
//         (*cmd)->out = tmp;
//         (*cmd)->append = append;
//     }
//     else
//     {
//         if ((*cmd)->in)
//             free((*cmd)->in);
//         (*cmd)->in = tmp;
//     }
//     return (0);
// }
