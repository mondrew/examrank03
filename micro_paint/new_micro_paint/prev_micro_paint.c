/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mondrew <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 14:04:32 by mondrew           #+#    #+#             */
/*   Updated: 2020/09/23 09:42:11 by mondrew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro_paint.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	ft_putstr(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	write(1, str, i);
}

void	ft_lst_clear(t_list *lst)
{
	t_list	*tmp;

	if (!lst)
		return ;
	while (lst->next != NULL)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
	free(lst);
}

t_list	*ft_lst_new(char type, float x, float y, float width, float height, \
															char elem_char)
{
	t_list	*new;

	if (!(new = malloc(sizeof(t_list))))
		return (NULL);
	new->type = type;
	new->x = x;
	new->y = y;
	new->width = width;
	new->height = height;
	new->elem_char = elem_char;
	new->next = NULL;
	return (new);
}

t_list	*ft_lst_add_back(t_list *lst, char type, float x, float y, \
						float width, float height, char elem_char)
{
	t_list	*new;
	t_list	*tmp;

	if (!lst)
	{
		if (!(new = ft_lst_new(type, x, y, width, height, elem_char)))
			return (NULL);
		return (new);
	}
	tmp = lst;
	if (!(new = malloc(sizeof(t_list))))
	{
		ft_lst_clear(lst);
		return (NULL);
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->type = type;
	new->x = x;
	new->y = y;
	new->width = width;
	new->height = height;
	new->elem_char = elem_char;
	new->next = NULL;
	return (lst);
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

int		ft_error(FILE *ifp, t_list *lst)
{
	ft_putstr("Error: Operation file corrupted\n");
	fclose(ifp);
	if (lst)
		ft_lst_clear(lst);
	return (1);
}

void	ft_fill_background(t_data *first, char *str)
{
	int		line;
	int		row;
	int		i;

	line = 0;
	i = 0;
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

int		ft_round(float num, int id)
{
	float	mod;

	if (num < 0)
		mod = -num;
	else
		mod = num;
	while (mod >= 10)
		mod -= 10;
	while (mod >= 1)
		mod -= 1;
	if (mod == 0)
		return (num);
	if (id == UP)
	{
		if (num > 0)
			return (num - mod + 1);
		else if (num < 0)
			return (num + mod);
	}
	else
	{
		if (num > 0)
			return (num - mod);
		else if (num < 0)
			return (num + mod - 1);
	}
	return (0);
}

void	ft_fill_pictures(t_data *first, t_list **lst_ptr, char *str)
{
	t_list	*lst;
	int		i;// x
	int		j;// y

	lst = *lst_ptr;
	while (lst)
	{
		j = 0;
		while (j < first->height)
		{
			i = 0;
			while (i < first->width)
			{
				if ((i >= lst->x) && (i <= (lst->x + lst->width)) && \
					(j >= lst->y) && (j <= (lst->y + lst->height)))
				{
					if (((i >= lst->x) && ((i - lst->x) < 1)) || \
						((i <= (lst->x + lst->width)) && ((lst->x + lst->width - i) < 1)) || \
						((j >= lst->y) && ((j - lst->y) < 1)) || \
						((j <= (lst->y + lst->height)) && ((lst->y + lst->height - j) < 1)))
						str[(first->width + 1) * j + i] = lst->elem_char;
					if (lst->type == 'R')
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

/*
void	ft_fill_pictures(t_data *first, t_list **lst_ptr, char *str)
{
	t_list	*lst;
	int		start_x;
	int		start_y;
	int		stop_x;
	int		stop_y;
	int		i;

	lst = *lst_ptr;
	i = 0;
	while (lst != NULL)
	{
		start_x = ft_round(lst->x, UP);
		start_y = ft_round(lst->y, UP);
		stop_x = ft_round(lst->x + lst->width, DOWN);
		stop_y = ft_round(lst->y + lst->height, DOWN);
		if ((start_x > (first->width - 1)) || (stop_x < 0) || \
				((start_y > (first->height - 1)) || (stop_y < 0)) || \
				(start_x > stop_x || start_y > stop_y))
		{
			lst = ft_lst_del_one(lst);
		}
		else
		{
			// first line
			if (start_y >= 0 && start_y <= (first->height - 1))
			{
				i = start_x;
				while (i <= stop_x && i <= (first->width - 1))
				{
					if (i >= 0)
						str[start_y * (first->width + 1) + i] = lst->elem_char;
					i++;
				}
			}
			start_y++;
			// middle lines
			while (start_y < 0 && start_y < stop_y && start_y <= (first->height - 1))
				start_y++;
			while (start_y < stop_y && start_y <= (first->height - 1))
			{
				i = start_x;
				if (i >= 0 && i <= (first->width - 1))
					str[start_y * (first->width + 1) + i] = lst->elem_char;
				while ((lst->type == 'R') && i <= (first->width - 1) && \
						i <= stop_x)
				{
					if (i >= 0)
						str[start_y * (first->width + 1) + i] = lst->elem_char;
					i++;
				}
				i = stop_x;
				if (i >= 0 && i <= (first->width - 1))
					str[start_y * (first->width + 1) + i] = lst->elem_char;
				start_y++;
			}
			// last line
			if (stop_y >= 0 && stop_y <= (first->height - 1))
			{
				i = start_x;
				while (i <= stop_x && i <= (first->width - 1))
				{
					if (i >= 0)
						str[stop_y * (first->width + 1) + i] = lst->elem_char;
					i++;
				}
			}
			lst = ft_lst_del_one(lst);
		}
	}
	*lst_ptr = lst;
}

*/

int		ft_print_image(t_data *first, t_list **lst)
{
	char	*str;

	if (!(str = malloc(sizeof(char) * ((first->width * first->height) + \
												(first->height - 1) + 1))))
		return (-1);
	ft_fill_background(first, str);
	ft_fill_pictures(first, lst, str);
	ft_putstr(str);
	ft_putstr("\n");
	free(str);
	return (0);
}

int		ft_micro_paint(char *filename)
{
	FILE	*ifp;
	t_data	first;
	t_list	*lst;
	int		ret;
	char	final_char;

	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	elem_char;

	lst = NULL;
	if ((ifp = fopen(filename, "r")) == NULL)
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	// The first line
	if ((ret = fscanf(ifp, "%d %d %c%c", &first.width, &first.height, \
					&first.back_char, &final_char)) == 4)
	{
		if (final_char != '\n')
			return (ft_error(ifp, lst));
		if ((first.width > 0 && first.width <= 300) && \
			(first.height > 0 && first.height <= 300) && \
			(first.back_char > 31 && first.back_char < 127))
			;
		else 
			return (ft_error(ifp, lst));
	}
	else
		return (ft_error(ifp, lst));
	// Other lines
	while ((ret = fscanf(ifp, "%c %f %f %f %f %c%c", &type, &x, \
			&y, &width, &height, &elem_char, &final_char)) != EOF)
	{
		if ((ret == 7 && final_char == '\n') || (ret == 6))
		{
			if ((type == 'r' || type == 'R') && (width > 0) && (height > 0) && \
				(elem_char > 31 && elem_char < 127))
			{
				if (!(lst = ft_lst_add_back(lst, type, x, y, width, height, elem_char)))
					return (ft_error(ifp, lst));
			}
		}
		else
			return (ft_error(ifp, lst));
		if (ret == 6)
		{
			if ((ret = fscanf(ifp, "%c %f %f %f %f %c%c", &type, &x, &y, \
						&width, &height, &elem_char, &final_char)) == EOF)
				break ;
			else
				return (ft_error(ifp, lst));
		}
	}
	if (ret != EOF)
		return (ft_error(ifp, lst));
	if ((ft_print_image(&first, &lst)) == -1)
	{
		ft_lst_clear(lst);
		fclose(ifp);
		return (1);
	}
	ft_lst_clear(lst);
	fclose(ifp);
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
	ret = ft_micro_paint(argv[1]);
	return (ret);
}
