/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Initialize MLX window and image
*/
void	init_mlx(t_window *win)
{
	win->mlx = mlx_init();
	if (!win->mlx)
	{
		ft_putstr_fd("Error: MLX init failed\n", 2);
		exit(1);
	}
	win->win = mlx_new_window(win->mlx, WIDTH, HEIGHT, TITLE);
	if (!win->win)
	{
		ft_putstr_fd("Error: window creation failed\n", 2);
		exit(1);
	}
	win->img = mlx_new_image(win->mlx, WIDTH, HEIGHT);
	if (!win->img)
	{
		ft_putstr_fd("Error: image creation failed\n", 2);
		exit(1);
	}
	win->img_data = mlx_get_data_addr(win->img, &win->bits_per_pixel,
			&win->line_length, &win->endian);
}

/*
** Calculate automatic zoom
*/
void	calc_zoom(t_window *win)
{
	int	max_dimension;
	int	zoom_x;
	int	zoom_y;

	max_dimension = win->map->width + win->map->height;
	zoom_x = WIDTH / max_dimension;
	zoom_y = HEIGHT / max_dimension;
	if (zoom_x < zoom_y)
		win->zoom = zoom_x;
	else
		win->zoom = zoom_y;
	win->zoom = (win->zoom * 80) / 100;
	if (win->zoom < 1)
		win->zoom = 1;
}

/*
** When a key is pressed
*/
int	key_press(int key, t_window *win)
{
	if (key == KEY_ESC)
		close_win(win);
	return (0);
}

/*
** Free an entire map
*/
void	free_map(t_map *map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->z_matrix && map->z_matrix[i])
			free(map->z_matrix[i]);
		i++;
	}
	if (map->z_matrix)
		free(map->z_matrix);
	free(map);
}

/*
** Close the program properly
*/
int	close_win(t_window *win)
{
	if (win->img)
		mlx_destroy_image(win->mlx, win->img);
	if (win->win)
		mlx_destroy_window(win->mlx, win->win);
	if (win->mlx)
	{
		mlx_destroy_display(win->mlx);
		free(win->mlx);
	}
	if (win->map)
		free_map(win->map);
	free(win);
	exit(0);
}
