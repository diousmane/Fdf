/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    ft +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Put a pixel in the image
*/
void	put_pixel(t_window *win, int x, int y, int color)
{
	int	position;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		position = y * win->line_length + x * (win->bits_per_pixel / 8);
		*(unsigned int *)(win->img_data + position) = color;
	}
}

/*
** Draw connections from one point
*/
void	draw_connections(t_window *win, t_point p1, int x, int y)
{
	t_point	p2;

	if (x < win->map->width - 1)
	{
		p2.x = x + 1;
		p2.y = y;
		project_point(&p2, win->map->z_matrix[y][x + 1], win);
		draw_line(win, p1, p2);
	}
	if (y < win->map->height - 1)
	{
		p2.x = x;
		p2.y = y + 1;
		project_point(&p2, win->map->z_matrix[y + 1][x], win);
		draw_line(win, p1, p2);
	}
}

/*
** Draw all the map
*/
void	draw_map(t_window *win)
{
	int		x;
	int		y;
	t_point	p1;

	y = 0;
	while (y < win->map->height)
	{
		x = 0;
		while (x < win->map->width)
		{
			p1.x = x;
			p1.y = y;
			project_point(&p1, win->map->z_matrix[y][x], win);
			draw_connections(win, p1, x, y);
			x++;
		}
		y++;
	}
}
