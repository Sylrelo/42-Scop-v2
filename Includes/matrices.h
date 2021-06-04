#ifndef MATRICES_H
# define MATRICES_H

#include <math.h>

typedef float mat4f[4][4];

void 		mat4_init(mat4f out)
{
	out[0][0] = 1.f;
	out[0][1] = 0.f;
	out[0][2] = 0.f;
	out[0][3] = 0.f;
	out[1][0] = 0.f;
	out[1][1] = 1.f;
	out[1][2] = 0.f;
	out[1][3] = 0.f;
	out[2][0] = 0.f;
	out[2][1] = 0.f;
	out[2][2] = 1.f;
	out[2][3] = 0.f;
	out[3][0] = 0.f;
	out[3][1] = 0.f;
	out[3][2] = 0.f;
	out[3][3] = 1.f;
}

void 	    mat4_mult(mat4f res, mat4f a, mat4f b)
{
	int i, j, k; 
    for (i = 0; i < 4; i++) 
    { 
        for (j = 0; j < 4; j++) 
        { 
            res[i][j] = 0; 
            for (k = 0; k < 4; k++) 
                res[i][j] += a[i][k] * b[k][j]; 
        } 
    } 
}


void		mat4_rotate(mat4f result, t_vec3f rot)
{
	mat4f	rot_x;
	mat4f	rot_y;
	mat4f	rot_z;
	mat4f	rot_xy;

	mat4_init(rot_x);
	mat4_init(rot_y);
	mat4_init(rot_z);
	rot_x[1][1] = cos(rot.x);
	rot_x[1][2] = -sin(rot.x);
	rot_x[2][1] = sin(rot.x);
	rot_x[2][2] = cos(rot.x);
	rot_y[0][0] = cos(rot.y);
	rot_y[0][2] = sin(rot.y);
	rot_y[2][0] = -sin(rot.y);
	rot_y[2][2] = cos(rot.y);
	rot_z[0][0] = cos(rot.z);
	rot_z[1][0] = -sin(rot.z);
	rot_z[0][1] = sin(rot.z);
	rot_z[1][1] = cos(rot.z);
	mat4_mult(rot_xy, rot_x, rot_y);
	mat4_mult(result, rot_xy, rot_z);
}

void		mat4_scale(mat4f result, t_vec3f scale)
{
	mat4_init(result);
	result[0][0] = scale.x;
	result[1][1] = scale.y;
	result[2][2] = scale.z;
}

void		mat4_translate(mat4f result, t_vec3f pos)
{
	mat4_init(result);
	result[3][0] = pos.x;
	result[3][1] = pos.y;
	result[3][2] = pos.z;
}

void		mat4_perspective(mat4f result, float fov, float aspect, float near_plane, float far_plane)
{
	float	tan_half_fov;
	
	mat4_init(result);
	tan_half_fov = tanf(fov / 2.0f);
	result[0][0] = 1.0f / (aspect * tan_half_fov);
	result[1][1] = 1.0f / (tan_half_fov);
	result[2][2] = (far_plane + near_plane) / (far_plane - near_plane) * -1;
	result[2][3] = -1.0f;
	result[3][2] = (2.0f * far_plane * near_plane) / (far_plane - near_plane) * -1;
}

void		mat4_cpy(mat4f dest, mat4f src)
{
	int	i;
	int	j;

	if (!dest || !src)
		return ;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			dest[i][j] = src[i][j];
	}
}

void		mat4_empty(mat4f mat)
{
	int		i;
	int		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			mat[i][j] = 0.0;
	}
}


void	matmat(mat4f res, t_vec3f t, t_vec3f r, int scale)
{
	mat4f	tt;
	mat4f	tr;
	mat4f	ts;
	mat4f	tmp;

	mat4_rotate(tr, r);
	mat4_scale(ts, (t_vec3f){scale, scale, scale});
	mat4_translate(tt, t);
	mat4_mult(tmp, ts, tr);
	mat4_mult(res, tmp, tt);
}

void	printmat(mat4f mat)
{
	int			i;
	i = -1;
	while (++i < 4)
	{
		printf("%.2f %.2f %.2f %.2f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
	}
	printf("\n");

}

#endif