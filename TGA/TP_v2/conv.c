#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

void calculateBWPixelValue(int i, int j, unsigned char* img, unsigned char* img_out, bmpInfoHeader* h, int win_size) {
	unsigned char *win = (unsigned char* ) malloc(win_size * win_size * sizeof(unsigned char));
	unsigned char out_val;
	if(win == NULL) {
	    exit(1);
  	}

  	int Nfil = h->height;
  	int Ncol = h->width;
  	int ind = i * Ncol + j;

  	// Filling the window
    for(int win_i = -win_size/2; win_i <= win_size/2; win_i++) {
      for(int win_j = -win_size/2; win_j <= win_size/2; win_j++) {
        // Si esta fuera, extender el ultimo valor
        int img_i, img_j;
        if(i+win_i < 0)
          img_i = 0;
        else if(i+win_i >= Nfil)
           img_i = Nfil -1;
        else
           img_i = i + win_i;  
        
        if(j+win_j < 0)
          img_j = 0;
        else if(j+win_j >= Ncol)
           img_j = Ncol -1;
        else
           img_j = j + win_j;  

        win[(win_i+win_size/2) * win_size + win_j + win_size/2] = img[(img_i * Ncol + img_j)*3];
      } 
    }

    // Ordenar valores dentro de la ventana
    int swap;
    for (int c = 0 ; c < win_size * win_size - 1; c++) {
      for (int d = 0 ; d < win_size * win_size - c - 1; d++) {
        if (win[d] > win[d+1]) {
          swap   = win[d];
          win[d]   = win[d+1];
          win[d+1] = swap;
        }
      }
    }

   // Retornar la mediana
	img_out[ind*3] = win[win_size * win_size / 2];
	img_out[ind*3 + 1] = win[win_size * win_size / 2];
	img_out[ind*3 + 2] = win[win_size * win_size / 2];
	free(win);
}

void calculateColoredPixelValue(int i, int j, unsigned char* img, unsigned char* img_out, bmpInfoHeader* h, int win_size) {
	unsigned char *win_r = (unsigned char* ) malloc(win_size * win_size * sizeof(unsigned char));
	unsigned char *win_g = (unsigned char* ) malloc(win_size * win_size * sizeof(unsigned char));
	unsigned char *win_b = (unsigned char* ) malloc(win_size * win_size * sizeof(unsigned char));
	unsigned char out_val;
	if(win_r == NULL || win_g == NULL || win_b == NULL) {
	    exit(1);
  }

	int Nfil = h->height;
	int Ncol = h->width;
	int ind = i * Ncol + j;

	// Filling the window
	for(int win_i = -win_size/2; win_i <= win_size/2; win_i++) {
	  for(int win_j = -win_size/2; win_j <= win_size/2; win_j++) {
	    // Si esta fuera, extender el ultimo valor
	    int img_i, img_j;
	    if(i+win_i < 0)
	      img_i = 0;
	    else if(i+win_i >= Nfil)
	       img_i = Nfil -1;
	    else
	       img_i = i + win_i;  
	    
	    if(j+win_j < 0)
	      img_j = 0;
	    else if(j+win_j >= Ncol)
	       img_j = Ncol -1;
	    else
	       img_j = j + win_j;  

	    win_r[(win_i+win_size/2) * win_size + win_j + win_size/2] = img[(img_i * Ncol + img_j)*3];
	    win_g[(win_i+win_size/2) * win_size + win_j + win_size/2] = img[(img_i * Ncol + img_j)*3+1];
	    win_b[(win_i+win_size/2) * win_size + win_j + win_size/2] = img[(img_i * Ncol + img_j)*3+2];
	  } 
	}


	// Ordenar valores dentro de cada ventana
	int swap;
	for (int c = 0 ; c < win_size * win_size - 1; c++) {
	  for (int d = 0 ; d < win_size * win_size - c - 1; d++) {
	    if (win_r[d] > win_r[d+1]) {
	      swap   = win_r[d];
	      win_r[d]   = win_r[d+1];
	      win_r[d+1] = swap;
	    }
	  }
	}

	for (int c = 0 ; c < win_size * win_size - 1; c++) {
	  for (int d = 0 ; d < win_size * win_size - c - 1; d++) {
	    if (win_g[d] > win_g[d+1]) {
	      swap   = win_g[d];
	      win_g[d]   = win_g[d+1];
	      win_g[d+1] = swap;
	    }
	  }
	}

	for (int c = 0 ; c < win_size * win_size - 1; c++) {
	  for (int d = 0 ; d < win_size * win_size - c - 1; d++) {
	    if (win_b[d] > win_b[d+1]) {
	      swap   = win_b[d];
	      win_b[d]   = win_b[d+1];
	      win_b[d+1] = swap;
	    }
	  }
	}


   // Retornar la mediana
  img_out[ind*3] = win_r[win_size * win_size / 2];
  img_out[ind*3 + 1] = win_g[win_size * win_size / 2];
  img_out[ind*3 + 2] = win_b[win_size * win_size / 2];
	free(win_r);
	free(win_g);
	free(win_b);
}

int main(int argc, char *argv[]) {
	int win_size = 5;
  char file_name[50];
  char isColored = 1;
  strcpy(file_name, "test.bmp");
	if( argc >= 2 ) {
		printf("1\n");
		win_size = atoi(argv[1]);
	}
	if( argc >= 3 ) {
		printf("2\n");
  	strcpy(file_name, argv[2]);
	}
	if( argc >= 4 ) {
		printf("3\n");
		isColored = atoi(argv[3]);
	}

  bmpInfoHeader *h;
  h = (bmpInfoHeader*) malloc(sizeof (bmpInfoHeader));
  unsigned char* img = LoadBMP(file_name, h);
  int pixel_count = h->height * h->width;
  printf("%i %i %i\n", h->height, h->width, h->imgsize);
  unsigned char* img_out = (unsigned char* ) malloc(h->imgsize * sizeof(unsigned char));

  for(int i = 0; i < pixel_count; i++) {
  	if (isColored) calculateColoredPixelValue(i / h->width, i % h->width, img, img_out, h, win_size);
  	else calculateBWPixelValue(i / h->width, i % h->width, img, img_out, h, win_size);
  }

  char* out_file_name = (char*) malloc(strlen(file_name) + 12);
  strcpy(out_file_name, "out_");
  strcat(out_file_name, file_name);
  SaveBMP(out_file_name, h, img_out);

  free(h);
  free(img);
  free(img_out);
}