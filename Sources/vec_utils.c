#include "Scop.h"

t_vec3f		vec_add(t_vec3f u, t_vec3f v)
{
	u.x = u.x + v.x;
	u.y = u.y + v.y;
	u.z = u.z + v.z;

    return (u);
}

t_vec3f		vec_sub(t_vec3f u, t_vec3f v)
{
	u.x = u.x - v.x;
	u.y = u.y - v.y;
	u.z = u.z - v.z;

    return (u);
}

t_vec3f		vec_multf(t_vec3f u, float f)
{
	u.x = u.x * f;
	u.y = u.y * f;
	u.z = u.z * f;

    return (u);
}

t_vec3f		vec_cross(t_vec3f u, t_vec3f v)
{
    u.x = u.y * v.z - u.z * v.y;
    u.y = u.z * v.x - u.x * v.z;
    u.z = u.x * v.y - u.y * v.x;

    return (u);
}
