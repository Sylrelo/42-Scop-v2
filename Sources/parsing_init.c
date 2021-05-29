/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:53:09 by slopez            #+#    #+#             */
/*   Updated: 2021/05/29 14:52:01 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

typedef struct stat t_stat;

void	die(char *string)
{
	printf("%s\n", string);
	exit (1);
}

void	parser_realloc_end(t_parser *parser, size_t v_count, size_t vn_count, size_t vt_count)
{
	printf("v  %8zu / %-8zu\n", v_count, parser->v_count);
	printf("vn %8zu / %-8zu\n", vn_count, parser->vn_count);
	printf("vt %8zu / %-8zu\n", vt_count, parser->vt_count);
	
	if (v_count) {
		parser->v_count = v_count + 1;
		parser->v = realloc(parser->v, sizeof(t_vec3f) * parser->v_count);
		if (!parser->v)
			die ("Error parser_realloc_end v");
	} else {
		parser->v_count = 0;
		free(parser->v);
	}

	if (vn_count) {
		parser->vn_count = vn_count + 1;
		parser->vn = realloc(parser->vn, sizeof(t_vec3f) * parser->vn_count);
		if (!parser->vn)
			die ("Error parser_realloc_end vn");
	} else {
		parser->vn_count = 0;
		free(parser->vn);
	}

	if (vt_count) {
		parser->vt_count = vt_count + 1;
		parser->vt = realloc(parser->vt, sizeof(t_vec2f) * parser->vt_count);
		if (!parser->vt)
			die ("Error parser_realloc_end vt");
	} else {
		parser->vt_count = 0;
		free(parser->vt);
	}


}

void parser_init(char *file)
{
	t_parser parser;
	size_t	v_count 	= 0;
	size_t	vn_count 	= 0;
	size_t	vt_count 	= 0;
	FILE 	*fp 		= fopen(file, "r");
	char	*line 		= 0;
	ssize_t	read 		= 0;
	size_t	len 		= 0;
	t_stat	st;

	stat(file, &st);

	parser.vn_count = (st.st_size / 50) / 3;
	parser.v_count 	= (st.st_size / 50) / 3;
	parser.vt_count = (st.st_size / 50) / 3;

	if (!(parser.v 	= calloc(sizeof(t_vec3f), parser.v_count)))
		die("Error calloc v");
	if (!(parser.vn = calloc(sizeof(t_vec3f), parser.vn_count)))
		die("Error calloc vn");
	if (!(parser.vt = calloc(sizeof(t_vec2f), parser.vt_count)))
		die("Error calloc vt");

	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
		if (!strncmp(line, "v ", 2) && v_count++) {
			if (v_count >= parser.v_count) {
				parser.v_count += REALLOC_SIZE;
				if (!(parser.v = realloc(parser.v, sizeof(t_vec3f) * parser.v_count)))
					die("Erreor realloc v");
			}
			sscanf(line, "v %f %f %f", &parser.v[v_count].x, &parser.v[v_count].y, &parser.v[v_count].z);
		}
		else if (!strncmp(line, "vn ", 3) && vn_count++) {
			if (vn_count >= parser.vn_count) {
				parser.vn_count += REALLOC_SIZE;
				if (!(parser.vn = realloc(parser.vn, sizeof(t_vec3f) * parser.vn_count)))
					die("Erreor realloc vn");
			}
			sscanf(line, "vn %f %f %f", &parser.vn[vn_count].x, &parser.vn[vn_count].y, &parser.vn[vn_count].z);
		} else if (!strncmp(line, "vt ", 3) && vt_count++) {
			if (vt_count >= parser.vt_count) {
				parser.vt_count += REALLOC_SIZE;
				if (!(parser.vt = realloc(parser.vt, sizeof(t_vec2f) * parser.vt_count)))
					die("Erreor realloc vt");
			}
			sscanf(line, "vt %f %f", &parser.vt[vt_count].x, &parser.vt[vt_count].y);
		}
	}

	parser_realloc_end(&parser, v_count, vn_count, vt_count);
	fclose(fp);

}