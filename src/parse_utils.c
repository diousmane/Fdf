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
** Count numbers on one line
*/
int	count_columns(char *line)
{
	int		count;
	int		in_word;
	int		i;

	if (!line)
		return (0);
	count = 0;
	in_word = 0;
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

/*
** Process one line: validate fields and fill row
*/
int	process_line(char *line, t_map *map, int y)
{
	char	**numbers;
	int		x;
	int		i;

	numbers = ft_split(line, ' ');
	if (!numbers)
		return (0);
	x = 0;
	i = 0;
	while (numbers[i])
	{
		if (!is_valid_integer(numbers[i]))
			return (free_split(numbers), 0);
		if (x < map->width)
			map->z_matrix[y][x++] = ft_atoi(numbers[i]);
		i++;
	}
	free_split(numbers);
	while (x < map->width)
		map->z_matrix[y][x++] = 0;
	return (1);
}

/*
** Allocate matrices
*/
int	allocate_matrices(t_map *map)
{
	int		i;

	map->z_matrix = malloc(sizeof(int *) * map->height);
	if (!map->z_matrix)
		return (0);
	i = 0;
	while (i < map->height)
	{
		map->z_matrix[i] = malloc(sizeof(int) * map->width);
		if (!map->z_matrix[i])
		{
			while (--i >= 0)
				free(map->z_matrix[i]);
			free(map->z_matrix);
			map->z_matrix = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Fill the map
*/
int	fill_map(char *filename, t_map *map)
{
	int		fd;
	int		ok;

	if (!allocate_matrices(map))
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	ok = read_and_fill_rows(fd, map);
	close(fd);
	return (ok);
}
