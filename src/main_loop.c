/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:03:46 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 13:03:56 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_and_expand(char *input, t_env *env_list, int *last_cmd_code)
{
	t_token	*token_list;
	t_cmd	*cmd;

	token_list = parse_input(input, last_cmd_code);
	if (!token_list)
		return (NULL);
	expand_tokens(token_list, env_list, last_cmd_code);
	cmd = parse_commands(token_list, env_list, last_cmd_code);
	free_token_list(token_list);
	close_all_fd(3);
	return (cmd);
}

int	before_execution(t_cmd *cmd)
{
	handle_signals(SIGINT, IGNORE);
	if (handle_all_heredocs(cmd) == -1)
		return (-1);
	return (0);
}

int	execute_flow(t_cmd *cmd, t_env **env_list)
{
	if (is_empty_command(cmd))
	{
		if (is_redirection(cmd))
			execute_only_redirections(cmd);
	}
	else if (is_builtins(cmd->args[0]) && !has_pipes(cmd))
	{
		if (is_redirection(cmd))
			handle_builtin_redirection(cmd, env_list);
		else
			builtins_execution(cmd, env_list);
	}
	else if (has_pipes(cmd))
		execute_pipeline(cmd, *env_list);
	else if (is_redirection(cmd) && cmd->out && check_output_directory(cmd))
		return (1);
	else if (is_redirection(cmd) && cmd->in && access(cmd->in, F_OK) == -1)
		return (printf(ERR_DIR, cmd->in), 1);
	else if (is_redirection(cmd))
		execute_redirection(cmd, *env_list);
	else
		execute_commands(cmd, *env_list);
	return (0);
}

void	process_commands(char *input, t_env **env_list, int *last_cmd_code)
{
	t_cmd	*cmd;

	cmd = parse_and_expand(input, *env_list, last_cmd_code);
	if (!cmd)
		return ;
	if (before_execution(cmd) == -1)
	{
		if (cmd->exit_status)
			*last_cmd_code = *(cmd->exit_status);
		free_commands(cmd);
		return ;
	}
	if (execute_flow(cmd, env_list) == 1)
	{
		*last_cmd_code = 1;
		free_commands(cmd);
		return ;
	}
	if (cmd->exit_status)
		*last_cmd_code = *(cmd->exit_status);
	free_commands(cmd);
}
