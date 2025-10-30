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
** Init, draw and enter MLX loop
*/
int	init_and_run(t_window *win)
{
	init_mlx(win);
	calc_zoom(win);
	draw_map(win);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	mlx_hook(win->win, 2, 1L << 0, key_press, win);
	mlx_hook(win->win, 17, 0, close_win, win);
	mlx_loop(win->mlx);
	return (1);
}

/*
** Launch FdF program (alloc, parse, run)
*/
int	start_fdf(char *filename)
{
	t_window	*win;
	t_map		*map;

	map = malloc(sizeof(t_map));
	win = malloc(sizeof(t_window));
	if (!map || !win)
	{
		if (map)
			free(map);
		if (win)
			free(win);
		return (0);
	}
	if (!parse_map(filename, map))
	{
		free(map);
		free(win);
		return (0);
	}
	win->map = map;
	return (init_and_run(win));
}

/*
** Check if file has .fdf extension
*/
int	check_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strncmp(filename + len - 4, ".fdf", 4) == 0);
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
	if (!check_extension(argv[1]))
	{
		ft_putstr_fd("Error: file must have .fdf extension\n", 2);
		return (1);
	}
	if (!start_fdf(argv[1]))
	{
		return (1);
	}
	return (0);
}
