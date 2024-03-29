#include "mini_paint.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

t_list	*ft_lst_new(char type, float x, float y, float radius, \
															char elem_char)
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
	t_list	*new;
	t_list	*tmp;

	if (!lst)
	{
		if (!(new = ft_lst_new(type, x, y, radius, elem_char)))
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
	new->radius = radius;
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

void	ft_fill_pictures(t_data *first, t_list **lst_ptr, char *str)
{
	int		i; // x
	int		j; // y
	float	dist;
	t_list	*lst;

	lst = *lst_ptr;
	while (lst != NULL)
	{
		j = 0;
		while (j < first->height)
		{
			i = 0;
			while (i < first->width)
			{
				dist = sqrtf((powf((i - lst->x), 2) + powf((j - lst->y), 2)));
				if (dist <= lst->radius)
				{
					// border
					if ((lst->radius - dist) < 1)
					{
						str[(j * (first->width + 1)) + i] = lst->elem_char;
					}
					// filled inside
					if (lst->type == 'C')
					{
						str[(j * (first->width + 1)) + i] = lst->elem_char;
					}
				}
				i++;
			}
			j++;
		}
		lst = ft_lst_del_one(lst);
	}
	*lst_ptr = lst;
}

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

int		ft_mini_paint(char *filename)
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
	while ((ret = fscanf(ifp, "%c %f %f %f %c%c", &type, &x, &y, \
					&radius, &elem_char, &final_char)) != EOF)
	{
		if ((ret == 6 && final_char == '\n') || (ret == 5))
		{
			if ((type == 'c' || type == 'C') && (radius > 0) && \
				(elem_char > 31 && elem_char < 127))
			{
				if (!(lst = ft_lst_add_back(lst, type, x, y, radius, elem_char)))
					return (ft_error(ifp, lst));
			}
			else
				return (ft_error(ifp, lst));
		}
		else
			return (ft_error(ifp, lst));
		if (ret == 5)
		{
			if ((ret = fscanf(ifp, "%c %f %f %f %c%c", &type, &x, &y, \
						&radius, &elem_char, &final_char)) == EOF)
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
	ret = ft_mini_paint(argv[1]);
	return (ret);
}
