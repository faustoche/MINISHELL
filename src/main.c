/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 14:58:52 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

static char	*prompt(void)
{
	char	*line;

	line = readline("minislay> ~ ");
	if (!line)
	{
		printf("exit\n");
		rl_clear_history();
		free(line);
		return (NULL);
	}
	if (line && *line)
		add_history(line);
	return (line);
}

t_env	*init_environment(char **envp, char *pwd)
{
	t_env	*env_list;
	t_env	*og_env;

	og_env = init_env(envp);
	if (!og_env)
	{
		og_env = init_minimal_env();
		if (!og_env)
			return (printf("Error: cannot create minimal env\n"), NULL);
	}
	env_list = copy_env_list(og_env);
	if (!env_list)
	{
		printf("Error: invalid env variable\n");
		return (NULL);
	}
	free_env_list(&og_env);
	if (!env_list)
	{
		(free_env_list(&og_env), printf("Error: env variable init\n"));
		return (NULL);
	}
	env_list = change_var_value(env_list, "OLDPWD", pwd);
	return (env_list);
}

void	handle_signals_main(void)
{
	handle_signals(SIGINT, PROMPT);
	handle_signals(SIGQUIT, IGNORE);
}

char	*first_input_check(int *last_cmd_code, t_env *env_list)
{
	char	*input;
	char	*fixed_input;

	input = prompt();
	if (!input)
		return (NULL);
	if (g_received_signal == SIGINT)
	{
		*last_cmd_code = 128 + SIGINT;
		g_received_signal = 0;
	}
	fixed_input = fix_dollar_quote(input);
	free(input);
	if (!fixed_input)
		return (NULL);
	if (handle_direct_env_var(fixed_input, env_list))
	{
		free(fixed_input);
		return (NULL);
	}
	return (fixed_input);
}

int	main(int ac, char **av, char **envp)
{
	char	pwd[PATH_MAX];
	t_env	*env_list;
	char	*input;
	int		last_cmd_code;

	(void)av;
	if (ac != 1)
		(printf("minislay: too many arguments\n"), exit(2));
	last_cmd_code = 0;
	if (!(getcwd(pwd, sizeof(pwd))))
		return (print_error_message("Error: can't get pwd\n"));
	env_list = init_environment(envp, pwd);
	if (!env_list)
		return (1);
	while (1)
	{
		handle_signals_main();
		input = first_input_check(&last_cmd_code, env_list);
		if (!input)
			break ;
		process_commands(input, &env_list, &last_cmd_code);
	}
	(free_env_list(&env_list), rl_clear_history(), close_all_fd(3));
	return (EXIT_SUCCESS);
}
