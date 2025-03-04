/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:16 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Compte le nombre d’éléments de la liste. */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		lst = lst->next;
		count++;
	}
	return (count);
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
	t_list	*lst = ft_lstnew("Faustoche");
	printf("Taille apres ajout de lst : %d\n", ft_lstsize(lst));
	   
	t_list	*new1 = ft_lstnew("new1");
	ft_lstadd_front(&lst, new1);
	printf("Taille apres ajout de new1 : %d\n", ft_lstsize(lst));
	
	t_list	*new2 = ft_lstnew("new2");
	ft_lstadd_front(&lst, new2);
	printf("Taille apres ajout de new2 : %d\n", ft_lstsize(lst));
}*/