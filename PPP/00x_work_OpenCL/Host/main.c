#include "stdlib.h"
#include "stdio.h"
#include "CL/cl.h"

#define TIME_MEASUREMENT

char *stream;

void run (int numDevices, cl_device_id *deviceIDs, cl_platform_id* platformIds);


// OpenCL Fehlerausgabe
void openClFehler (cl_int err)
{

	switch (err)
	{
	case CL_DEVICE_NOT_FOUND:
		fprintf(stderr,"CL_DEVICE_NOT_FOUND\n");
	break;
	case CL_DEVICE_NOT_AVAILABLE:
		fprintf(stderr,"CL_DEVICE_NOT_FOUND\n");
	break;
        case CL_COMPILER_NOT_AVAILABLE:
                fprintf(stderr,"CL_COMPILER_NOT_AVAILABLE\n");
        break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                fprintf(stderr,"CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
        break;
        case CL_OUT_OF_RESOURCES :
                fprintf(stderr,"CL_OUT_OF_RESOURCES\n");
        break;
        case CL_OUT_OF_HOST_MEMORY :
                fprintf(stderr,"CL_OUT_OF_HOST_MEMORY\n");
        break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:
                fprintf(stderr,"CL_PROFILING_INFO_NOT_AVAILABLE\n");
        break;
        case CL_MEM_COPY_OVERLAP :
                fprintf(stderr,"CL_MEM_COPY_OVERLAP\n");
        break;
        case CL_IMAGE_FORMAT_MISMATCH:
                fprintf(stderr,"CL_IMAGE_FORMAT_MISMATCH\n");
        break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
                fprintf(stderr,"CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
        break;
        case CL_BUILD_PROGRAM_FAILURE:
                fprintf(stderr,"CL_BUILD_PROGRAM_FAILURE\n");
        break;
        case CL_MAP_FAILURE :
                fprintf(stderr,"CL_MAP_FAILURE\n ");
        break;
        case CL_INVALID_VALUE :
                fprintf(stderr,"CL_INVALID_VALUE\n");
        break;
        case CL_INVALID_DEVICE_TYPE:
                fprintf(stderr,"CL_INVALID_DEVICE_TYPE\n");
        break;
        case CL_INVALID_PLATFORM :
                fprintf(stderr,"CL_INVALID_PLATFORM\n");
        break;
        case CL_INVALID_DEVICE:
                fprintf(stderr,"CL_INVALID_DEVICE\n");
        break;
        case CL_INVALID_CONTEXT:
                fprintf(stderr,"CL_INVALID_CONTEXT\n");
        break;
        case CL_INVALID_QUEUE_PROPERTIES:
                fprintf(stderr,"CL_INVALID_QUEUE_PROPERTIES\n");
        break;
        case CL_INVALID_COMMAND_QUEUE:
                fprintf(stderr,"CL_INVALID_COMMAND_QUEUE\n");
        break;
        case CL_INVALID_HOST_PTR :
                fprintf(stderr,"CL_INVALID_HOST_PTR\n");
        break;
        case CL_INVALID_MEM_OBJECT:
                fprintf(stderr,"CL_INVALID_MEM_OBJECT\n");
        break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                fprintf(stderr,"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
        break;
        case CL_INVALID_IMAGE_SIZE:
                fprintf(stderr,"CL_INVALID_IMAGE_SIZE\n");
        break;
        case CL_INVALID_SAMPLER :
                fprintf(stderr,"CL_INVALID_SAMPLER\n");
        break;
        case CL_INVALID_BINARY:
                fprintf(stderr,"CL_INVALID_BINARY\n");
        break;
        case CL_INVALID_BUILD_OPTIONS:
                fprintf(stderr,"CL_INVALID_BUILD_OPTIONS\n");
        break;
        case CL_INVALID_PROGRAM :
                fprintf(stderr,"CL_INVALID_PROGRAM\n");
        break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
                fprintf(stderr,"CL_INVALID_PROGRAM_EXECUTABLE\n");
        break;
        case CL_INVALID_KERNEL_NAME:
                fprintf(stderr,"CL_INVALID_KERNEL_NAME\n");
        break;
        case CL_INVALID_KERNEL_DEFINITION:
                fprintf(stderr,"CL_INVALID_KERNEL_DEFINITION\n");
        break;
        case CL_INVALID_KERNEL:
                fprintf(stderr,"CL_INVALID_KERNEL\n");
        break;
        case CL_INVALID_ARG_INDEX :
                fprintf(stderr,"CL_INVALID_ARG_INDEX\n");
        break;
        case CL_INVALID_ARG_SIZE:
                fprintf(stderr,"CL_INVALID_ARG_SIZE\n");
        break;
        case CL_INVALID_KERNEL_ARGS:
                fprintf(stderr,"CL_INVALID_KERNEL_ARGS\n");
        break;
        case CL_INVALID_WORK_DIMENSION:
                fprintf(stderr,"CL_INVALID_WORK_DIMENSION\n");
        break;
        case CL_INVALID_WORK_GROUP_SIZE:
                fprintf(stderr,"CL_INVALID_WORK_GROUP_SIZE\n");
        break;
        case CL_INVALID_WORK_ITEM_SIZE:
                fprintf(stderr,"CL_INVALID_WORK_ITEM_SIZE\n");
        break;
        case CL_INVALID_GLOBAL_OFFSET:
                fprintf(stderr,"CL_INVALID_GLOBAL_OFFSET\n");
        break;
        case CL_INVALID_EVENT_WAIT_LIST:
                fprintf(stderr,"CL_INVALID_EVENT_WAIT_LIST\n");
        break;
        case CL_INVALID_EVENT:
                fprintf(stderr,"CL_INVALID_EVENT\n");
        break;
        case CL_INVALID_OPERATION:
                fprintf(stderr,"CL_INVALID_OPERATION\n");
        break;
        case CL_INVALID_GL_OBJECT:
                fprintf(stderr,"CL_INVALID_GL_OBJECT\n");
        break;
        case CL_INVALID_BUFFER_SIZE :
                fprintf(stderr,"CL_INVALID_BUFFER_SIZE\n");
        break;
        case CL_INVALID_MIP_LEVEL:
                fprintf(stderr,"CL_INVALID_MIP_LEVEL\n");
        break;
        case CL_INVALID_GLOBAL_WORK_SIZE:
                fprintf(stderr,"CL_INVALID_GLOBAL_WORK_SIZE\n");
        break;
	default:
		fprintf(stderr,"Unbekannter Fehler\n");
	break;
	}
}

/*
	Infos �ber die Patter suchen.
	Zuerst die Gr��e der Patter holen
	Dann den Inhalt der Patter holen
*/
void DisplayPlatformInfo( cl_platform_id id, cl_platform_info name, char* str)
{
	cl_int err;
	size_t paramValueSize;
	char * info;

	// Gr��e des Pattern holen
	err = clGetPlatformInfo( id, name, 0, NULL, &paramValueSize);
	if ( err != CL_SUCCESS)
	{
		fprintf(stderr, "Failed to find OpenCL platform %s\n", str);
		return;
	}

	// Speicher in Gr��e des Pattern holen
	info = (char *)malloc( sizeof( char) * paramValueSize);

	// Inhalt der Gr��e des Pattern holen
	err = clGetPlatformInfo( id, name, paramValueSize, info, NULL);
	if ( err != CL_SUCCESS)
	{
		openClFehler( err);
		fprintf( stderr,"Failed to find OpenCL platform %s\n",str);
		return;
	}
	printf( "%s\t%s\n", str, info);
	free( info);
}

void DisplayDeviceInfo(cl_device_id id, cl_device_info name, char* str)
{
        cl_int err;
        size_t paramValueSize;
        cl_uint info;

//	err = clGetDeviceInfo( id, name, 0, NULL, &paramValueSize);
//	openClFehler(err);
        // Gr��e des Pattern holen
        err = clGetDeviceInfo(id,name,0,NULL,&paramValueSize);
        if (err != CL_SUCCESS)
        {
		openClFehler(err);
                fprintf(stderr, "Failed to find OpenCL platform %s\n", str);
                return;
        }
        // Speicher in Gr��e des Pattern holen
        // Inhalt der Gr��e des Pattern holen

 	err = clGetDeviceInfo(id,name,paramValueSize,&info,NULL);
        if (err != CL_SUCCESS)
        {
		openClFehler(err);
                fprintf(stderr,"Failed to find OpenCL platform %s\n",str);;
                return;
        }

        printf("%s\t%d\n", str, info);
}


int main(int argc, char** argv) {

	// OpenCL Deklarationen
	// ==================================================
    cl_int err;							// error code returned from api calls

    cl_uint i;
    cl_uint numPlatforms;				// Anzahl der OpenCL Plattformen
	cl_uint numDevices = 0;
    cl_platform_id * platformIds;

    cl_device_id deviceIds[2];		// compute device id

    // ==================================================


    err = clGetPlatformIDs( 0, NULL, &numPlatforms); // Anzahl der OpenCL Plattformen welche genutzt werden k�nnen in "numPlatforms"
    // Pr�fen ob mehr wie 0 Plattformen gefunden wurden
    if ( err != CL_SUCCESS || numPlatforms <= 0) {
    	openClFehler(err);
   		fprintf(stderr, "Failed to find any OpenCL platform.\n");
   		return 0;
    }

    printf("======= Plattform Infos ==========\n");
	printf("number of platforms: %d\n", numPlatforms);

	platformIds = ( cl_platform_id *) malloc( sizeof( cl_platform_id) * numPlatforms);
	err = clGetPlatformIDs( numPlatforms, platformIds, NULL);
	if ( err != CL_SUCCESS) {
		fprintf( stderr, "Failed to find any OpenCL platforms.\n");
		return 0;
	}

	for ( i = 0; i < numPlatforms; i++) {
		printf("======= Plattform - %d - ==========\n", ( i+1));
		DisplayPlatformInfo( platformIds[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");
		DisplayPlatformInfo( platformIds[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");
		DisplayPlatformInfo( platformIds[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
		DisplayPlatformInfo( platformIds[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
		DisplayPlatformInfo( platformIds[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");
	}
	printf("===================================\n\n");

	printf("======= Device Infos ==========\n");
	err = clGetDeviceIDs( platformIds[0], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	if (numDevices < 1) {
		fprintf( stderr, "keine GPU vorhanden\n");
	} else {
		deviceIds[0] = (cl_device_id) malloc(numDevices * sizeof(cl_device_id));
		printf("Anzahl der GPUs: %d\n", numDevices);
	}
	if (err != CL_SUCCESS) {
		openClFehler(err);
		fprintf( stderr, "Fail to find any OpenCL Device. \n");
		return 0;
	}
	err = clGetDeviceIDs( platformIds[0], CL_DEVICE_TYPE_GPU, 1, &deviceIds[0],	NULL);
	if (err != CL_SUCCESS) {
		openClFehler(err);
		return 0;
	}
	for (i = 0; i < numDevices; i++) {
		DisplayDeviceInfo( deviceIds[0], CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID");
		DisplayDeviceInfo( deviceIds[0], CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
		DisplayDeviceInfo( deviceIds[0], CL_DEVICE_TYPE, "CL_DEVICE_TYPE");
		DisplayDeviceInfo( deviceIds[0], CL_DEVICE_IMAGE_SUPPORT, "CL_DEVICE_IMAGE_SUPORT");
		DisplayDeviceInfo( deviceIds[0], CL_DEVICE_ADDRESS_BITS, "CL_DEVICE_ADDRESS_BITS");
	}
	char ret_string[1014];
	clGetDeviceInfo( deviceIds[0], CL_DEVICE_NAME, sizeof(ret_string), &ret_string, NULL);
	printf("CL_DEVICE_NAME: \t\t\t%s\n", ret_string);

	run(numDevices, deviceIds, platformIds);
	free(platformIds);

	return 0; // main() end
}


void readOpenCLFile ()
{
	/*
	FILE *file;
	unsigned int size;

	file = fopen( "./Kernel/reduction.cl","rb");
	fseek( file, 0, SEEK_END);
	size = ftell( file);
	stream = calloc( size +1, 1);
	fseek( file, 0, SEEK_SET);
	fread( stream, 1, size, file);
	printf("\n\n======================================================================\n");
	printf("START openCL Programm:\n%s\n",stream);
	printf("\n==ENDE OpenCL Programm================================================\n");
	*/

	FILE *file;
	unsigned int size;

	file = fopen( "./Kernel/reduction.cl","rb");
	fseek( file, 0, SEEK_END);
	size = ftell( file);
	fseek( file, 0, SEEK_SET);
	stream = (char *) malloc( size +1);
	fread( stream, 1, size, file);
	fclose( file);
	printf("\n\n======================================================================\n");
	printf("START openCL Programm:\n%s\n",stream);
	printf("\n==ENDE OpenCL Programm================================================\n");

}

void run ( int numDevices, cl_device_id* deviceIDs, cl_platform_id* platformIds)
{

	/* =====================================================================
	 * Variablen
	 * =====================================================================
	 */
	cl_int err = 0;							// error code returned from api calls
	cl_context context = NULL;				// compute context
	cl_command_queue commandQueue = NULL;	// compute command queue
    cl_program program = NULL;				// compute programm
    cl_kernel kernel = NULL;				// compute kernel

    cl_mem input = NULL;					// input buffer
    cl_mem output = NULL;					// output buffer

    size_t global;							// global domain size for our calculation
    size_t local;							// local domain size for our calculation

	unsigned int i;
	cl_event event;
	/* =====================================================================
	 * Variablen ENDE
	 * =====================================================================
	 */

	const unsigned int elements = 501;					// Anzahl der Elemente
	size_t datasize = sizeof( unsigned int) * elements;	// Datengr��e auf Grundlage Anzahl der Elemente


	// Create a compute context
	context = clCreateContext( NULL, numDevices, deviceIDs, NULL, NULL, &err);
	if ( context == NULL || err != CL_SUCCESS)
	{
		openClFehler( err);
		printf( "ERROR: Failed at clCreateContext()\n");
		return;
	}
	printf( "OpenCL context created\n");


	// Create OpenCL command queue
#ifdef TIME_MEASUREMENT
	// mit Zeitmessung
	commandQueue = clCreateCommandQueue( context, deviceIDs[0], CL_QUEUE_PROFILING_ENABLE, &err);
#else
	// ohne Zeitmessung
	commandQueue = clCreateCommandQueue( context, deviceIDs[0], 0, &err);
#endif

	if ( commandQueue == NULL || err != CL_SUCCESS)
	{
		openClFehler( err);
		printf( "ERROR: Failed at clCreateCommandQueue()\n()\n");
		return;
	}
	printf( "OpenCL command queue created\n");


	printf( "start reading OpenCL File\n");
	readOpenCLFile();	// read OpenCL file into *stream
	printf( "finish reading OpenCL File\n");


	// Buffer anlegen, f�r die �bergabe an die Grafikkarte
	input = clCreateBuffer( context, CL_MEM_READ_ONLY, datasize, NULL, &err);
	if( !input || err != CL_SUCCESS)
		openClFehler( err);
	output = clCreateBuffer( context, CL_MEM_WRITE_ONLY, datasize, NULL, &err);
	if( !output || err != CL_SUCCESS)
		openClFehler( err);

	// =============================


	int *A = NULL;
	int *C = NULL;

	A=( int *)malloc( datasize);
	C=( int *)malloc( datasize);

	// Arrays f�llen
	for( i=0; i < elements; i++)
	{
		A[i] = i;
		C[i] = 0;
	}

	// Write Buffer anlegen
	err = clEnqueueWriteBuffer( commandQueue, input, CL_FALSE, 0, datasize, A, 0, NULL, NULL);
	if( err != CL_SUCCESS )
	{
		printf( "ERROR: at clenqueueWriteBuffer()\n");
		openClFehler( err);
	}

	// Programm Source einlesen
	program = clCreateProgramWithSource( context, 1, ( const char**)&stream, NULL, &err);
	if( err != CL_SUCCESS || program == NULL)
	{
		printf( "ERROR bei CreateProgramWithSource()\n");
		openClFehler( err);
		return;
	}

	// Eingelesenes Programm builden
	err = clBuildProgram( program, numDevices, deviceIDs, NULL, NULL, NULL);
	if( err != CL_SUCCESS)
	{
		printf( "ERROR bei BuildProgramm()\n");
		openClFehler( err);
		return;
	}

	// Kernel bauen, mit der Funktion reduction
    kernel = clCreateKernel( program, "reduction", &err);

	//Kernel Argumenete Setzten
    err = clSetKernelArg( kernel, 0, sizeof( cl_int), ( void *)&elements);
	if( err != CL_SUCCESS)
		openClFehler( err);
    err = clSetKernelArg( kernel, 1, sizeof( cl_mem), &input);
	if( err != CL_SUCCESS)
		openClFehler( err);
    err = clSetKernelArg( kernel, 2, sizeof( cl_mem), &output);
	if( err != CL_SUCCESS)
		openClFehler( err);
    err = clSetKernelArg( kernel, 3, elements * sizeof( cl_int), 0);
	if( err != CL_SUCCESS)
		openClFehler( err);

    global=elements;

#ifdef TIME_MEASUREMENT
	// Zeit zur�ck setzen
	clFinish( commandQueue);
#endif
	// Programm ausf�hren
#ifdef TIME_MEASUREMENT
    err = clEnqueueNDRangeKernel( commandQueue, kernel, 1, NULL, &global, NULL, 0, NULL, &event);
	clWaitForEvents( 1 , &event);
#else
    err = clEnqueueNDRangeKernel( commandQueue, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
#endif

	// Ergebniss Buffer wieder einlesen
    //
    err = clEnqueueReadBuffer( commandQueue, output, CL_TRUE, 0, datasize, C, 0, NULL, NULL);
    if ( err != CL_SUCCESS)
    {
    	openClFehler( err);
        printf( "ERROR: Failed to read output array! clEnqueueReadBuffer()\n");
    }

    // Ergebnisausgabe
	printf("Summe A: 0 bis B:%d = %d\n",elements,C[0]);

	float f = ( ( ( elements - 1) * ( elements - 1)) + ( elements - 1)) / 2;
	printf( "Vergleich (kleiner Gau�): %f\n", f);


#ifdef TIME_MEASUREMENT
	cl_ulong time_start, time_end;
	double total_time;
	// Startzeit holen
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof( time_start), &time_start, NULL);
	// Endzeit holen
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof( time_end), &time_end, NULL);
	// Zeit differenz ausrechnen
	total_time = time_end - time_start;
	printf( "\nBen�tigte Zeit auf der GPU = %0.3f ms\n", (total_time / 1000000.0) );
#endif


	// cleanup
	//
	free( stream);
	free( A);
	free( C);

	clReleaseMemObject( input);
	clReleaseMemObject( output);
	clReleaseProgram( program);
	clReleaseKernel( kernel);
	clReleaseCommandQueue( commandQueue);
	clReleaseContext( context);

	return; // function run() end
}
