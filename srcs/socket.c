/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/15 01:17:15 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/15 01:17:21 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

SOCKET			init_socket(u_char protocol)
{
	SOCKET			sock = NULL;
	bool			opt = true;

	if ((sock = socket(AF_INET, SOCK_RAW, protocol)) < 0) // Raw socket descriptor
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
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&opt, sizeof(opt)) < 0) // allow socket to send datagrams to broadcast addresses
	{
		printf("setsockopt() failed to set SO_BROADCAST\n");
		close(sock);
		return (SOCKET_ERROR);
	}
	return (sock);
}

bool			send_socket_raw(t_thread_handler *handler, int raw_len, int port)
{
	struct sockaddr_in		sin;
	struct ip				*ip_header = (struct ip*)((void*)handler->buffer_raw);

	pthread_mutex_lock(&queue_lock);
	ft_memset (&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip_header->ip_dst.s_addr;
	sin.sin_port = htons(port);

	return (sendto(handler->fd, handler->buffer_raw, raw_len, 0,
		(struct sockaddr *)&sin, sizeof(struct sockaddr)));
}
