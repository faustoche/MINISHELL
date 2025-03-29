/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/29 22:08:20 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator(int c)
{
	return (c == '|' || c == '>' || c == '<' || c == ';'
		|| c == '&' || c == '(' || c == ')');
}

int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	skip_space(t_lexer *lexer)
{
	while (lexer->input[lexer->pos] && is_space(lexer->input[lexer->pos]))
		lexer->pos++;
}

int	is_redirection(t_cmd *cmd)
{
    printf("Checking redirection: in=%p, out=%p, heredoc=%d\n", cmd->in, cmd->out, cmd->heredoc);
	return (cmd->in || cmd->out || cmd->append || cmd->heredoc);
}

int	is_numeric(char *str)
{
	int	i;

	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	else
		i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void check_open_fds(void) 
{
    char fd_path[256];
    DIR *dir;
    struct dirent *entry;

    snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd", getpid());
    
    dir = opendir(fd_path);
    if (dir == NULL) {
        perror("Impossible d'ouvrir le répertoire des descripteurs");
        return;
    }

    printf("Descripteurs de fichiers ouverts :\n");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            char full_path[512];
            char link_target[1024];
            int len;

            snprintf(full_path, sizeof(full_path), "%s/%s", fd_path, entry->d_name);
            len = readlink(full_path, link_target, sizeof(link_target) - 1);
            
            if (len != -1) {
                link_target[len] = '\0';
                printf("FD %s -> %s\n", entry->d_name, link_target);
            }
        }
    }

    closedir(dir);
}