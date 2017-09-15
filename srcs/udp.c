/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   udp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 20:43:32 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/14 20:43:39 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void			udp_handler(t_thread_handler *thread_handler, char *scan, char *host)
{
	int 		raw_len;

	raw_len = sizeof(struct ip) + sizeof(struct udphdr);
	thread_handler->fd = init_socket(IPPROTO_UDP);
	if (thread_handler->fd == SOCKET_ERROR)
		return ;
	thread_handler->buffer_raw = malloc(sizeof(*thread_handler->buffer_raw) * raw_len);
	if (thread_handler->buffer_raw == NULL)
		return ;
	set_ipv4_header(thread_handler->buffer_raw, raw_len, host, IPPROTO_UDP);
}
