/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prototypes.Parsing.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:54:46 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 12:18:35 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPES_PARSING_H
# define PROTOTYPES_PARSING_H

#include "Scop.h"

void    parser_init(t_scop *scop, char *argv);
void    parser_mtl_start(t_scop *scop, t_parser *parser, char path[256], char *file);

#endif