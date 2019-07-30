#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS	300
#define COLS	300

void clear(unsigned char image[][COLS]);
void header(int row, int col, unsigned char head[32]);


int main(int argc, char** argv)
{

	FILE			*fp;
	unsigned char   head[32];
	unsigned char	image[ROWS][COLS];	
	double			alpha,p,q,L[ROWS][COLS],Ls,Ll,x,y;
	int             i,j,k;
	double			a[]= {0.5,0.5,0.5,0.5,0.5,0.1,1,0.5,0.5},
					m[]= {1,1,1,1,1,1,1,0.1,10000},
					r[]= {50,50,50,10,100,50,50,50,50},
					Sx[]= {0,1/1.732,1,0,0,0,0,0,0},
					Sy[]= {0,1/1.732,0,0,0,0,0,0,0},
					Sz[]= {1,1/1.732,0,1,1,1,1,1,1};
	double          V[3] = {0,0,1}, N[3],H[3];
	char            ch;
	char			filename[10][50], ifilename[50];
	
	strcpy( filename[0], "image1" );
	strcpy( filename[1], "image2" );
	strcpy( filename[2], "image3" );
	strcpy( filename[3], "image4" );
	strcpy( filename[4], "image5" );
	strcpy( filename[5], "image6" );
	strcpy( filename[6], "image7" );
	strcpy( filename[7], "image8" );
	strcpy( filename[8], "image9" );
	
	/* Create a header */
	header(ROWS, COLS, head);
	
	

	
for (k = 0; k < 9; k++)
{
	clear(image);
	for (i = 0; i<ROWS; i++)
	{
		for (j = 0; j<COLS; j++)
		{
		
			x =  j - COLS/2 ;
			y = - i + ROWS/2 ;
	
			if (pow(x, 2) + pow(y, 2) < pow(r[k], 2))
			{

				N[0] = x / r[k];
				N[2] = sqrt(r[k] * r[k] - (x*x + y * y)) / r[k];
				N[1] = y / r[k];

				//Ll = cos(theta);
				//theta > 0
				Ll = max(Sx[k] * N[0] + Sy[k] * N[1] + Sz[k] * N[2], 0);

				H[0] = (V[0]*Sx[k])/(sqrt(Sx[k]*Sx[k] + Sy[k]*Sy[k] + Sz[k]*Sz[k])*sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]));
    			H[1] = (V[1]*Sy[k])/(sqrt(Sx[k]*Sx[k] + Sy[k]*Sy[k] + Sz[k]*Sz[k])*sqrt(V[0] * V[0] + V[1] * V[1] + V[2] * V[2]));
    			H[2] = (V[2]*Sz[k])/(sqrt(Sx[k]*Sx[k] + Sy[k]*Sy[k] + Sz[k]*Sz[k])*sqrt(V[0] * V[0] + V[1] * V[1] + V[2] * V[2]));
	
				//alpha = acos(H[0]*N[0]+H[1]*N[1]+H[2]*N[2]);
				alpha = acos(max(H[0] * N[0] + H[1] * N[1] + H[2] * N[2], 0));
	
				Ls = exp(-pow(alpha/m[k],2));
	
	
				L[i][j] = a[k]*Ll + (1 - a[k])*Ls;
				//L[i][j] = a[k] * Ll;
				//L[i][j] =  (1 - a[k])*Ls;
	
			}	
	
			else
			{
				L[i][j] = 0;
			}
			
				image[i][j] = 255 * L[i][j];
	       
			//printf("image[%d][%d]=%d \n", i,j,image[i][j]);
		}
	}





		/* Save the original image as ras */
		strcpy(ifilename, filename[k]);
		if (!(fp = fopen(strcat(ifilename, "-o.ras"), "wb")))
		{
			fprintf(stderr, "error: could not open %s\n", ifilename);
			exit(1);
		}

	
		fwrite(head, 4, 8, fp);
		for (i = 0; i < ROWS; i++) fwrite(image[i], 1, COLS, fp);
		fclose(fp);

}
    
	printf("Press any key to exit: ");
	gets(&ch);

	return 0;
}



void clear(unsigned char image[][COLS])
{
	int	i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) image[i][j] = 0;
}



void header(int row, int col, unsigned char head[32])
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */

	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch++;
	head[6] = *ch;
	ch++;
	head[5] = *ch;
	ch++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch++;
	head[10] = *ch;
	ch++;
	head[9] = *ch;
	ch++;
	head[8] = *ch;

	ch = (char*)&num;
	head[19] = *ch;
	ch++;
	head[18] = *ch;
	ch++;
	head[17] = *ch;
	ch++;
	head[16] = *ch;


	/* Big-endian for unix */
	/*
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
*/
}
