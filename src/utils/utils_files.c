/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:12:45 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 18:16:53 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_fd(int fd)
{
	if (fd >= 1024)
		return ;
	close(fd);
	close_all_fd(fd + 1);
}

int	open_file(char *filename, int token)
{
	int	fd;

	fd = 0;
	if (!filename)
	{
		printf("Error: NULL filename\n");
		return (-1);
	}
	if (token == REDIR_OUT)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (token == REDIR_APPEND)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (token == REDIR_IN)
		fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("minislay:");
		printf(ERR_DIR, filename);
		close_all_fd(3);
		return (-1);
	}
	return (fd);
}
