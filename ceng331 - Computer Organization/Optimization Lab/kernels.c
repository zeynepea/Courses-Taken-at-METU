/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
 /*
  * Please fill in the following student struct
  */
team_t team = {
	"e2171577",              /* Student ID */

	"Zeynep Erdogan",     /* full name */
	"e2171577@metu.edu.tr",  /* email address */

	"",                   /* leave blank */
	""                    /* leave blank */
};


/***************
 * Sobel KERNEL
 ***************/

 /******************************************************
  * Your different versions of the sobel functions  go here
  ******************************************************/

  /*
   * naive_sobel - The naive baseline version of Sobel
   */
char naive_sobel_descr[] = "sobel: Naive baseline implementation";
void naive_sobel(int dim, int *src, int *dst) {
	int i, j, k, l;
	int ker[3][3] = { {-1, 0, 1},
					 {-2, 0, 2},
					 {-1, 0, 1} };
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			dst[j*dim + i] = 0;
			if (!((i == 0) || (i == dim - 1) || (j == 0) || (j == dim - 1))) {
				for (k = -1; k <= 1; k++)
					for (l = -1; l <= 1; l++) {
						dst[j*dim + i] = dst[j*dim + i] + src[(j + l)*dim + (i + k)] * ker[(l + 1)][(k + 1)];
					}
			}
		}
}
/*
 * sobel - Your current working version of sobel
 * IMPORTANT: This is the version you will be graded on
 */


char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim, int *src, int *dst)
{
	int x1, temp, temp2, b = 0;
	int a = dim * dim -1;
	for (int i = 0; i < dim; i++){
		dst[i] = 0;
		
	}
	for (int i = 0; i < dim; i++){
		
		dst[a-i] = 0;
	}
	for (int i = 1; i < dim-1; i++){
		dst[i*dim] = 0;
	
	}
	for (int i = 1; i < dim-1; i++){
		
		dst[(i+1)*dim-1] = 0;
	}
	for (int i = 1; i < dim - 1; i++) {
		b = i * dim;
		for (int j = 1; j < dim - 2; j = j + 2) {
			x1 = b + j;
			temp = - src[x1 - dim - 1];
			temp2 = - src[x1 - dim];
			temp = temp - src[x1 - 1] * 2;
			temp2 = temp2 - src[x1] * 2;
			temp = temp - src[x1 + dim - 1];
			temp2 = temp2 - src[x1 + dim];
			temp = temp + src[x1 - dim + 1];
			temp2 = temp2 + src[x1 - dim + 2];
			temp = temp + src[x1 + 1] * 2;
			temp2 = temp2 + src[x1 + 2] * 2;
			temp = temp + src[x1 + dim + 1];
			temp2 = temp2 + src[x1 + dim + 2];
			dst[x1] = temp;
			dst[x1 + 1] = temp2;
		}
	}

}

/*********************************************************************
 * register_sobel_functions - Register all of your different versions
 *     of the sobel functions  with the driver by calling the
 *     add_sobel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_sobel_functions() {
	add_sobel_function(&naive_sobel, naive_sobel_descr);
	add_sobel_function(&sobel, sobel_descr);
	/* ... Register additional test functions here */
}




/***************
 * MIRROR KERNEL
 ***************/

 /******************************************************
  * Your different versions of the mirror func  go here
  ******************************************************/

  /*
   * naive_mirror - The naive baseline version of mirror
   */
char naive_mirror_descr[] = "Naive_mirror: Naive baseline implementation";
void naive_mirror(int dim, int *src, int *dst) {

	int i, j;

	for (j = 0; j < dim; j++)
		for (i = 0; i < dim; i++) {
			dst[RIDX(j, i, dim)] = src[RIDX(j, dim - 1 - i, dim)];
		}

}


/*
 * mirror - Your current working version of mirror
 * IMPORTANT: This is the version you will be graded on
 */
char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim, int *src, int *dst)
{
	int a, b, c, d;
	if(dim % 128 == 0){
		for (int j = 0; j < dim; j++) {
			a = dim - 127;
			b = j * dim;
			for (int i = 0; i < a; i = i + 128) {
				c = b + i;
				d = b + dim - i;
				dst[c]    = src[--d];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
			}
		}
	}
	else if(dim % 64 == 0){
		for (int j = 0; j < dim; j++) {
			a = dim - 63;
			b = j * dim;
			for (int i = 0; i < a; i = i + 64) {
				c = b + i;
				d = b + dim - i;
				dst[c] = src[--d];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
				dst[++c ] = src[--d ];
			}
		}
	}
	else{
		for (int j = 0; j < dim; j++) {
			a = dim - 31;
			b = j * dim;
			for (int i = 0; i < a; i = i + 32) {
				c = b + i;
				d = b + dim - i;
				dst[c] =    src[d-1];
				dst[c+1 ] = src[d- 2];
				dst[c+2 ] = src[d- 3];
				dst[c+3 ] = src[d-4 ];
				dst[c+4 ] = src[d-5 ];
				dst[c+5 ] = src[d-6 ];
				dst[c+6 ] = src[d-7 ];
				dst[c+7 ] = src[d-8 ];
				dst[c+8 ] = src[d-9 ];
				dst[c+9 ] = src[d-10 ];
				dst[c+10 ] = src[d-11 ];
				dst[c+11 ] = src[d-12 ];
				dst[c+12 ] = src[d-13 ];
				dst[c+13 ] = src[d-14 ];
				dst[c+14 ] = src[d-15 ];
				dst[c+15 ] = src[d-16 ];
				dst[c+16 ] = src[d-17 ];
				dst[c+17 ] = src[d-18 ];
				dst[c+18 ] = src[d-19 ];
				dst[c+19 ] = src[d-20 ];
				dst[c+20 ] = src[d-21 ];
				dst[c+21 ] = src[d-22 ];
				dst[c+22 ] = src[d-23 ];
				dst[c+23 ] = src[d-24 ];
				dst[c+24 ] = src[d-25 ];
				dst[c+25 ] = src[d-26 ];
				dst[c+26 ] = src[d-27 ];
				dst[c+27 ] = src[d-28 ];
				dst[c+28 ] = src[d-29 ];
				dst[c+29 ] = src[d-30 ];
				dst[c+30 ] = src[d-31 ];
				dst[c+31 ] = src[d-32 ];
			}
		}
	}
}





/*********************************************************************
 * register_mirror_functions - Register all of your different versions
 *     of the mirror functions  with the driver by calling the
 *     add_mirror_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_mirror_functions() {
	add_mirror_function(&naive_mirror, naive_mirror_descr);
	add_mirror_function(&mirror, mirror_descr);

	/* ... Register additional test functions here */
}

