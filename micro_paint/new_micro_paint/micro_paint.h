#ifndef MICRO_PAINT_H
# define MICRO_PAINT_H

# define UP 0
# define DOWN 1

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
	float			width;
	float			height;
	char			elem_char;
	struct s_list	*next;
}					t_list;

#endif
