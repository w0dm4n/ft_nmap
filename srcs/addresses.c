/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addresses.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/04 16:02:03 by frmarinh          #+#    #+#             */
/*   Updated: 2017/10/04 16:02:07 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static char			**get_host_addresses(struct hostent *host)
{
	char			**hosts		= NULL;
	char			*tmp_host	= NULL;
	struct in_addr	IP;
	int				i			= 0;
	int				len			= ft_array_len(host->h_addr_list) + 1;

	if (len > 0) {
		if (!(hosts = (char**)malloc(len * sizeof(char*))))
			return (NULL);
		while (host->h_addr_list[i]) {
			IP.s_addr = *((uint32_t*) host->h_addr_list[i]);
			if ((tmp_host = inet_ntoa(IP)) != NULL) {
				hosts[i] = ft_strdup(tmp_host);
			}
			i++;
		}
		hosts[i] = NULL;
		return (hosts);
	}
	return (NULL);
}

bool		load_addresses(t_nmap *nmap)
{
	t_host			*hosts				= nmap->hosts;
	struct hostent	*current_host		= NULL;
	bool			address_found		= false;
	while (hosts)
	{
		if ((current_host = gethostbyname(hosts->address)) != NULL) {
			if ((hosts->addresses = get_host_addresses(current_host)) != NULL) {
				address_found = true;
			}
		} else {
			printf("Failed to resolve \"%s\".\n", hosts->address);
		}
		hosts = hosts->next;
	}
	return (address_found);
}
