
// #include <Scop.h>

#include <stdio.h>
#include <stdlib.h>


#define WORD 2
#define DWORD 4
#define LONG 4

typedef struct 		s_bitmap
{
	uint32_t 		bfSize;
	uint32_t 		bfOffBits;
	uint32_t 		biSize;
	uint32_t 		biWidth;
	uint32_t 		biHeight;
	uint32_t 		biBitCount;
	uint32_t 		biSizeImage;
	unsigned char 	*content;
} 					t_bitmap;

static int read_bytes(FILE *fp, uint32_t byte_len)
{
	int total;

	total = 0;

	for (unsigned int i = 0; i < byte_len; i++)
	{
		total += (fgetc(fp) << (8 * i));
	}

	return total;
}

int main(void)
{
	FILE 		*fp;
	t_bitmap	bmp;

	int 		byte;
	long int 	cur;

	fp = fopen("./Resources/Textures/nyancat.bmp", "r");
	printf("%p\n", fp);

	while ((byte = fgetc(fp)) != EOF)
	{
		cur = ftell(fp);
		if (cur == 35)
		{
			// dprintf(1, "%d ", byte+ read_bytes(fp, 3));
			bmp.biSizeImage = byte + read_bytes(fp, 3);
		}
	}

	printf("bmp size : %u\n", bmp.biSizeImage);
}




