/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_id.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 05:07:18 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/18 05:07:23 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int		get_id()
{
	static int	id = 59000;
	int			current_id = 0;

	pthread_mutex_lock(&id_lock);
	current_id = id++;
	pthread_mutex_unlock(&id_lock);
	return (current_id);
}
