
#include "Libmatvec.h"
#include <math.h>

t_vec3f	m4_mult_vec3f(t_mat4 mat, t_vec3f vec)
{
	t_vec3f result = (t_vec3f) {0, 0, 0};

	result.x = vec.x * mat.value[0][0] + vec.y * mat.value[0][1] + vec.z * mat.value[0][2];
	result.y = vec.x * mat.value[1][0] + vec.y * mat.value[1][1] + vec.z * mat.value[1][2];
	result.z = vec.x * mat.value[2][0] + vec.y * mat.value[2][1] + vec.z * mat.value[2][2];

	return (result);
}