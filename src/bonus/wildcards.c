/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:35:02 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/19 12:11:12 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_wildcard(char *sign, char *name)
{
	if (!sign || !name)
		return (0);
	if (*sign == '*' && *name == '\0')
		return (match_wildcard(sign + 1, name));
	if (*sign == '*')
	{
		while (*name)
		{
			if (match_wildcard(sign + 1, name))
				return (1);
			name++;
		}
		return (match_wildcard(sign + 1, name));
	}
	if (*sign == '?' || *sign == *name)
		return (match_wildcard(sign + 1, name + 1));
	return (0);
}

char	**expand_wildcards(char *sign)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;
	
	count = 0;
    entry = NULL;
	dir = opendir(".");
	if (!dir)
	{
		perror("opendir failed\n");
		return (NULL);
	}
	matches = malloc(sizeof(char *) * 100);
	if (!matches)
		return (NULL);
	while (entry == readdir(dir))
	{
		if (match_wildcard(sign, entry->d_name))
			matches[count++] = ft_strdup(entry->d_name);
	}
	matches[count] = NULL;
	closedir(dir);
	return (matches);
}

void	free_wildcards(char **matches)
{
	int	i;

	i = 0;
	if (!matches)
    return ;
	while (matches[i])
	{
        free(matches[i]);
		i++;
	}
	free(matches);
}

// void	expand_command_arg(t_cmd *cmd)
// {
// 	int		i;
// 	char	**expanded_args;

// 	i = 0;
// 	while (cmd->args[i])
// 	{
// 		expanded_args = expand_wildcards(cmd->args[i]);
// 		if (cmd->args[i])
// 			free(cmd->args[i]);
// 		cmd->args[i] = expanded_args;
// 		i++;
// 	}
// }

/* Pour tester */

// int main() {
//     // Tu peux tester ici avec un tableau de commandes simulées.
//     char *args[] = {"*.c", "*.h", NULL};
//     t_cmd cmd = {args};

//     expand_command_arg(&cmd);

//     // Affichage des résultats pour tester
//     for (int i = 0; cmd.args[i]; i++) {
//         printf("Expanded arg: %s\n", cmd.args[i]);
//     }
//     for (int i = 0; cmd.args[i]; i++) {
//         free(cmd.args[i]);
//     }

//     return 0;
// }
