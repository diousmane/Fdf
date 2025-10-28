/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousou <ousou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:59:00 by ousou             #+#    #+#             */
/*   Updated: 2025/10/26 23:59:00 by ousou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

/*
** Count lines in file
*/
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
** Count numbers on one line
*/
int	count_columns(char *line)
{
	char	**words;
	int		nb_columns;
	int		i;
	int		result;

	words = ft_split(line, ' ');
	if (!words)
		return (0);
	nb_columns = 0;
	while (words[nb_columns])
		nb_columns++;
	result = nb_columns;
	i = 0;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
	return (result);
}

/*
** Process one line
*/
static void	process_line(char *line, t_map *map, int y)
{
	char	**numbers;
	int		x;

	numbers = ft_split(line, ' ');
	if (!numbers)
		return ;
	x = 0;
	while (x < map->width && numbers[x])
	{
		map->z_matrix[y][x] = ft_atoi(numbers[x]);
		map->color_matrix[y][x] = COLOR_WHITE;
		if (map->z_matrix[y][x] < map->z_min)
			map->z_min = map->z_matrix[y][x];
		if (map->z_matrix[y][x] > map->z_max)
			map->z_max = map->z_matrix[y][x];
		free(numbers[x]);
		x++;
	}
	free(numbers);
}

/*
** Allocate matrices
*/
static int	allocate_matrices(t_map *map)
{
	int	i;

	map->z_matrix = malloc(sizeof(int *) * map->height);
	if (!map->z_matrix)
		return (0);
	map->color_matrix = malloc(sizeof(int *) * map->height);
	if (!map->color_matrix)
		return (0);
	i = 0;
	while (i < map->height)
	{
		map->z_matrix[i] = malloc(sizeof(int) * map->width);
		if (!map->z_matrix[i])
			return (0);
		map->color_matrix[i] = malloc(sizeof(int) * map->width);
		if (!map->color_matrix[i])
			return (0);
		i++;
	}
	return (1);
}

/*
** Fill the map
*/
void	fill_map(char *filename, t_map *map)
{
	int		fd;
	char	*line;
	int		y;

	if (!allocate_matrices(map))
		return ;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return ;
	map->z_min = 2147483647;
	map->z_max = -2147483648;
	y = 0;
	line = get_next_line(fd);
	while (line && y < map->height)
	{
		process_line(line, map, y);
		free(line);
		y++;
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	close(fd);
}
