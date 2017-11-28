/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lld.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nolivier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:56:38 by nolivier          #+#    #+#             */
/*   Updated: 2017/09/28 11:22:04 by nolivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void			ft_print_lld(t_process *process, t_settings *sets,
	unsigned int ocp, unsigned char **params)
{
	unsigned int		tmp;
	unsigned int		i;

	if (!((sets->options & VERBOSITY) && (sets->options & OPERATIONS)))
		return ;
	tmp = 0;
	i = 0xffffffff;
	if (((ocp >> 6) & 0x3) == DIR_CODE)
	{
		while (++i < 4 && i < DIR_SIZE)
			tmp |= ((sets->memory[(params[1] - sets->memory + i) % MEM_SIZE])
			<< (8 * (DIR_SIZE - 1 - i)));
		ft_printf("P%5d | lld %d r%d\n", process->num, tmp,
		sets->memory[(process->proc_counter + DIR_SIZE + 2) % MEM_SIZE]);
	}
	else if (((ocp >> 6) & 0x3) == IND_CODE)
	{
		while (++i < 4 && i < DIR_SIZE)
			tmp |= ((sets->memory[(params[1] - sets->memory + i) % MEM_SIZE])
			<< (8 * (DIR_SIZE - 1 - i)));
		ft_printf("P%5d | lld %d r%d\n", process->num, tmp,
		sets->memory[(process->proc_counter + IND_SIZE + 2) % MEM_SIZE]);
	}
}

static void			ft_put_reg(unsigned char **params, unsigned char *memory,
	t_process *process)
{
	unsigned int		i;

	i = 0;
	while (i < REG_SIZE && i < DIR_SIZE)
	{
		params[1][REG_SIZE - 1 - i] =
		memory[(params[0] - memory + (DIR_SIZE - 1 - i)) % MEM_SIZE];
		i++;
	}
	while (i < DIR_SIZE)
	{
		params[1][REG_SIZE - 1 - i] = 0;
		i++;
	}
	ft_modif_carry_reg(process, params[1], 13);
}

int					ft_lld(t_settings *sets, t_process *process,
	t_process **begin)
{
	unsigned char		ocp;
	unsigned int		counter;
	unsigned char		*params[2];

	if (++process->cycle_count < (g_op_tab + 12)->cycles)
		return (1);
	if (!(ocp = ft_get_ocp(sets, process, &counter, params)))
	{
		ft_pc_move(process, sets, counter);
		return (0);
	}
	ft_print_lld(process, sets, ocp, params);
	ft_put_reg(params, sets->memory, process);
	ft_pc_move(process, sets, counter);
	ft_graph_process(process, sets);
	return (1);
	(void)begin;
}
