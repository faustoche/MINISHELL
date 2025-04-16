/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:59:54 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 14:00:22 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_termsig(int printed_signal, int status)
{
	if (!printed_signal)
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			g_received_signal = SIGQUIT;
		}
		else if (WTERMSIG(status) == SIGINT)
			g_received_signal = SIGINT;
		printed_signal = 1;
	}
	return (printed_signal);
}
