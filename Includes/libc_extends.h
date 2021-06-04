// Header-only (https://en.wikipedia.org/wiki/Header-only) to extend libc

#ifndef LIBC_EXTENDS
# define LIBC_EXTENDS

inline size_t	strcountchr(char *line, char character)
{
	size_t	count = 0;

	while (*line++)
		if (*line == character)
			count++;
	return (count);
}

inline void		_floatset(float *dest, const float value, const size_t len)
{
	size_t	i = 0;

	while (i < len)
	{
		dest[i] = value;
		i++;
	}
}

inline void		_floatncat(float *dest, const float *values, const size_t start, const size_t len)
{
	size_t	i = start;
	size_t	j = 0;

	while (i < start + len)
	{
		dest[i] = values[j];
		i++;
		j++;
	}
}

#endif