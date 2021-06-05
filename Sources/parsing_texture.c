#include "Scop.h"

#include <stdio.h>

static void generate_filepath(char filepath[256], char path[256], char *file)
{
    file = _strtrim(file);
	memset(filepath, 0, 256);
    strcat(filepath, path);
    strcat(filepath, file);
    filepath[strlen(path) + strlen(file)] = 0;
}

void    parse_texture(t_scop *scop, t_mat *material, char path[256], char *file)
{
    char    filepath[256];

    generate_filepath(filepath, path, file);

    printf("%s %s - %s\n", path, file, filepath);
    (void)scop;
    (void)material;
    // exit (1);
}