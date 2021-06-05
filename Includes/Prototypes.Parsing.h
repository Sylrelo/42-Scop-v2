/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prototypes.Parsing.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:54:46 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 14:45:47 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPES_PARSING_H
# define PROTOTYPES_PARSING_H

#include "Scop.h"

void    parser_init(t_scop *scop, char *argv);
void    parser_mtl_start(t_scop *scop, char path[256], char *file);

size_t  parse_face(t_parser *parser, t_mat **materials, size_t *material_count, char *last_mtl, char *line);
void    parse_texture(t_scop *scop, t_mat *material, char path[256], char *file);


void        init_mat_default_values(t_mat *material);

#endif