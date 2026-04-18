/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 01:21:59 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/18 01:38:33 by sloubiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	ft_atoi(const char *s)
{
	long int	i;
	int			neg;

	i = 0;
	neg = 1;
	while ((*s >= 9 && *s <= 13) || *s == 32)
		s++;
	if (*s == '-' || *s == '+')
		if (*(s++) == '-')
			neg = -1;
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		i *= 10;
		i += (*s % 48);
		if (neg == -1 && i > 2147483648)
			return (0);
		if (neg == 1 && i > 2147483647)
			return (0);
		s++;
	}
	return ((int) neg * i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
