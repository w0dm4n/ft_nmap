/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 18:27:05 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/12 18:27:06 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void					*in_thread(void *handler)
{
	t_thread_handler		*thread_handler = (t_thread_handler*)handler;
	if (thread_handler) {
		start_scans(thread_handler);
	}
	return (NULL);
}

static t_thread_handler		*alloc_handler(t_nmap *nmap, int index, int len)
{
	t_thread_handler		*new = NULL;

	if (!(new = (t_thread_handler*)malloc(sizeof(struct s_thread_handler))))
		return (NULL);
	new->nmap = nmap;
	new->start = index;
	new->ports_len = len;
	return (new);
}

void						new_thread(t_nmap *nmap, int index, int len)
{
	if (len > 0) {
		pthread_t			thread;
		t_thread_handler	*handler = alloc_handler(nmap, index, len);
		if (handler) {
			pthread_create(&thread, NULL, in_thread, handler);
			printf("New thread with start index %d started !\n", index);
			pthread_join(thread, NULL);
		}
	}
}

void						instantiate_threads(t_nmap *nmap)
{
	int i = 0, end_diff = 0, total = 0;
	int ports = nmap->ports_index / nmap->threads;
	while (i < nmap->threads)
	{
		if ((i + 1) == nmap->threads) {
			end_diff = ports * nmap->threads;
			if (end_diff < nmap->ports_index) {
				end_diff = nmap->ports_index - end_diff;
				new_thread(nmap, total, (ports + end_diff));
				total += ports + end_diff;
			} else {
				new_thread(nmap, total, ports);
				total += ports;
			}
		} else {
			new_thread(nmap, total, ports);
			total += ports;
		}
		i++;
	}
}
