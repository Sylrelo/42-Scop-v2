/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:53:09 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 14:48:34 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include "Prototypes.Parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

typedef struct stat t_stat;

void	parser_free(t_parser *parser)
{
	if (parser->v_count)
		free(parser->v);
	if (parser->vt_count)
		free(parser->vt);
	if (parser->vn_count)
		free(parser->vn);
}

void	_realloc_end(size_t count, size_t size, size_t *alloc_count, void **array)
{
	if (count) {
		*alloc_count = count;
		*array = realloc(*array, size * *alloc_count);
		if (!*array)
			die ("Error parser_realloc_end v");
	} 
	else {
		*alloc_count = 0;
		free(*array);
	}
}

void	parser_realloc_end(t_parser *parser, size_t v_count, size_t vn_count, size_t vt_count)
{
	printf("\033[0;32m[debug] v  %8zu / %-8zu\n", v_count, parser->v_count);
	printf("[debug] vn %8zu / %-8zu\n", vn_count, parser->vn_count);
	printf("[debug] vt %8zu / %-8zu\n\033[0m", vt_count, parser->vt_count);
	
	_realloc_end(v_count, sizeof(t_vec3f), &parser->v_count, (void *) &parser->v);
	_realloc_end(vn_count, sizeof(t_vec3f), &parser->vn_count, (void *) &parser->vn);
	_realloc_end(vt_count, sizeof(t_vec2f), &parser->vt_count, (void *) &parser->vt);
}

void	parser_realloc(size_t *count, size_t *alloc, size_t size, void **array)
{
	if (*count < *alloc)
		return ;

	*alloc += 1500;
	if (!(*array = realloc(*array, size * *alloc)))
		die("Erreor realloc");
}

void parse_face(char *line) 
{
	//printf("%s", line);
	(void) line;
}

void parser_init(t_scop *scop, char *file)
{
	t_parser parser;
	size_t	f_count 	= 0;
	size_t	v_count 	= 0;
	size_t	vn_count 	= 0;
	size_t	vt_count 	= 0;
	FILE 	*fp;
	char	*line 		= 0;
	ssize_t	read 		= 0;
	size_t	len 		= 0;
	t_stat	st;
	
	char	last_mtl[256];

	fp = fopen(file, "r");

	stat(file, &st);


	// void get_file_relative_path()
	char	*file_tmp;
	char 	*token;
	char 	path[256];
	size_t	path_len;

	path_len = 0;

	file_tmp = strdup(file);
	token = strtok(file, "/");
	while (token != NULL) {
		if (!strcmp(token, strrchr(file_tmp, '/') + 1)) 
			break ;
		strcat(path, token);
		strcat(path, "/");
		path_len += strlen(token) + 1;
		token = strtok(NULL, "/");
	}
	path[path_len] = 0;
	//-------------------------

	parser.vn_count = (st.st_size / 200);
	parser.v_count 	= (st.st_size / 200);
	parser.vt_count = (st.st_size / 200);


	if (!(parser.v 	= calloc(parser.v_count, sizeof(t_vec3f))))
		die("Error calloc v");
	if (!(parser.vn = calloc(parser.vn_count, sizeof(t_vec3f))))
		die("Error calloc vn");
	if (!(parser.vt = calloc(parser.vt_count, sizeof(t_vec2f))))
		die("Error calloc vt");

	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
		if (!strncmp(line, "mtllib ", 7)) 
		{
			parser_mtl_start(scop, &parser, path, line + 7);
		}
		else if (!strncmp(line, "v ", 2)) 
		{
			parser_realloc(&v_count, &parser.v_count, sizeof(t_vec3f), (void *) &parser.v);
			sscanf(line, "v %f %f %f", &parser.v[v_count].x, &parser.v[v_count].y, &parser.v[v_count].z);
			v_count++;
		} 
		else if (!strncmp(line, "vn ", 3)) 
		{
			parser_realloc(&vn_count, &parser.vn_count, sizeof(t_vec3f), (void *) &parser.vn);
			sscanf(line, "vn %f %f %f", &parser.vn[vn_count].x, &parser.vn[vn_count].y, &parser.vn[vn_count].z);
			vn_count++;
		}
		else if (!strncmp(line, "vt ", 3)) 
		{
			parser_realloc(&vt_count, &parser.vt_count, sizeof(t_vec2f), (void *) &parser.vt);
			sscanf(line, "vt %f %f", &parser.vt[vt_count].x, &parser.vt[vt_count].y);
			vt_count++;
		}
		else if (!strncmp(line, "f ", 2)) 
		{
			//last_mtl
			parse_face(line);
			f_count++;
		} 
		else if (!strncmp(line, "usemtl ", 7))
		{
			strcpy(last_mtl, line);
		}
	}
	parser_realloc_end(&parser, v_count, vn_count, vt_count);

	printf("\033[0;32m\n[debug] %-5s %zu\n", "V", v_count);
	printf("[debug] %-5s %zu\n", "VT", vt_count);
	printf("[debug] %-5s %zu\n", "VN", vn_count);
	printf("[debug] %-5s %zu\n\033[0;m", "F", f_count);

	parser_free(&parser);
	free(line);
	free(file_tmp);
	fclose(fp);
}