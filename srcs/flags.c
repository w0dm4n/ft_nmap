/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 14:39:39 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/10 14:39:52 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

t_flag		*new_flag(char *flag)
{
	t_flag		*new;

	if (!(new = (t_flag*)malloc(sizeof(struct s_flag))))
		return (NULL);
	new->next = NULL;
	new->flag = ft_strdup(flag);
	new->value = NULL;
	return (new);
}

t_flag		*get_flags()
{
	return (g_flags);
}

t_flag		*get_flag(char *name)
{
	t_flag		*flags = get_flags();
	while (flags)
	{
		if (!ft_strcmp(flags->flag, name))
			return (flags);
		flags = flags->next;
	}
	return (NULL);
}

void		add_flag(char *flag)
{
	t_flag		*flags = get_flags();
	if (!flags) {
		g_flags = new_flag(flag);
	} else {
		while (flags->next)
			flags = flags->next;
		flags->next = new_flag(flag);
	}
}

bool		add_value(char *value)
{
	t_flag		*flags = get_flags();

	if (flags) {
		while (flags)
		{
			if (flags->value == NULL) {
				flags->value = ft_strdup(value);
				return (true);
			}
			flags = flags->next;
		}
	}
	return (false);
}
