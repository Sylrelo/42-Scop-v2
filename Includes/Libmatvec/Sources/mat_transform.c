
#include "Libmatvec.h"
#include <math.h>

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

	return m4_mult(m4_mult(rot_x, rot_y), rot_z);
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