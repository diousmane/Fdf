/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Launch the program
*/
static int	launch_program(char *filename, t_window *win, t_map *map)
{
	if (!map)
		return (0);
	if (!parse_map(filename, map))
	{
		ft_putstr_fd("Error: reading file\n", 2);
		free(map);
		return (0);
	}
	win->map = map;
	init_mlx(win);
	calc_zoom(win);
	draw_map(win);
	return (1);
}

/*
** Setup and run MLX loop
*/
static void	run_mlx(t_window *win)
{
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	mlx_hook(win->win, 2, 1L << 0, key_press, win);
	mlx_hook(win->win, 17, 0, close_win, win);
	mlx_loop(win->mlx);
}

/*
** Launch FdF program
*/
static int	launch_fdf(char *filename)
{
	t_window	*win;
	t_map		*map;

	map = malloc(sizeof(t_map));
	win = malloc(sizeof(t_window));
	if (!win)
	{
		if (map)
			free(map);
		return (0);
	}
	if (!launch_program(filename, win, map))
	{
		free(win);
		return (0);
	}
	run_mlx(win);
	return (1);
}

/*
** Main function
*/
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Usage: ./fdf map_file.fdf\n", 2);
		return (1);
	}
	if (!launch_fdf(argv[1]))
	{
		ft_putstr_fd("Error\n", 2);
		return (1);
	}
	return (0);
}
