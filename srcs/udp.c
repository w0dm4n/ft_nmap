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

static SOCKET	init_socket()
{
	SOCKET	sock = NULL;
	bool	opt = true;

	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) // Raw socket descriptor
	{
		printf("socket() failed : Operation not permitted\n");
		return (SOCKET_ERROR);
	}
	if ((setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt))) < 0) // set flag so socket expects us to provide IPv4 header.
	{
		printf("setsockopt() failed to set IP_HDRINCL\n");
		close(sock);
		return (SOCKET_ERROR);
	}
	return (sock);
}

void			udp_handler(t_thread_handler *thread_handler, char *scan, char *host)
{
	int 		raw_len;

	raw_len = sizeof(struct ip) + sizeof(struct udphdr);
	thread_handler->fd = init_socket();
	if (thread_handler->fd == SOCKET_ERROR)
		return ;
	thread_handler->buffer_raw = malloc(sizeof(*thread_handler->buffer_raw) * raw_len);
	if (thread_handler->buffer_raw == NULL)
		return ;
	set_ipv4_header(thread_handler->buffer_raw, raw_len, host, IPPROTO_UDP);
}
