/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/08 22:14:14 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* - volatile : force le compilateur a acceder a 
			la memoire ou la variable est stockee,
			pour etre sur que mise a jour bien effectuee
			dans gestionnaire de signaux.
	- sig_atomic_t : garantit que acces a la variable ne peuvent
			etre interrompus ou modifies de facon incomplete. */

volatile sig_atomic_t	g_received_signal = 0;

static char	*prompt(void)
{
	char	*line;

	line = readline("minislay> \033[1;34m~\033[0m ");
	if (!line)
	{
		printf("exit\n");
		clear_history();
		free(line);
		return (NULL);
	}
	if (line && *line)
		add_history(line);
	return (line);
}

static int handle_all_heredocs(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_eof)
		{
			if (handle_heredoc(cmd, cmd->heredoc_eof, cmd) == -1)
				return (-1);
			free(cmd->heredoc_eof);
			cmd->heredoc_eof = NULL;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char	pwd[PATH_MAX];
	char	*input;
	t_token	*token_list;
	t_cmd	*commands;
	t_env	*env_list;
	t_env	*original_env;

	(void)ac;
	(void)av;
	commands = NULL;
	token_list = NULL;
	if(!(getcwd(pwd, sizeof(pwd))))
		return (print_error_message("Error: can't get pwd\n"));
	original_env = init_env(envp);
	env_list = copy_env_list(original_env);
	if (!env_list)
		return (print_error_message("Error: invalid env variable\n"));
	free_env_list(&original_env);
	if (!env_list)
	{
		free_env_list(&original_env);
		return (print_error_message("Error: env variable init\n"));
	}
	env_list = change_var_value(env_list, "OLDPWD", pwd);
	while (1)
	{
		set_signal_handlers();
		input = prompt();
		if (!input)
			break;
		char *fixed_input = fix_dollar_quote(input);
		free(input);
		if (!fixed_input)
			continue ;
		input = fixed_input;
		token_list = parse_input(input);
		if (!token_list)
		{
			free(input);
			input = NULL;
			continue ;
		}
		expand_tokens(token_list, env_list);
		commands = parse_commands(token_list, env_list);
		free_token_list(token_list);
		free(input);
		input = NULL;
		close_all_fd(3);
		if (commands)
		{
			if (commands && handle_all_heredocs(commands) == -1)
			{
				free_commands(commands);
				continue ;
			}
			if (is_empty_command(commands))
			{
				if (is_redirection(commands))
					execute_only_redirections(commands);
			}
			else if (is_builtins(commands->args[0]) && !has_pipes(commands))
			{
				if (is_redirection(commands))
					handle_builtin_redirection(commands, &env_list);
				else
					builtins_execution(commands, &env_list);
			}
			else if (has_pipes(commands))
				execute_pipeline(commands, env_list);
			else if (is_redirection(commands))
				execute_redirection(commands, env_list);
			else
				execute_commands(commands, env_list);
		}
		close_all_fd(3);
		free_commands(commands);
		commands = NULL;
	}
	if (input)
		free(input);
	free_env_list(&env_list);
	clear_history();
	close_all_fd(3);
	return (EXIT_SUCCESS);
}
