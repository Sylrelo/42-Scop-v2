#ifndef MATRICES_H
# define MATRICES_H

#include <math.h>

typedef float mat4f[4][4];

typedef struct 	s_mat4
{
	float		value[4][4];
}				t_mat4;

t_mat4		m4_init(void)
{
	t_mat4 out;

	out.value[0][0] = 1.f;
	out.value[0][1] = 0.f;
	out.value[0][2] = 0.f;
	out.value[0][3] = 0.f;
	out.value[1][0] = 0.f;
	out.value[1][1] = 1.f;
	out.value[1][2] = 0.f;
	out.value[1][3] = 0.f;
	out.value[2][0] = 0.f;
	out.value[2][1] = 0.f;
	out.value[2][2] = 1.f;
	out.value[2][3] = 0.f;
	out.value[3][0] = 0.f;
	out.value[3][1] = 0.f;
	out.value[3][2] = 0.f;
	out.value[3][3] = 1.f;

	return (out);
}

t_mat4 	    m4_mult(t_mat4 a, t_mat4 b)
{
	t_mat4 	res;
	int		i;
	int		j;
	int		k; 
	
    for (i = 0; i < 4; i++) 
    { 
        for (j = 0; j < 4; j++) 
        { 
            res.value[i][j] = 0; 
            for (k = 0; k < 4; k++) 
                res.value[i][j] += a.value[i][k] * b.value[k][j]; 
        } 
    }

	return (res);
}

t_mat4		m4_rotation(float x, float y, float z)
{
	const float cosx	= cos(x);
	const float cosy	= cos(y);
	const float cosz	= cos(z);
	const float sinx	= sin(x);
	const float siny	= sin(y);
	const float sinz	= sin(z);
	t_mat4	rot_x		= m4_init();
	t_mat4	rot_y		= m4_init();
	t_mat4	rot_z		= m4_init();
	t_mat4	rot_xy;


	rot_x.value[1][1] = cosx;
	rot_x.value[1][2] = -sinx;
	rot_x.value[2][1] = sinx;
	rot_x.value[2][2] = cosx;

	rot_y.value[0][0] = cosy;
	rot_y.value[0][2] = siny;
	rot_y.value[2][0] = -siny;
	rot_y.value[2][2] = cosy;

	rot_z.value[0][0] = cosz;
	rot_z.value[1][0] = -sinz;
	rot_z.value[0][1] = sinz;
	rot_z.value[1][1] = cosz;

	rot_xy = m4_mult(rot_x, rot_y);
	return m4_mult(rot_xy, rot_z);
}

t_mat4		m4_scale(float x, float y, float z)
{
	t_mat4 result = m4_init();

	result.value[0][0] = x;
	result.value[1][1] = y;
	result.value[2][2] = z;

	return (result);
}

t_mat4		m4_translate(float x, float y, float z)
{
	t_mat4 result = m4_init();

	result.value[3][0] = x;
	result.value[3][1] = y;
	result.value[3][2] = z;

	return (result);
}

t_mat4		m4_perspective(float fov, float aspect, float near, float far)
{
	t_mat4 result 		= m4_init();
	float tan_half_fov 	= tanf(fov * .5f);;
	
	result.value[0][0] = 1.0f / (aspect * tan_half_fov);
	result.value[1][1] = 1.0f / (tan_half_fov);
	result.value[2][2] = (far + near) / (far - near) * -1;
	result.value[2][3] = -1.0f;
	result.value[3][2] = (2.0f * far * near) / (far - near) * -1;

	return (result);
}

t_mat4		m4_rotation_around_center(t_vec3f center, float x, float y, float z)
{
	const t_vec3f	inverse_center 	= vec_multf(center, -1);
	const t_mat4	trans_center 	= m4_translate(inverse_center.x, inverse_center.y, inverse_center.z);
	const t_mat4	trans_origin	= m4_translate(center.x, center.y, center.z);
	const t_mat4	rotation		= m4_rotation(x, y, z);
	t_mat4			result;


	result = m4_mult(trans_center, rotation);
	result = m4_mult(result, trans_origin);

	return (result);
}

t_mat4		m4_mult3(t_mat4 rotation, t_mat4 scale, t_mat4 translate)
{
	return (m4_mult(m4_mult(scale, rotation), translate));
}

void		m4_print(t_mat4 mat)
{
	int			i;

	i = -1;
	while (++i < 4)
	{
		printf("%.2f %.2f %.2f %.2f\n", mat.value[i][0], mat.value[i][1], mat.value[i][2], mat.value[i][3]);
	}
	printf("\n");

}

#endif