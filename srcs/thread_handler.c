/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/16 03:52:59 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/16 03:53:07 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

t_thread_handler		*add_thread_handler(t_thread_handler *threads, t_thread_handler* new)
{
	if (!threads) {
		threads = new;
		return (threads);
	} else {
		t_thread_handler *start = threads;
		while (threads->next)
			threads = threads->next;
		threads->next = new;
		return (start);
	}
}
