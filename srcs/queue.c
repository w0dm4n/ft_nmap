/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 02:48:21 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/19 02:32:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

t_queue			*new_queue(int port, u_char proto, char *scan, int id)
{
	t_queue		*new_queue = NULL;

	if (!(new_queue = (t_queue*)malloc(sizeof(struct s_queue))))
		return (NULL);
	new_queue->next = NULL;
	new_queue->port = port;
	new_queue->proto = proto;
	new_queue->scan = ft_strdup(scan);
	new_queue->done = false;
	new_queue->id = id;
	return (new_queue);
}

void			add_queue(t_queue *new_queue)
{
	if (all_queues) {
		t_queue	*tmp = all_queues;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_queue;
	} else {
		all_queues = new_queue;
	}
	pthread_mutex_unlock(&queue_lock);
}

t_queue			*find_queue(u_char proto, int id)
{
	pthread_mutex_lock(&queue_lock);
	t_queue	*tmp = all_queues;
	while (tmp)
	{
		if (id == -1 && tmp->proto == IPPROTO_UDP) {
			pthread_mutex_unlock(&queue_lock);
			return (tmp);
		}
		if (tmp->id == id && tmp->proto == proto && !tmp->done) {
			pthread_mutex_unlock(&queue_lock);
			return (tmp);
		}
		tmp = tmp->next;
	}
	pthread_mutex_unlock(&queue_lock);
	return (NULL);
}
