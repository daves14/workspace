__kernel
void reduction(unsigned int N,__global int *A,__global int *B,__local int *s)
{
	int idx=get_global_id(0)<<1;
	int idl=get_local_id(0);
	int isz=get_local_size(0)>>1;
	int a= (idx  <N) ? A[idx]   : 0;
	int b= (idx+1<N) ? A[idx+1] : 0;

	s[idl]=a+b;
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&1))s[idl]+=s[idl+1];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&3))s[idl]+=s[idl+2];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&7))s[idl]+=s[idl+4];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&15))s[idl]+=s[idl+8];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&31))s[idl]+=s[idl+16];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&63))s[idl]+=s[idl+32];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&127))s[idl]+=s[idl+64];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&255))s[idl]+=s[idl+128];
	barrier(CLK_LOCAL_MEM_FENCE);
        if(0==(idl&511))s[idl]+=s[idl+256];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(0==(idl&1023))s[idl]+=s[idl+512];
	barrier(CLK_LOCAL_MEM_FENCE);
	if(idl==0)B[get_group_id(0)]=s[idl];
};
