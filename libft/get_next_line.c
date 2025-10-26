/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ousdiall <ousdiall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 00:00:00 by ousdiall          #+#    #+#             */
/*   Updated: 2025/10/26 19:08:10 by ousdiall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*extract_line(char *stash)
{
	int		len;
	char	*line;

	if (!stash || !stash[0])
		return (NULL);
	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		line = malloc(sizeof(char) * (len + 2));
	else
		line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	len = 0;
	while (stash[len] && stash[len] != '\n')
	{
		line[len] = stash[len];
		len++;
	}
	if (stash[len] == '\n')
		line[len++] = '\n';
	line[len] = '\0';
	return (line);
}

char	*clean_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	i++;
	j = 0;
	while (stash[i + j])
		j++;
	new_stash = malloc(sizeof(char) * (j + 1));
	if (!new_stash)
		return (free(stash), NULL);
	j = -1;
	while (stash[i])
		new_stash[++j] = stash[i++];
	new_stash[++j] = '\0';
	return (free(stash), new_stash);
}

static int	read_buffer(int fd, char *buffer, char **stash)
{
	int	read_bytes;

	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes < 0)
	{
		free(buffer);
		if (*stash)
		{
			free(*stash);
			*stash = NULL;
		}
		return (-1);
	}
	buffer[read_bytes] = '\0';
	*stash = ft_strjoin_get(*stash, buffer);
	if (!*stash)
	{
		free(buffer);
		return (-1);
	}
	return (read_bytes);
}

static char	*read_and_stash(int fd, char *stash)
{
	char	*buffer;
	int		read_bytes;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	read_bytes = 1;
	while (!ft_strchr_get(stash, '\n') && read_bytes > 0)
	{
		read_bytes = read_buffer(fd, buffer, &stash);
		if (read_bytes < 0)
			return (NULL);
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_and_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = clean_stash(stash);
	return (line);
}
