/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:53:09 by slopez            #+#    #+#             */
/*   Updated: 2021/05/29 14:40:47 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/types.h>
#include <sys/stat.h>
// #include <unistd.h>
// #include <math.h>
// #include <signal.h>
#include <string.h>
// #include <ctype.h>


typedef struct stat t_stat;

void	die(char *string)
{
	printf("%s\n", string);
	exit (1);
}

void	parser_realloc_end(t_parser *parser, size_t v_count, size_t vn_count, size_t vt_count)
{
	if (v_count) {
		parser->v_alloc = v_count + 1;
		parser->v = realloc(parser->v, sizeof(t_vec3f) * parser->v_alloc);
		if (!parser->v) exit (1);
	} else {
		parser->v_alloc = 0;
		free(parser->v);
	}

	if (vn_count) {
		parser->vn_alloc = vn_count + 1;
		parser->vn = realloc(parser->vn, sizeof(t_vec3f) * parser->vn_alloc);
		if (!parser->vn) exit (1);
	} else {
		parser->vn_alloc = 0;
		free(parser->vn);
	}

	if (vt_count) {
		parser->vt_alloc = vt_count + 1;
		parser->vt = realloc(parser->vt, sizeof(t_vec2f) * parser->vt_alloc);
		if (!parser->vt) exit (1);
	} else {
		parser->vt_alloc = 0;
		free(parser->vt);
	}

	printf("v  %8zu / %-8zu\n", v_count, parser->v_alloc);
	printf("vn %8zu / %-8zu\n", vn_count, parser->vn_alloc);
	printf("vt %8zu / %-8zu\n", vt_count, parser->vt_alloc);

}

void parser_init(char *file)
{
	t_parser parser;
	FILE 	*fp 	= fopen(file, "r");
	char	*line 	= 0;
	ssize_t	read 	= 0;
	size_t	len 	= 0;
	t_stat	st;

	stat(file, &st);

	parser.vn_alloc = (st.st_size / 50);
	parser.v_alloc 	= (st.st_size / 50);
	parser.vt_alloc = (st.st_size / 50);

	if (!(parser.v = calloc(sizeof(t_vec3f), parser.v_alloc))) die("Error calloc");
	if (!(parser.vn = calloc(sizeof(t_vec3f), parser.vn_alloc))) die("Error calloc");
	if (!(parser.vt = calloc(sizeof(t_vec2f), parser.vt_alloc))) die("Error calloc");


	size_t	v_count 	= 0;
	size_t	vn_count 	= 0;
	size_t	vt_count 	= 0;

	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
		if (!strncmp(line, "v ", 2)) {
			v_count++;
			if (v_count > parser.v_alloc) {
				parser.v_alloc += 50;
				parser.v = realloc(parser.v, sizeof(t_vec3f) * parser.v_alloc);
			}
			sscanf(line, "v %f %f %f", &parser.v[v_count].x, &parser.v[v_count].y, &parser.v[v_count].z);
		}
		else if (!strncmp(line, "vn ", 3)) {
			vn_count++;
			if (vn_count > parser.vn_alloc) {
				parser.vn_alloc += 50;
				parser.vn = realloc(parser.vn, sizeof(t_vec3f) * parser.vn_alloc);
			}
			sscanf(line, "vn %f %f %f", &parser.vn[vn_count].x, &parser.vn[vn_count].y, &parser.vn[vn_count].z);
		} else if (!strncmp(line, "vt ", 3)) {
			vt_count++;
			if (vt_count > parser.vt_alloc) {
				parser.vt_alloc += 50;
				parser.vt = realloc(parser.vt, sizeof(t_vec2f) * parser.vt_alloc);
			}
			sscanf(line, "vn %f %f", &parser.vt[vt_count].x, &parser.vt[vt_count].y);
		}
	}

	parser_realloc_end(&parser, v_count, vn_count, vt_count);
	fclose(fp);

}