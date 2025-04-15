/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/15 18:28:55 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_abs_relative(t_env *env, t_cmd *cur)
{
	char	*bin;

	bin = ft_strdup(cur->args[0]);
	if (!bin)
		return ;
	if (access(bin, F_OK) == -1)
	{
		*cur->exit_status = 127;
		printf(ERR_DIR, cur->args[0]);
		free(bin);
	}
	else
	{
		create_child_process(cur->args, bin, env, cur->exit_status);
		free(bin);
	}
}

static void	process_bin_name(t_env *env, t_cmd *cur)
{
	char	*bin;

	bin = find_bin_path(cur->args[0], env);
	if (bin == NULL)
	{
		printf(ERR_CMD, cur->args[0]);
		if (cur->exit_status)
			*(cur->exit_status) = 127;
	}
	else
	{
		create_child_process(cur->args, bin, env, cur->exit_status);
		free(bin);
	}
}

void	execute_commands(t_cmd *cmd, t_env *env)
{
	t_cmd	*cur;

	cur = cmd;
	while (cur)
	{
		if (is_builtins(cur->args[0]) && cur->args && cur->args[0])
			builtins_execution(cur, &env);
		else if (cur->args && cur->args[0])
		{
			if (cur->args[0][0] == '/')
				process_abs_relative(env, cur);
			else if ((cur->args[0][0] == '/') || (cur->args[0][0] == '.'
				&& cur->args[0][1] == '/'))
				process_abs_relative(env, cur);
			else
				process_bin_name(env, cur);
		}
		cur = cur->next;
	}
}
