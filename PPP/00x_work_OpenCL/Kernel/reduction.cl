__kernel
void reduction(unsigned int elements,__global int * A, __global int * B, __local int * C)
{
        int idx=get_global_id(0)*2;
	int A_Momentan;
	int B_Momentan;
	if( idx > elements)
	{
		A_Momentan = 0;
	}
	else
		A_Momentan = A[idx];	
	if( idx +1 > elements )
		B_Momentan = 0;
	else
		B_Momentan = A[idx+1];
	
	int idl=get_local_id(0);
	int summe = A_Momentan + B_Momentan;
	C[idl] = summe;
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 2 == 0)
	{
		C[idl] += C[idl +1];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 4 == 0)
	{
		C[idl] += C[idl +2];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 8 == 0)
	{
		C[idl] += C[idl +4];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 16 == 0)
	{
		C[idl] += C[idl +8];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 32 == 0)
	{
		C[idl] += C[idl +16];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 64 == 0)
	{
		C[idl] += C[idl +32];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 128 == 0)
	{
		C[idl] += C[idl +64];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 256 == 0)
	{
		C[idl] += C[idl +128];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl % 512 == 0)
	{
		C[idl] += C[idl +256];
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl == 0)
	B[get_group_id(0)]= C[idl];
};
