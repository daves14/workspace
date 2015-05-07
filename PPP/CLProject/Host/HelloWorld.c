#include <CL/cl.h>
#include <CL/cl_platform.h>
#include <math.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// OpenCL kernel. Each work item takes care of one element of c
const char *kernelSource =                                       "\n" \
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable                    \n" \
"__kernel void vecAdd(  __global double *a,                       \n" \
"                       __global double *b,                       \n" \
"                       __global double *c,                       \n" \
"                       const unsigned int n)                    \n" \
"{                                                               \n" \
"    //Get our global thread ID                                  \n" \
"    int id = get_global_id(0);                                  \n" \
"                                                                \n" \
"    //Make sure we do not go out of bounds                      \n" \
"    if (id < n)                                                 \n" \
"        c[id] = a[id] + b[id];                                  \n" \
"                                                                \n" \
"}                                                               \n" \
"\n" ;

//Length of vectors
unsigned int n = 100000;

float *h_a;
float *h_b;
float *h_c;
float *h_a2;
float *h_b2;
float *h_c2;

cl_mem d_a;
cl_mem d_b;
cl_mem d_c;
cl_mem d_a2;
cl_mem d_b2;
cl_mem d_c2;

cl_platform_id cpPlatform;        // OpenCL platform
cl_device_id device_id;           // device ID
cl_context context;               // context
cl_command_queue queue;           // command queue
cl_program program;               // program
cl_kernel kernel;                 // kernel

cl_event eventLoadReady[2];
cl_event eventProcessReady[2];

pthread_t writeDataThread;
pthread_t processDataThread;


const char *ksrc() {
    char * buffer;
    long lSize;
    FILE * fp;

    fp = fopen ("reduction.cl", "rb" );
    if( !fp ) perror("reduction.cl"),exit(1);

    //get filesize
    fseek(fp , 0L , SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = (char *)calloc(1, lSize+1);
    if(!buffer) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    fclose(fp);
    return buffer;;
}


void *loadData()
{
    // Size, in bytes, of each vector
    size_t bytes = n*sizeof(float);
    cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, 0);

    // Write our data set into the input array in device memory
    clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0, bytes, h_a, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0, bytes, h_b, 0, NULL, &eventLoadReady[0]);

    // Write our data set into the input array in device memory
    clEnqueueWriteBuffer(queue, d_a2, CL_TRUE, 0, bytes, h_a, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_b2, CL_TRUE, 0, bytes, h_b, 0, NULL, &eventLoadReady[1]);

    return NULL;
}



void *processData()
{
    size_t bytes = n*sizeof(float);
    cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, 0);

    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0, bytes, h_c, 1, &eventProcessReady[0], NULL );

    clEnqueueReadBuffer(queue, d_c2, CL_TRUE, 0, bytes, h_c2, 1, &eventProcessReady[1], NULL );

    return NULL;
}



int main() {
    // Size, in bytes, of each vector
    size_t bytes = n*sizeof(float);

    // Allocate memory for each vector on host
    h_a = (float*)malloc(bytes);
    h_b = (float*)malloc(bytes);
    h_c = (float*)malloc(bytes);
    h_a2 = (float*)malloc(bytes);
    h_b2 = (float*)malloc(bytes);
    h_c2 = (float*)malloc(bytes);

    // Initialize vectors on host
    int i;
    for( i = 0; i < n; i++ )
    {
        h_a[i] = sinf(i)*sinf(i);
        h_b[i] = cosf(i)*cosf(i);
        h_a2[i] = h_a[i];
        h_b2[i] = h_b[i];
    }

    size_t globalSize, localSize;
    cl_int err;

    // Number of work items in each local work group
    localSize = 64;

    // Number of total work items - localSize must be devisor
    globalSize = ceil(n/(float)localSize)*localSize;



    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);

    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

    // Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    // Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);


    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);

    // Build the program executable
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "vecAdd", &err);

    // Create the input and output arrays in device memory for our calculation
    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);
    d_a2 = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_b2 = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_c2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);

    pthread_create (&writeDataThread, NULL, loadData, NULL);
    pthread_join (writeDataThread, NULL);

    // Set the arguments to our compute kernel
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);
    // Execute the kernel over the entire range of the data set
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 1,  &eventLoadReady[0], &eventProcessReady[0]);

    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a2);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b2);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c2);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 1,  &eventLoadReady[1], &eventProcessReady[1]);

    pthread_create(&processDataThread, NULL, processData, NULL);
    pthread_join(processDataThread, NULL);

    //Sum up vector c and print result divided by n, this should equal 1 within error
    double sum = 0;
    double sum2 = 0;
    for(i=0; i<n; i++) {
        sum += h_c[i];
        sum2 += h_c2[i];
    }
    printf("final results:\n\t sum: %f \n\t sum2: %f \n", sum/n, sum2/n);

    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);

    // release OpenCL resources
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_c);
    clReleaseMemObject(d_a2);
    clReleaseMemObject(d_b2);
    clReleaseMemObject(d_c2);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //release host memory
    free(h_a);
    free(h_b);
    free(h_c);
    free(h_a2);
    free(h_b2);
    free(h_c2);

    return EXIT_SUCCESS;
}