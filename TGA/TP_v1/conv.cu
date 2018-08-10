#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/resource.h>
extern "C" {
  #include "bmp.h"
}

#define THREADS 16

__global__ void convKernel(int Nfil, int Ncol, unsigned char *d_img, unsigned char *d_out, int win_size) {
  int i = blockIdx.y * blockDim.y + threadIdx.y;
  int j = blockIdx.x * blockDim.x + threadIdx.x;
  int ind = i * Ncol + j;
 
  unsigned char *win = (unsigned char* )malloc(win_size * win_size * sizeof(unsigned char));
  if(win == NULL) {
    d_out[ind] = -1;
    return;
  }

  if (i < Nfil && j < Ncol) {
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

        win[(win_i+win_size/2) * win_size + win_j + win_size/2] = d_img[img_i * Ncol + img_j];
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
   
   free(win);
   // Retornar la mediana
   if(ind < Nfil * Ncol)  d_out[ind] = win[win_size * win_size / 2];
  }
}


void InitM(int Nfil, int Ncol, int *v);
void CheckCudaError(char sms[], int line);


int main(int argc, char** argv) {
  unsigned int Nfil, Ncol;
  unsigned int numBytes;
  unsigned int nThreads;

  unsigned char *h_img, *h_out;
  unsigned char *d_img, *d_out;

  bmpInfoHeader* h = (bmpInfoHeader*) malloc(sizeof(bmpInfoHeader));
  unsigned char* img = LoadBMP("test.bmp", h);
  Nfil = h->height;
  Ncol = h->width;
  int imgSize = Ncol * Nfil;
  numBytes = imgSize * sizeof(unsigned char);
  
  h_img = (unsigned char*) malloc(numBytes);
  h_out = (unsigned char*) malloc(numBytes);
  
  if(h_img == NULL || h_out == NULL) {
    printf("Not enought memory to allocate image");
    exit(0);
  } 
 
  // Inicializar imagen
  for(int i = 0; i < imgSize; ++i){
    h_img[i] = img[i*3];
  }

  // Obtener Memoria en el device
  cudaMalloc((unsigned char**)&d_img, numBytes); 
  cudaMalloc((unsigned char**)&d_out, numBytes); 
  CheckCudaError((char *) "Obtener Memoria en el device", __LINE__); 

  // Copiar imagen desde el host en el device 
  cudaMemcpy(d_img, h_img, numBytes, cudaMemcpyHostToDevice);
  CheckCudaError((char *) "Copiar Datos Host --> Device", __LINE__);

  //
  // KERNEL ELEMENTO a ELEMENTO
  //

  nThreads = THREADS;
  int nBlocksFil = (Nfil+nThreads-1)/nThreads;
  int nBlocksCol = (Ncol+nThreads-1)/nThreads;

  dim3 dimGridE(nBlocksCol, nBlocksFil, 1);
  dim3 dimBlockE(nThreads, nThreads, 1);

  printf("\n");
  printf("Kernel Elemento a Elemento\n");
  printf("Dimension problema: %d filas x %d columnas\n", Nfil, Ncol);
  printf("Dimension Block: %d x %d x %d (%d) threads\n", dimBlockE.x, dimBlockE.y, dimBlockE.z, dimBlockE.x * dimBlockE.y * dimBlockE.z);
  printf("Dimension Grid: %d x %d x %d (%d) blocks\n", dimGridE.x, dimGridE.y, dimGridE.z, dimGridE.x * dimGridE.y * dimGridE.z);

  // Ejecutar el kernel elemento a elemento
  convKernel<<<dimGridE, dimBlockE>>>(Nfil, Ncol, d_img, d_out, 7);
  CheckCudaError((char *) "Invocar Kernel", __LINE__);

  // Obtener el resultado desde el host 
  cudaMemcpy(h_out, d_out, numBytes, cudaMemcpyDeviceToHost); 
  CheckCudaError((char *) "Copiar Datos Device --> Host", __LINE__);

  // Guardar imagen
  for(int i = 0; i < imgSize; ++i){
    img[i*3] = h_out[i];
    img[i*3+1] = h_out[i];
    img[i*3+2] = h_out[i];
  }
  SaveBMP("out.bmp", h, img);

  // Liberar Memoria del device 
  cudaFree(d_img); cudaFree(d_out);

  cudaDeviceSynchronize();

  // Imprimimos imagen original vs imagen resultante
  printf("Imagen original:\n");
  for (int i=0; i < 20 ; i++)
	printf("%i\n", h_img[i]);
 
  printf("Imagen resultante:\n");
  for (int i=0; i < 20 ; i++)
	printf("%i\n", h_out[i]);

  printf("Imagen resultante:\n");
  for (int i=0; i < 60 ; i++)
	printf("%i\n", img[i]);
}

void InitM(int Nfil, int Ncol, int *M) {
  M[0] = 35;  
  M[1] = 35;  
  M[2] = 8;  
 
  M[3] = 35;  
  M[4] = 8;  
  M[5] = 35;  
 
  M[6] = 35;  
  M[7] = 35;  
  M[8] = 8;
}

void CheckCudaError(char sms[], int line) {
  cudaError_t error;

  error = cudaGetLastError();
  if (error) {
    printf("(ERROR) %s - %s in %s at line %d\n", sms, cudaGetErrorString(error), __FILE__, line);
    exit(EXIT_FAILURE);
  }
  //else printf("(OK) %s \n", sms);
}
