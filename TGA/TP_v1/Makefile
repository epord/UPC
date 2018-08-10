CUDA_HOME   = /Soft/cuda/8.0.61

NVCC        = $(CUDA_HOME)/bin/nvcc
NVCC_FLAGS  = -O3 -I$(CUDA_HOME)/include -I$(CUDA_HOME)/sdk/CUDALibraries/common/inc 
LD_FLAGS    = -lcudart -Xlinker -rpath,$(CUDA_HOME)/lib64 -I$(CUDA_HOME)/sdk/CUDALibraries/common/lib

EXE	        = conv.exe
OBJ	        = conv.o

default: $(EXE)

conv.o: conv.cu
	$(NVCC) -c -o $@ conv.cu $(NVCC_FLAGS)
	gcc -c bmp.c

$(EXE): $(OBJ)
	$(NVCC) $(OBJ) bmp.o -o $(EXE) $(LD_FLAGS)

clean:
	rm -rf *.o $(EXE)
	rm deviceQuery.*
