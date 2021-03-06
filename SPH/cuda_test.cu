#include <stdio.h>      
#include <stdlib.h>     
#include <OpenGL\glew.h>      
#include <OpenGL\freeglut.h>      
#include <cuda_runtime.h>      
#include <cuda.h>      
#include <cuda_gl_interop.h>      

#define GET_PROC_ADDRESS(str) wglGetProcAddress(str)      
#define DIM 1000

PFNGLBINDBUFFERARBPROC    glBindBuffer = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = NULL;
PFNGLGENBUFFERSARBPROC    glGenBuffers = NULL;
PFNGLBUFFERDATAARBPROC    glBufferData = NULL;

GLuint bufferObj;
cudaGraphicsResource *resource;

//#define CUDA_Test

#ifdef CUDA_Test

struct cuComplex {

	float r;
	float i;

	__device__ cuComplex(float a, float b) :r(a), i(b) {}
	__device__ float magnitude2() { return r * r + i * i; }
	__device__ cuComplex operator * (const cuComplex & a) {
		return cuComplex(r * a.r - i * a.i, i * a.r + r * a.i);
	}
	__device__ cuComplex operator + (const cuComplex & a) {
		return cuComplex(r + a.r, i + a.i);
	}
};

__device__ int Julia(int x, int y) {

	const float scale = 1.5;
	float jx = scale * (float)(DIM / 2 - x) / (DIM / 2);
	float jy = scale * (float)(DIM / 2 - y) / (DIM / 2);

	cuComplex c(-0.8, 0.156);
	cuComplex a(jx, jy);

	for (int i = 0; i < 200; i++) {
		a = a * a + c;
		if (a.magnitude2() > 1000) return 0;
	}

	return 1;
}

__global__ void cudaGLKernel(uchar4 *ptr)
{
	//int x = threadIdx.x + blockIdx.x * blockDim.x;
	//int y = threadIdx.y + blockIdx.y * blockDim.y;
	//int offset = x + y * blockDim.x * gridDim.x;

	///*float fx = x / (float)DIM - 0.5f;
	//float fy = y / (float)DIM - 0.5f;*/

	//unsigned char green = 255 * sinf(x*y);
	//unsigned char red = 255 * cosf(offset*offset);
	//unsigned char blue = 255 * cosf(offset*x*y);

	//ptr[offset].x = red;
	//ptr[offset].y = green;	
	//ptr[offset].z = blue;
	//ptr[offset].w = 255;
	
	int x = blockIdx.x;
	int y = blockIdx.y;
	int offset = x + y * gridDim.x;
	int v = Julia(x, y);
	printf("%d\n", gridDim.x);
	unsigned char green = 255 * v;
	unsigned char red = 0;
	unsigned char blue = 0;
	unsigned char w = 255;
	
	ptr[offset * 4 + 0].x = green;
	ptr[offset * 4 + 1].y = red;
	ptr[offset * 4 + 2].z = blue;
	ptr[offset * 4 + 3].w = w;
}

void drawFunc(void)
{
	glDrawPixels(DIM, DIM, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glutSwapBuffers();
}

static void keyFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		cudaGraphicsUnregisterResource(resource);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		glDeleteBuffers(1, &bufferObj);
		exit(0);
	}
}

int main(int argc, char* argv[])
{
	// 定义一个设备属性对象prop    
	cudaDeviceProp prop;
	int dev;

	memset(&prop, 0, sizeof(cudaDeviceProp));

	//限定设备计算功能集的版本号    
	prop.major = 1;
	prop.minor = 0;

	//选择在计算功能集的版本号为1.0的GPU设备上运行    
	cudaChooseDevice(&dev, &prop);

	//选定GL程序运行的设备    
	cudaGLSetGLDevice(dev);

	//OpenGL环境初始化    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(DIM, DIM);
	glutCreateWindow("CUDA+OpenGL");

	glBindBuffer = (PFNGLBINDBUFFERARBPROC)GET_PROC_ADDRESS("glBindBuffer");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)GET_PROC_ADDRESS("glDeleteBuffers");
	glGenBuffers = (PFNGLGENBUFFERSARBPROC)GET_PROC_ADDRESS("glGenBuffers");
	glBufferData = (PFNGLBUFFERDATAARBPROC)GET_PROC_ADDRESS("glBufferData");

	glGenBuffers(1, &bufferObj);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, bufferObj);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, DIM*DIM * 4, NULL, GL_DYNAMIC_DRAW_ARB);

	cudaGraphicsGLRegisterBuffer(&resource, bufferObj, cudaGraphicsMapFlagsNone);

	uchar4* devPtr;
	size_t size;
	cudaGraphicsMapResources(1, &resource, NULL);
	cudaGraphicsResourceGetMappedPointer((void**)&devPtr, &size, resource);

	dim3 grids(DIM / 16, DIM / 16);
	dim3 threads(16, 16);
	cudaGLKernel << <grids, threads >> > (devPtr);

	cudaGraphicsUnmapResources(1, &resource, NULL);
	glutKeyboardFunc(keyFunc);
	glutDisplayFunc(drawFunc);
	glutMainLoop();
	return 0;
}

#endif