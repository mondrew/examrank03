/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mondrew <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 07:32:00 by mondrew           #+#    #+#             */
/*   Updated: 2020/09/24 08:20:49 by mondrew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_paint.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	if (!str)
		return ;
	write(1, str, ft_strlen(str));
}

void	ft_lst_clear(t_list *lst)
{
	t_list	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}

t_list	*ft_lst_del_one(t_list *lst)
{
	t_list	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst->next;
	free(lst);
	lst = tmp;
	return (lst);
}

t_list	*ft_lst_new(char type, float x, float y, \
						float radius, char elem_char)
{
	t_list	*new;

	if (!(new = malloc(sizeof(t_list))))
		return (NULL);
	new->type = type;
	new->x = x;
	new->y = y;
	new->radius = radius;
	new->elem_char = elem_char;
	new->next = NULL;
	return (new);
}

t_list	*ft_lst_add_back(t_list *lst, char type, float x, float y, \
						float radius, char elem_char)
{
	t_list	*tmp;
	t_list	*new;

	if (!(new = ft_lst_new(type, x, y, radius, elem_char)))
	{
		ft_lst_clear(lst);
		return (NULL);
	}
	if (!lst)
		lst = new;
	else
	{
		tmp = lst;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (lst);
}

int		ft_error(FILE *ifp, t_list *lst)
{
	fclose(ifp);
	if (lst)
		ft_lst_clear(lst);
	return (1);
}

void	ft_fill_pictures(t_data *first, t_list **lst_ptr, char *str)
{
	t_list	*lst;
	int		i;
	int		j;
	float	dist;

	lst = *lst_ptr;
	while (lst)
	{
		j = 0;
		while (j < first->height)
		{
			i = 0;
			while (i < first->width)
			{
				dist = sqrtf(powf(i - lst->x, 2) + powf(j - lst->y, 2));
				if (dist <= lst->radius)
				{
					if ((lst->radius - dist) < 1)
						str[(first->width + 1) * j + i] = lst->elem_char;
					if (lst->type == 'C')
						str[(first->width + 1) * j + i] = lst->elem_char;
				}
				i++;
			}
			j++;
		}
		lst = ft_lst_del_one(lst);
	}
	*lst_ptr = lst;
}

void	ft_fill_background(t_data *first, char *str)
{
	int		line;
	int		row;
	int		i;

	i = 0;
	line = 0;
	while (line < first->height)
	{
		row = 0;
		while (row < first->width)
		{
			str[i] = first->back_char;
			i++;
			row++;
		}
		str[i] = '\n';
		i++;
		line++;
	}
	i--;
	str[i] = '\0';
}

int		ft_print_image(t_data *first, t_list **lst_ptr)
{
	char	*str;

	if (!(str = malloc(sizeof(char) * ((first->width * first->height) + \
						(first->height - 1) + 1))))
		return (-1);
	ft_fill_background(first, str);
	ft_fill_pictures(first, lst_ptr, str);
	ft_putstr(str);
	ft_putstr("\n");
	free(str);
	return (0);
}

int		mini_paint(char *filename)
{
	FILE	*ifp;
	t_data	first;
	t_list	*lst;
	int		ret;

	char	final_char;

	char	type;
	float	x;
	float	y;
	float	radius;
	char	elem_char;

	lst = NULL;
	if (!(ifp = fopen(filename, "r")))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	if ((ret = fscanf(ifp, "%d %d %c%c", &first.width, &first.height, \
					&first.back_char, &final_char)) == 4)
	{
		if (first.width > 0 && first.width <= 300 && \
			first.height > 0 && first.height <= 300 && \
			first.back_char > 31 && first.back_char < 127 && \
			final_char == '\n')
			;
		else
			return (ft_error(ifp, lst));
	}
	else
		return (ft_error(ifp, lst));
	while ((ret = fscanf(ifp, "%c %f %f %f %c%c", &type, &x, &y, &radius, \
					&elem_char, &final_char)) != EOF)
	{
		if ((ret == 6 && final_char == '\n') || (ret == 5))
		{
			if ((type == 'c' || type == 'C') && (radius > 0) && \
					(elem_char > 31 && elem_char < 127))
			{
				if (!(lst = ft_lst_add_back(lst, type, x, y, radius, elem_char)))
				{
					fclose(ifp);
					return (1);
				}
			}
			else
				return (ft_error(ifp, lst));
		}
		else
			return (ft_error(ifp, lst));
		if (ret == 5)
		{
			if ((ret = fscanf(ifp, "%c %f %f %f %c%c", &type, &x, &y, &radius, \
					&elem_char, &final_char)) == EOF)
				break;
			else
				return (ft_error(ifp, lst));
		}
	}
	if ((ft_print_image(&first, &lst)) == -1)
	{
		fclose(ifp);
		if (!lst)
			ft_lst_clear(lst);
		return (1);
	}
	fclose(ifp);
	if (!lst)
		ft_lst_clear(lst);
	return (0);
}

int		main(int argc, char **argv)
{
	int		ret;

	if (argc != 2)
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
	ret = mini_paint(argv[1]);
	return (ret);
}
