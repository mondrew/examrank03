/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mondrew <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 14:11:13 by mondrew           #+#    #+#             */
/*   Updated: 2020/09/23 21:38:18 by mondrew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_PAINT_H
# define MINI_PAINT_H

typedef struct		s_data
{
	int				width;
	int				height;
	char			back_char;
}					t_data;

typedef struct		s_list
{
	char			type;
	float			x;
	float			y;
	float			radius;
	char			elem_char;
	struct s_list	*next;
}					t_list;

#endif
