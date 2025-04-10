/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/10 11:53:25 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* - volatile : force le compilateur a acceder a 
			la memoire ou la variable est stockee,
			pour etre sur que mise a jour bien effectuee
			dans gestionnaire de signaux.
	- sig_atomic_t : garantit que acces a la variable ne peuvent
			etre interrompus ou modifies de facon incomplete. 
*/

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

int	main(int ac, char **av, char **envp)
{
	char	pwd[PATH_MAX];
	char	*input;
	t_token	*token_list;
	t_cmd	*commands;
	t_env	*env_list;
	t_env	*original_env;
	char	*fixed_input;
	//t_cmd	*last_cmd;

	(void)ac;
	(void)av;
	commands = NULL;
	token_list = NULL;
	if (!(getcwd(pwd, sizeof(pwd))))
		return (print_error_message("Error: can't get pwd\n"));
	original_env = init_env(envp);
	if (!original_env)
	{
		original_env = init_minimal_env();
		if (!original_env)
			return (print_error_message("Error: couldn't create minimal env\n"));
	}
	env_list = copy_env_list(original_env);
	if (!env_list)
	{
		//free_env_list(env_list);
		return (print_error_message("Error: invalid env variable\n"));
	}
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
			break ;
		fixed_input = fix_dollar_quote(input);
		free(input);
		if (!fixed_input)
			continue ;
		input = fixed_input;
		if (fixed_input)
		{
			if (handle_direct_env_var(fixed_input, env_list))
			{
				free(fixed_input);
				continue;
			}
		}
		token_list = parse_input(input);
		if (!token_list)
		{
			free(input);
			input = NULL;
			continue ;
		}
		//last_cmd = NULL;
		expand_tokens(token_list, env_list, commands);
		commands = parse_commands(token_list, env_list);
		free_token_list(token_list);
		free(input);
		input = NULL;
		close_all_fd(3);
		if (commands)
		{
			handle_signals(SIGINT, IGNORE);
			if (commands && handle_all_heredocs(commands) == -1)
			{
				free_commands(commands);
				continue ;
			}
			if (is_empty_command(commands)) // pour gerer si <> en premier
			{
				if (is_redirection(commands))
				{
					execute_only_redirections(commands);
					free_commands(commands);
					commands = NULL;
					continue ;
				}
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
			else if (is_redirection(commands) && commands->out && check_output_directory(commands))
			{
				free_commands(commands);
				commands = NULL;
				continue ;
			}
			else if (is_redirection(commands) && commands->in && access(commands->in, F_OK) == -1)
			{
				printf(ERR_DIR, commands->in);
				free_commands(commands);
				commands = NULL;
				continue ;
			}
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

int	check_output_directory(t_cmd *commands)
{
	char	*dir;
	char	*file;
	
	if (!commands->out)
		return (0);
		
	dir = ft_strdup(commands->out);
	if (!dir)
		return (0);
		
	file = strrchr(dir, '/');
	if (file)
	{
		*file = '\0';
		if (access(dir, F_OK) == -1)
		{
			printf(ERR_DIR, commands->out);
			free(dir);
			return (1);
		}
	}
	free(dir);
	return (0);
}
