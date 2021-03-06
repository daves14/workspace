#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

char* programSource;

void    readfile()
{       FILE *file;

        file=fopen("./Kernel/reduction.cl","rb");
        fseek(file,0, SEEK_END);
        int len = ftell(file);
        fseek(file, 0, SEEK_SET);
        programSource=(char *)malloc(len+1);
        fread(programSource,len,1,file);
        fclose(file);
}

int main()
{
        const int elements=78254135;
        size_t    datasize=sizeof(int)*elements;
        size_t    retSize;
	int 	  nG;

        cl_int  status;
        cl_uint numDevices=0;
        cl_context context;
        cl_device_id *devices;
        cl_platform_id *platforms=NULL;
        cl_uint numPlatforms=0;
        cl_program program;
        cl_kernel kernel;
        cl_command_queue cmdQueue;
        size_t globalWorkSize[1];
        size_t localWorkSize[1];

        cl_mem bufA;
        cl_mem bufB;

        int *A=NULL;
        int *B=NULL;
        int *C=NULL;

        int i;

        status=clGetPlatformIDs(0,NULL,&numPlatforms);
        platforms=(cl_platform_id*) malloc (numPlatforms*sizeof(cl_platform_id));
        status=clGetPlatformIDs(numPlatforms,platforms,NULL);
        status=clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU,0,NULL,&numDevices);
        devices=(cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
        status=clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU,numDevices,devices,NULL);
        context=clCreateContext(NULL,numDevices,devices,NULL,NULL,&status);
        cmdQueue=clCreateCommandQueue(context,devices[0],0,&status);
        clGetDeviceInfo(devices[0],CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t),(void*)localWorkSize,NULL);

        nG=(elements>>1)/localWorkSize[0];
        if(nG*2*localWorkSize[0]<elements)nG++;
        globalWorkSize[0]=nG*localWorkSize[0];
	retSize=nG*sizeof(int);

        bufA=clCreateBuffer(context,CL_MEM_READ_ONLY,datasize,NULL,&status);
        bufB=clCreateBuffer(context,CL_MEM_WRITE_ONLY,retSize,NULL,&status);

        A=(int *)malloc(datasize);
        B=(int *)malloc(retSize);
        C=(int *)malloc(retSize);
        for(i=0;i<nG;i++)C[i]=0;
        for(i=0;i<elements;i++)A[i]=-1000+2000*(float)rand()/RAND_MAX;
        for(i=0;i<elements;i++)C[i/(localWorkSize[0]<<1)]+=A[i];

        status=clEnqueueWriteBuffer(cmdQueue,bufA,CL_FALSE,0,datasize,A,0,NULL,NULL);
        readfile();
        program=clCreateProgramWithSource(context,1,(const char**)&programSource,NULL,&status);
        status=clBuildProgram(program,numDevices,devices,NULL,NULL,NULL);
        kernel=clCreateKernel(program,"reduction",&status);

        status=clSetKernelArg(kernel,0,sizeof(cl_int),(void *)&elements);
        status=clSetKernelArg(kernel,1,sizeof(cl_mem),&bufA);
        status=clSetKernelArg(kernel,2,sizeof(cl_mem),&bufB);
        status=clSetKernelArg(kernel,3,localWorkSize[0]*sizeof(cl_int),0);

        status=clEnqueueNDRangeKernel(cmdQueue,kernel,1,NULL,globalWorkSize,localWorkSize,0,NULL,NULL);

        clEnqueueReadBuffer(cmdQueue,bufB,CL_TRUE,0,retSize,B,0,NULL,NULL);

        for(i=0;i<nG;i++)printf("C[%i]=%i B[%i]=%i \n",i,C[i],i,B[i]);

        clReleaseMemObject(bufA);
        clReleaseMemObject(bufB);

        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseCommandQueue(cmdQueue);

        free(devices);
        free(platforms);
        free(A);
        free(B);
        return 0;
}
