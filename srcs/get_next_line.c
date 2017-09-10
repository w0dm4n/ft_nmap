/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/07 02:58:19 by frmarinh          #+#    #+#             */
/*   Updated: 2016/01/07 04:09:38 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"
#include "get_next_line.h"

t_gnl	*alloc_list(t_gnl *list)
{
	if (!(list = (t_gnl *)malloc(sizeof(t_gnl))))
		return (NULL);
	if (!(list->buffer = (char **)malloc(sizeof(char) * 1024)))
		return (NULL);
	list->buffer = ft_set_null(list->buffer);
	if (!(list->fd = (int *)malloc(sizeof(int))))
		return (NULL);
	list->fd[0] = 0;
	if (!(list->line = (int *)malloc(sizeof(int))))
		return (NULL);
	list->line[0] = 0;
	return (list);
}

t_gnl	*read_fd(int fd, int fd_pos, int res, t_gnl *list)
{
	char *content;

	if (!(content = (char *)malloc(sizeof(char) * BUFF_SIZE)))
		return (NULL);
	while ((res = read(fd, content, BUFF_SIZE)))
	{
		if (res == -1)
			return (NULL);
		list->buffer[fd_pos] = ft_add_in(list->buffer[fd_pos], content, res, 0);
	}
	list->line[fd_pos] = 0;
	free(content);
	return (list);
}

t_gnl	*add_fd(t_gnl *list, int const fd)
{
	int	*new;
	int	i;

	i = 1;
	while (list->fd[i])
		i++;
	if (!(new = (int*)malloc(sizeof(int) * (i + 1))))
		return (NULL);
	i = 1;
	while (list->fd[i])
	{
		new[i] = list->fd[i];
		i++;
	}
	new[i] = fd;
	list->fd = new;
	return (list);
}

char	*get_line(char *buffer, int line, int i, int i_2)
{
	int		line_pos;
	char	*line_tmp;

	line_pos = 0;
	while (buffer[i])
	{
		if (line_pos == line)
			break ;
		if (buffer[i] == '\n')
			line_pos++;
		i++;
	}
	if (!(line_tmp = (char*)malloc(sizeof(char) * (ft_strlen(buffer) + 1))))
		return (NULL);
	while (buffer[i] != '\n' && buffer[i])
		line_tmp[i_2++] = buffer[i++];
	line_tmp[i_2] = '\0';
	return (line_tmp);
}

int		get_next_line(int const fd, char **line)
{
	static t_gnl	*list;
	int				fd_position;
	char			*tmp;

	fd_position = 0;
	if (!line || fd < 3 || !BUFF_SIZE)
		return (-1);
	if (!list)
		if (!(list = alloc_list(list)))
			return (-1);
	if (!ft_check_fd(list->fd, fd))
		list = add_fd(list, fd);
	fd_position = ft_check_fd(list->fd, fd);
	if (list->buffer[fd_position] == NULL || !list->buffer[fd_position])
		if (!(list = read_fd(list->fd[fd_position], fd_position, 0, list)))
			return (-1);
	tmp = get_line(list->buffer[fd_position], list->line[fd_position]++, 0, 0);
	*line = tmp;
	if ((list->line[fd_position]) >= ft_count_lines(list->buffer[fd_position]))
		return (0);
	return (1);
}
