/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/15 05:56:01 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/15 05:56:08 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

char		*get_default_interface_host()
{
	struct ifaddrs *addrs;
	struct ifaddrs *tmp;
	getifaddrs(&addrs);
	tmp = addrs;

	while (tmp)
	{
		if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
			if (!ft_strcmp(tmp->ifa_name, DEFAULT_INTERFACE)) {
				struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
				return ft_strdup(inet_ntoa(pAddr->sin_addr));
			}
		}
		tmp = tmp->ifa_next;
	}
	return (NULL);
}
