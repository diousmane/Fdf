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

/*
** Draw a horizontal line
*/
void	line_horizontal(t_window *win, t_point p1, t_point p2)
{
	int	dx;
	int	dy;
	int	err;
	int	dir_y;

	dx = abs_value(p2.x - p1.x);
	dy = abs_value(p2.y - p1.y);
	if (p1.y < p2.y)
		dir_y = 1;
	else
		dir_y = -1;
	err = dx / 2;
	while (p1.x <= p2.x)
	{
		put_pixel(win, p1.x, p1.y, COLOR_WHITE);
		err -= dy;
		if (err < 0)
		{
			p1.y += dir_y;
			err += dx;
		}
		p1.x++;
	}
}

/*
** Draw a vertical line
*/
void	line_vertical(t_window *win, t_point p1, t_point p2)
{
	int	dx;
	int	dy;
	int	err;
	int	dir_x;

	dx = abs_value(p2.x - p1.x);
	dy = abs_value(p2.y - p1.y);
	if (p1.x < p2.x)
		dir_x = 1;
	else
		dir_x = -1;
	err = dy / 2;
	while (p1.y <= p2.y)
	{
		put_pixel(win, p1.x, p1.y, COLOR_WHITE);
		err -= dx;
		if (err < 0)
		{
			p1.x += dir_x;
			err += dy;
		}
		p1.y++;
	}
}

/*
** Draw a line between two points (white)
*/
void	draw_line(t_window *win, t_point p1, t_point p2)
{
	if (abs_value(p2.x - p1.x) >= abs_value(p2.y - p1.y))
	{
		if (p1.x > p2.x)
			line_horizontal(win, p2, p1);
		else
			line_horizontal(win, p1, p2);
	}
	else
	{
		if (p1.y > p2.y)
			line_vertical(win, p2, p1);
		else
			line_vertical(win, p1, p2);
	}
}
