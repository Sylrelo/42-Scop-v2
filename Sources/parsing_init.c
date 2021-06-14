/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:53:09 by slopez            #+#    #+#             */
/*   Updated: 2021/06/15 00:32:05 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

#include <stdio.h>
#include <sys/stat.h>
#include <float.h>

typedef struct stat t_stat;

static void	free_obj_arrays(t_parser *parser)
{
	if (parser->v_count)
		free(parser->v);
	if (parser->vt_count)
		free(parser->vt);
	if (parser->vn_count)
		free(parser->vn);
}

static void	realloc_obj_arrays(size_t *count, size_t *alloc, size_t size, void **array)
{
	if (*count < *alloc)
		return ;

	*alloc += 1500;
	if (!(*array = realloc(*array, size * *alloc)))
		die("Erreor realloc");
}

// static void	_realloc_end(size_t count, size_t size, size_t *alloc_count, void **array)
// {
// 	if (count)
// 	{
// 		*alloc_count = count;
// 		if (!(*array = realloc(*array, size * *alloc_count)))
// 			die ("Error parser_realloc_end v");
// 	} 
// 	else
// 	{
// 		*alloc_count = 0;
// 		free(*array);
// 	}
// }

// static void	parser_realloc_end(t_parser *parser, size_t v_count, size_t vn_count, size_t vt_count)
// {
// 	_realloc_end(v_count, sizeof(t_vec3f), &parser->v_count, (void *) &parser->v);
// 	_realloc_end(vn_count, sizeof(t_vec3f), &parser->vn_count, (void *) &parser->vn);
// 	_realloc_end(vt_count, sizeof(t_vec2f), &parser->vt_count, (void *) &parser->vt);
// }

static void get_relative_path(char path[256], char *file)
{
	char	*file_tmp;
	char 	*token;
	size_t	path_len;

	if (!(file_tmp = strdup(file)))
		die ("Error strdup file_tmp");

	memset(path, 0, 256);
	path_len = 0;

	token = strtok(file, "/");
	while (token != NULL)
	{
		if (!strcmp(token, strrchr(file_tmp, '/') + 1)) 
			break ;
		strcat(path, token);
		strcat(path, "/");
		path_len += strlen(token) + 1;
		token = strtok(NULL, "/");
	}
	free(file_tmp);
}

void 		parser_init(t_scop *scop, char *file)
{
	t_parser	parser;
	FILE 		*fp;
	t_stat		st;
	char		last_mtl[256];
	char		path[256];
	char		*line 		= NULL;
	size_t		f_count 	= 0;
	size_t		v_count 	= 0;
	size_t		vn_count 	= 0;
	size_t		vt_count 	= 0;
	ssize_t		read 		= 0;
	size_t		len 		= 0;

	if (stat(file, &st))
		die ("File not found.");
	
	if (st.st_mode & S_IFDIR)
		die ("Cannot open directory.");	

	if (!(fp = fopen(file, "r")))
		die("Cannot open file");

	parser.max = (t_vec3f){ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	parser.min = (t_vec3f){ FLT_MAX, FLT_MAX, FLT_MAX };

	memset(last_mtl, 0, 256);
	strcpy(last_mtl, "DEFAULT_NO_MATERIAL");
	get_relative_path(path, file);
	
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
			parser_mtl_start(scop, path, line + 7);
		else if (!strncmp(line, "usemtl ", 7))
		{
			memset(last_mtl, 0, 256);
			strcpy(last_mtl, _strtrim(line + 7));
		}
		else if (!strncmp(line, "v ", 2)) 
		{
			realloc_obj_arrays(&v_count, &parser.v_count, sizeof(t_vec3f), (void *) &parser.v);
			sscanf(line, "v %f %f %f", &parser.v[v_count].x, &parser.v[v_count].y, &parser.v[v_count].z);
			v_count++;
		} 
		else if (!strncmp(line, "vn ", 3)) 
		{
			realloc_obj_arrays(&vn_count, &parser.vn_count, sizeof(t_vec3f), (void *) &parser.vn);
			sscanf(line, "vn %f %f %f", &parser.vn[vn_count].x, &parser.vn[vn_count].y, &parser.vn[vn_count].z);
			vn_count++;
		}
		else if (!strncmp(line, "vt ", 3)) 
		{
			realloc_obj_arrays(&vt_count, &parser.vt_count, sizeof(t_vec2f), (void *) &parser.vt);
			sscanf(line, "vt %f %f", &parser.vt[vt_count].x, &parser.vt[vt_count].y);
			vt_count++;
		}
		else if (!strncmp(line, "f ", 2)) 
		{
			parse_face(&parser, &scop->objects[scop->objects_count].materials, &scop->objects[scop->objects_count].nb_mats, last_mtl, _strtrim(line + 2));
			f_count++;
		}
	}

	free_obj_arrays(&parser);
	free(line);
	line = NULL;
	fclose(fp);

	//center, max, min par objet a faire
	// scop->obects[scop->objects_count].center ...
	scop->center = vec_multf(vec_add(parser.min, parser.max), .5);
	scop->objects_count++;

}