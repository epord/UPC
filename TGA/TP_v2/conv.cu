#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/resource.h>
extern "C" {
  #include "bmp.h"
}

#define THREADS 16

__global__ void convKernel(int Nfil, int Ncol, unsigned char *d_img, unsigned char *d_out, int win_size, unsigned char *d_win) {
  int i = blockIdx.y * blockDim.y + threadIdx.y;
  int j = blockIdx.x * blockDim.x + threadIdx.x;
  int ind = i * Ncol + j;

   //d_out[ind] = 100;//win[win_size * win_size / 2];
  unsigned char *win = (unsigned char* )malloc(win_size * win_size * sizeof(unsigned char));
 // printf("asdf");
  if(win == NULL) {
    d_out[ind] = -1;
    return;
  }
 
  //d_out[ind] = d_img[ind];
  //return;

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
   
   // Retornar la mediana
   if(ind < Nfil * Ncol){
//	printf("yay: %d \n", win[win_size*win_size/2]); 
	 d_out[ind] = win[win_size * win_size / 2];
   }
   free(win);
   return;
  }
}


void InitM(int Nfil, int Ncol, int *v);
void CheckCudaError(char sms[], int line);


int main(int argc, char** argv) {
  unsigned int Nfil, Ncol;
  unsigned int numBytes;
  unsigned int nThreads;
  unsigned int winSize = 3;

  unsigned char *h_img1, *h_img2, *h_img3, *h_img4, *h_out1, *h_out2, *h_out3, *h_out4;
  unsigned char *d_img1, *d_img2, *d_img3, *d_img4, *d_out1, *d_out2, *d_out3, *d_out4, *d_win1, *d_win2, *d_win3, *d_win4;

  bmpInfoHeader* h = (bmpInfoHeader*) malloc(sizeof(bmpInfoHeader));
  unsigned char* img = LoadBMP("test.bmp", h);
  Nfil = h->height;
  Ncol = h->width;
  int imgSize = Ncol * Nfil;
  numBytes = imgSize * sizeof(unsigned char) / 4;

  int count;
  cudaGetDeviceCount(&count);
  if(count <  4) { printf("ERROR: no hay suficientes GPUs\n"); exit(0);}  

  cudaMallocHost((unsigned char **)&h_img1, numBytes);
  cudaMallocHost((unsigned char **)&h_img2, numBytes);
  cudaMallocHost((unsigned char **)&h_img3, numBytes);
  cudaMallocHost((unsigned char **)&h_img4, numBytes);
  cudaMallocHost((unsigned char **)&h_out1, numBytes);
  cudaMallocHost((unsigned char **)&h_out2, numBytes);
  cudaMallocHost((unsigned char **)&h_out3, numBytes);
  cudaMallocHost((unsigned char **)&h_out4, numBytes);
  
  if(h_img1 == NULL || h_img2 == NULL || h_img3 == NULL || h_img4 == NULL || h_out1 == NULL || h_out2 == NULL || h_out3 == NULL || h_out4 == NULL) {
    printf("Not enought memory to allocate image");
    exit(0);
  } 
 
  // Inicializar imagen dividida en 4
  int indCol = 0;
  int indFil = 0;
  int baseIndex;
  for(int i = 0; i < imgSize; ++i){
    indCol = i%Ncol;
    indFil = i/Ncol;
    baseIndex = indCol + indFil*Ncol/2;
    if(indCol < Ncol/2 && indFil < Nfil/2){
      h_img1[baseIndex] = img[i*3];
    } else if(indCol >= Ncol/2 && indFil < Nfil/2){
      h_img2[baseIndex-Ncol/2] = img[i*3];
    } else if(indCol < Ncol/2 && indFil >= Nfil/2){
      h_img3[baseIndex-Nfil*Ncol/4] = img[i*3];
    } else if(indCol >= Ncol/2 && indFil >= Nfil/2){
      h_img4[baseIndex-Nfil*Ncol/4-Ncol/2] = img[i*3];
    }
  }

  for(int i = 0; i < 50; ++i){
    printf("%i ",h_img1[i]);
  }
  printf("\n");

  for(int i = 0; i < 50; ++i){
    printf("%i ",h_img2[i]);
  }
  printf("\n");
  for(int i = 0; i < 50; ++i){
    printf("%i ",h_img3[i]);
  }
  printf("\n");
  for(int i = 0; i < 50; ++i){
    printf("%i ",h_img4[i]);
  }
  printf("\n");
  // Obtener Memoria en el device
  cudaSetDevice(0);
  cudaMalloc((unsigned char**)&d_img1, numBytes); 
  cudaMalloc((unsigned char**)&d_out1, numBytes);
  cudaMalloc((unsigned char**)&d_win1, winSize*winSize*sizeof(unsigned char));
  
  cudaSetDevice(1);
  cudaMalloc((unsigned char**)&d_img2, numBytes); 
  cudaMalloc((unsigned char**)&d_out2, numBytes); 
  cudaMalloc((unsigned char**)&d_win2, winSize*winSize*sizeof(unsigned char));
  
  cudaSetDevice(2);
  cudaMalloc((unsigned char**)&d_img3, numBytes); 
  cudaMalloc((unsigned char**)&d_out3, numBytes); 
  cudaMalloc((unsigned char**)&d_win3, winSize*winSize*sizeof(unsigned char));
  
  cudaSetDevice(3);
  cudaMalloc((unsigned char**)&d_img4, numBytes); 
  cudaMalloc((unsigned char**)&d_out4, numBytes); 
  cudaMalloc((unsigned char**)&d_win4, winSize*winSize*sizeof(unsigned char));
  CheckCudaError((char *) "Obtener Memoria en el device", __LINE__); 

  //
  // KERNEL ELEMENTO a ELEMENTO
  //
  nThreads = THREADS;
  int nBlocksFil = (Nfil/2+nThreads-1)/nThreads;
  int nBlocksCol = (Ncol/2+nThreads-1)/nThreads;

  dim3 dimGridE(nBlocksCol, nBlocksFil, 1);
  dim3 dimBlockE(nThreads, nThreads, 1);


  // Copiar imagen desde el host en el device 
  cudaSetDevice(0);
  cudaMemcpyAsync(d_img1, h_img1, numBytes, cudaMemcpyHostToDevice);
  CheckCudaError((char *) "Copiar Datos Host --> Device", __LINE__);
 // cudaDeviceSetLimit(cudaLimitMallocHeapSize, winSize * winSize * sizeof(unsigned char));
  convKernel<<<dimGridE, dimBlockE>>>(Nfil/2, Ncol/2, d_img1, d_out1, winSize, d_win1);
  CheckCudaError((char *) "Invocar Kernel", __LINE__);
  
  cudaSetDevice(1);
  cudaMemcpyAsync(d_img2, h_img2, numBytes, cudaMemcpyHostToDevice);
  CheckCudaError((char *) "Copiar Datos Host --> Device", __LINE__);
//  cudaDeviceSetLimit(cudaLimitMallocHeapSize, winSize * winSize * sizeof(unsigned char));
  convKernel<<<dimGridE, dimBlockE>>>(Nfil/2, Ncol/2, d_img2, d_out2, winSize, d_win2);
  CheckCudaError((char *) "Invocar Kernel", __LINE__);

  cudaSetDevice(2);
  cudaMemcpyAsync(d_img3, h_img3, numBytes, cudaMemcpyHostToDevice);
  CheckCudaError((char *) "Copiar Datos Host --> Device", __LINE__);
//  cudaDeviceSetLimit(cudaLimitMallocHeapSize, winSize * winSize * sizeof(unsigned char));
  convKernel<<<dimGridE, dimBlockE>>>(Nfil/2, Ncol/2, d_img3, d_out3, winSize, d_win3);
  CheckCudaError((char *) "Invocar Kernel", __LINE__);

  cudaSetDevice(3);
  cudaMemcpyAsync(d_img4, h_img4, numBytes, cudaMemcpyHostToDevice);
  CheckCudaError((char *) "Copiar Datos Host --> Device", __LINE__);
//  cudaDeviceSetLimit(cudaLimitMallocHeapSize, winSize * winSize * sizeof(unsigned char));
  convKernel<<<dimGridE, dimBlockE>>>(Nfil/2, Ncol/2, d_img4, d_out4, winSize, d_win4);
  CheckCudaError((char *) "Invocar Kernel", __LINE__);
  cudaSetDevice(0); cudaDeviceSynchronize();
  cudaSetDevice(1); cudaDeviceSynchronize();
  cudaSetDevice(2); cudaDeviceSynchronize();
  cudaSetDevice(3); cudaDeviceSynchronize();

  printf("\n");
  printf("Kernel Elemento a Elemento\n");
  printf("Dimension problema: %d filas x %d columnas\n", Nfil, Ncol);
  printf("Dimension Block: %d x %d x %d (%d) threads\n", dimBlockE.x, dimBlockE.y, dimBlockE.z, dimBlockE.x * dimBlockE.y * dimBlockE.z);
  printf("Dimension Grid: %d x %d x %d (%d) blocks\n", dimGridE.x, dimGridE.y, dimGridE.z, dimGridE.x * dimGridE.y * dimGridE.z);

  // Ejecutar el kernel elemento a elemento
  //convKernel<<<dimGridE, dimBlockE>>>(Nfil, Ncol, d_img, d_out, 7);
  //CheckCudaError((char *) "Invocar Kernel", __LINE__);

  // Obtener el resultado desde el host 
  cudaSetDevice(0);
  cudaMemcpyAsync(h_out1, d_out1, numBytes, cudaMemcpyDeviceToHost); 
  CheckCudaError((char *) "Copiar Datos Device --> Host", __LINE__);
  cudaSetDevice(1);
  cudaMemcpyAsync(h_out2, d_out2, numBytes, cudaMemcpyDeviceToHost); 
  CheckCudaError((char *) "Copiar Datos Device --> Host", __LINE__);
  cudaSetDevice(2);
  cudaMemcpyAsync(h_out3, d_out3, numBytes, cudaMemcpyDeviceToHost); 
  CheckCudaError((char *) "Copiar Datos Device --> Host", __LINE__);
  cudaSetDevice(3);
  cudaMemcpyAsync(h_out4, d_out4, numBytes, cudaMemcpyDeviceToHost); 
  CheckCudaError((char *) "Copiar Datos Device --> Host", __LINE__);

  for(int i = 0; i < 10; ++i){
    printf("%i ",h_out1[i]);
  }
  printf("\n");

  for(int i = 0; i < 10; ++i){
    printf("%i ",h_out2[i]);
  }
  printf("\n");
  for(int i = 0; i < 10; ++i){
    printf("%i ",h_out3[i]);
  }
  printf("\n");
  for(int i = 0; i < 10; ++i){
    printf("%i ",h_out4[i]);
  }
  printf("\n");
  // Guardar imagen
  for(int i = 0; i < imgSize/4; ++i){
    indCol = i%(Ncol/2);
    indFil = i/(Ncol/2);
    baseIndex = indCol + indFil * Ncol;
    // Abajo a la izquierda
    img[baseIndex*3] = h_out1[i];
    img[baseIndex*3+1] = h_out1[i];
    img[baseIndex*3+2] = h_out1[i];

    // Abajo a la derecha    
    img[(baseIndex+Ncol/2)*3] = h_out2[i];
    img[(baseIndex+Ncol/2)*3+1] = h_out2[i];
    img[(baseIndex+Ncol/2)*3+2] = h_out2[i];

    // Arriba a la izquierda    
    img[(baseIndex+Nfil/2*Ncol)*3] = h_out3[i];
    img[(baseIndex+Nfil/2*Ncol)*3+1] = h_out3[i];
    img[(baseIndex+Nfil/2*Ncol)*3+2] = h_out3[i];
    
    // Arriba a la derecha
    img[(baseIndex+Nfil/2*Ncol+Ncol/2)*3] = h_out4[i];
    img[(baseIndex+Nfil/2*Ncol+Ncol/2)*3+1] = h_out4[i];
    img[(baseIndex+Nfil/2*Ncol+Ncol/2)*3+2] = h_out4[i];
  }
  SaveBMP("out.bmp", h, img);

  // Liberar Memoria del device 
  cudaSetDevice(0); cudaFree(d_img1); cudaFree(d_out1);
  cudaSetDevice(1); cudaFree(d_img2); cudaFree(d_out2);
  cudaSetDevice(2); cudaFree(d_img3); cudaFree(d_out3);
  cudaSetDevice(3); cudaFree(d_img4); cudaFree(d_out4);

  cudaDeviceSynchronize();

  // Imprimimos imagen original vs imagen resultante
 // printf("Imagen original:\n");
 // for (int i=0; i < 20 ; i++)
//	printf("%i\n", h_img[i]);
 
 // printf("Imagen resultante:\n");
  //for (int i=0; i < 20 ; i++)
//	printf("%i\n", h_out[i]);

  //printf("Imagen resultante:\n");
  //for (int i=0; i < 60 ; i++)
//	printf("%i\n", img[i]);
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
