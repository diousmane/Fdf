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
** Free split array
*/
void	free_split(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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
** Parse FDF value: extract number part (before comma) and convert to int
** Handles: "123" -> 123, "0xff" -> 255, "123,0xff00" -> 123
*/
int	parse_fdf_value(char *str)
{
	char	*value_str;
	int		i;
	int		result;

	i = 0;
	while (str[i] && str[i] != ',' && str[i] != '\n')
		i++;
	value_str = malloc(i + 1);
	if (!value_str)
		return (0);
	i = 0;
	while (str[i] && str[i] != ',' && str[i] != '\n')
	{
		value_str[i] = str[i];
		i++;
	}
	value_str[i] = '\0';
	result = ft_atoi_hex(value_str);
	free(value_str);
	return (result);
}

/*
** Read and fill rows from file (validates rectangular shape)
*/
int	read_and_fill_rows(int fd, t_map *map)
{
	char	*line;
	int		y;

	y = 0;
	line = get_next_line(fd);
	while (line && y < map->height)
	{
		if (count_columns(line) != map->width)
			return (free(line), 0);
		if (!process_line(line, map, y))
			return (free(line), 0);
		free(line);
		y++;
		line = get_next_line(fd);
	}
	return (1);
}
