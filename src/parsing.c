/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    ft +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	clear_zmatrix(t_map *map)
{
	int	i;

	if (!map || !map->z_matrix)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->z_matrix[i])
			free(map->z_matrix[i]);
		i++;
	}
	free(map->z_matrix);
	map->z_matrix = NULL;
}

int	count_lines(char *filename)
{
	int		fd;
	int		nb_lines;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	nb_lines = 0;
	line = get_next_line(fd);
	while (line)
	{
		nb_lines++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (nb_lines);
}

/*
** Get width from first line
*/
int	get_width(char *filename, t_map *map)
{
	int		fd;
	char	*first_line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	first_line = get_next_line(fd);
	if (!first_line)
	{
		close(fd);
		return (0);
	}
	map->width = count_columns(first_line);
	free(first_line);
	close(fd);
	return (1);
}

/*
** Main parsing function (multi-pass as earlier)
*/
int	parse_map(char *filename, t_map *map)
{
	map->height = count_lines(filename);
	if (map->height == 0)
	{
		ft_putstr_fd("Error: empty file\n", 2);
		return (0);
	}
	if (!get_width(filename, map))
	{
		ft_putstr_fd("Error: cannot open file\n", 2);
		return (0);
	}
	if (map->width == 0)
		return (0);
	if (!fill_map(filename, map))
	{
		ft_putstr_fd("Error: invalid map\n", 2);
		clear_zmatrix(map);
		return (0);
	}
	return (1);
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
