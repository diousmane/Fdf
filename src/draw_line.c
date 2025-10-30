/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    ft +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	plot_horizontal(t_window *win, t_point *p1, t_point *p2)
{
	int		dx;
	int		dy;
	int		err;
	int		dir_y;
	t_point	cur;

	dx = abs_value(p2->x - p1->x);
	dy = abs_value(p2->y - p1->y);
	dir_y = 1;
	if (p1->y >= p2->y)
		dir_y = -1;
	err = dx / 2;
	cur.x = p1->x;
	cur.y = p1->y;
	while (cur.x <= p2->x)
	{
		put_pixel(win, cur.x, cur.y, COLOR_WHITE);
		err -= dy;
		if (err < 0)
		{
			cur.y += dir_y;
			err += dx;
		}
		cur.x++;
	}
}

void	plot_vertical(t_window *win, t_point *p1, t_point *p2)
{
	int		dx;
	int		dy;
	int		err;
	int		dir_x;
	t_point	cur;

	dx = abs_value(p2->x - p1->x);
	dy = abs_value(p2->y - p1->y);
	dir_x = 1;
	if (p1->x >= p2->x)
		dir_x = -1;
	err = dy / 2;
	cur.x = p1->x;
	cur.y = p1->y;
	while (cur.y <= p2->y)
	{
		put_pixel(win, cur.x, cur.y, COLOR_WHITE);
		err -= dx;
		if (err < 0)
		{
			cur.x += dir_x;
			err += dy;
		}
		cur.y++;
	}
}

/*
** Draw a horizontal line
*/
void	line_horizontal(t_window *win, t_point *p1, t_point *p2)
{
	plot_horizontal(win, p1, p2);
}

/*
** Draw a vertical line
*/
void	line_vertical(t_window *win, t_point *p1, t_point *p2)
{
	plot_vertical(win, p1, p2);
}

/*
** Draw a line between two points (white)
*/
void	draw_line(t_window *win, t_point *p1, t_point *p2)
{
	if (abs_value(p2->x - p1->x) >= abs_value(p2->y - p1->y))
	{
		if (p1->x > p2->x)
			line_horizontal(win, p2, p1);
		else
			line_horizontal(win, p1, p2);
	}
	else
	{
		if (p1->y > p2->y)
			line_vertical(win, p2, p1);
		else
			line_vertical(win, p1, p2);
	}
}
