/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 21:47:45 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirection_to_cmd(t_cmd **cmd, char *file, int out, int append)
{
	char *tmp;

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

static int redirection(t_cmd **cmd, char *file, int out, int append)
{
	int fd;

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

int process_redir_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env)
{
    if (!(*curr))
        if (if_init_command(curr, head, env) == -1)
            return (-1);
    if (!(*tok)->next || ((*tok)->next->type != TOKEN_ARGUMENT
            && (*tok)->next->type != TOKEN_COMMAND))
    {
        printf(ERR_SYNTAX);
        free_commands(*head);
        return (-1);
    }
    if (handle_redirection(*tok, *curr, *head) == -1)
    {
        *head = NULL;
        *curr = NULL;
        return (-1);
    }
    *tok = (*tok)->next;
    if (*tok)
        *tok = (*tok)->next;
    return (0);
}

// int	process_redir_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env)
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

// int	process_redir_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env)
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

int	check_for_current(t_token *token, t_cmd *current, t_cmd *head)
{
	if (!current || !token->next)
	{
		printf("Error: invalid redirection\n");
		free_commands(head);
		return (-1);
	}
	return (0);
}

int	handle_redirection(t_token *token, t_cmd *current, t_cmd *head)
{
	int	result;

	result = 0;
	if (check_for_current(token, current, head) == -1)
		return (-1);
	if (token->type == REDIR_OUT)
		result = redirection(&current, token->next->value, 1, 0);
	else if (token->type == REDIR_IN)
		result = redirection(&current, token->next->value, 0, 0);
	else if (token->type == REDIR_APPEND)
		result = redirection(&current, token->next->value, 1, 1);
	else if (token->type == HEREDOC)
	{
		if (!(*token).next)
			return (print_error_message("Error: expected heredoc delimiter\n"));
		(*current).heredoc_eof = ft_strdup((*token).next->value);
		if (!(*current).heredoc_eof)
			return (print_error_message("Error: malloc heredoc_delim\n"));
	}
	else
		result = 0;
	if (result == -1)
		free_commands(head);
	return (result);
}
