/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:07 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Renvoie le dernier element de la liste */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
/*

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (new && lst)
	{
		new->next = *lst;
		*lst = new;
	}
}

int	main(void)
{
	t_list *lst = ft_lstnew("Faustoche");
	printf("%s\n", (char *)lst->content);
	t_list *new_content = ft_lstnew("Pistoche");
	ft_lstadd_front(&lst, new_content);
	t_list *last = ft_lstlast(lst);
	if (last)
		printf("%s\n", (char *)last->content);
	return (0);
}*/