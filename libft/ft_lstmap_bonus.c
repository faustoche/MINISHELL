/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:46:10 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Itère sur la liste ’lst’ et applique la fonction
’f ’au contenu de chaque élément. Crée une nouvelle
liste résultant des applications successives de
’f’. La fonction ’del’ est là pour détruire le
contenu d’un élément si nécessaire.*/

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*new_list;
	void	*new_element;

	if (!lst || !f || !del)
		return (NULL);
	start = NULL;
	while (lst)
	{
		new_element = f(lst->content);
		new_list = ft_lstnew(new_element);
		if (!new_list)
		{
			del(new_element);
			ft_lstclear(&start, del);
			return (NULL);
		}
		ft_lstadd_back(&start, new_list);
		lst = lst->next;
	}
	return (start);
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

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
	free(*lst);
	*lst = NULL;
}
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	*f(void *content)
{
	int	*new_c = malloc(sizeof(int));
	if (new_c == NULL)
		return (NULL);
	*new_c = *(int *)content;
	return (new_c);
}

void	del(void *content)
{
	free(content);
}

void	print_content(void *content)
{
	printf("Content : %s\n", (char *)content);
}

int	main(void)
{
	int	i;
	t_list	*lst = NULL;
	t_list	*new_list;
	int	str[] = {1, 2, 3, 4, 5, 6};

	i = 0;
	while (i < 6)
	{
		t_list	*node = ft_lstnew(&str[i]);
		if (!node)
		{
			ft_lstclear(&lst, del);
			return (1);
		}
		ft_lstadd_back(&lst, node);
		i++;
	}
	printf("list : \n");
	print_content(lst);
	
	new_list = ft_lstmap(lst, f, del);

	printf("new list : \n");
	print_content(new_list);

	ft_lstclear(&lst, del);
	ft_lstclear(&new_list, del);
}
*/