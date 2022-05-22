%%cu
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define N 17
__device__ int recursive(int nDames,int* col,int*diagP,int*diagN) 
{    
    
    int solution=0;
   if(nDames==N)
    {  
       
        return 1;}

    for(int i=0; i<N; ++i)
    {
    for(int j=0; j<nDames; ++j)
    if(col[j]==i || diagP[j]==nDames+1+i || diagN[j]==nDames+1-i) 
    goto next;
    col[nDames]=i;
    diagP[nDames]=nDames+1+i;
    diagN[nDames]=nDames+1-i;
    solution+=recursive(nDames+1,col,diagP,diagN); 
    next: continue;
    }
   
    return solution;
}



 __global__ void solution1(int *solution_ge)
 {   
     __shared__ int solution;
      int col[N];
      int diagP[N];
      int diagN[N];   
      int sol=0;
      col[0]=blockIdx.x/N;
      diagP[0]=1+blockIdx.x/N;
      diagN[0]=1-blockIdx.x/N;  
      int   indice3,indice4,indice5,indice6,indice7,indice8;
      int indice2=blockIdx.x%N;
      if ( col[0]==indice2 || diagP[0]==indice2+1+1 || diagN[0]==1+1-indice2) return;
      col[1]=indice2 ;
      diagP[1]=indice2+1+1; 
      diagN[1]=1+1-indice2;            
          if(N==2) {
              sol=1;
              goto next;
          }
           indice3=blockIdx.y/N;
           for(int j=0; j<2; ++j)
           if(col[j]==indice3 || diagP[j]==2+1+indice3 || diagN[j]==2+1-indice3) return;
             col[2]=indice3;
             diagP[2]=2+1+indice3;
             diagN[2]=2+1-indice3;
           if(N==3)
           {
               sol=1; 
               goto next;
           }    
             indice4=blockIdx.y%N;
             for(int j=0; j<3; ++j)
           if(col[j]==indice4 || diagP[j]==3+1+indice4 || diagN[j]==3+1-indice4)  return;
             col[3]=indice4;
             diagP[3]=3+1+indice4;
             diagN[3]=3+1-indice4;
             if(N==4) {
                       sol=1;
                       goto next;
                       }
              indice5=blockIdx.z/N;
              for(int j=0; j<4; ++j)
           if(col[j]==indice5 || diagP[j]==4+1+indice5 || diagN[j]==4+1-indice5)   return ; 
             col[4]=indice5;
             diagP[4]=4+1+indice5;
             diagN[4]=4+1-indice5;
              if(N==5){ sol=1;
                       goto next;
                       }
               indice6=blockIdx.z%N;
              for(int j=0; j<5; ++j)
           if(col[j]==indice6 || diagP[j]==5+1+indice6 || diagN[j]==5+1-indice6)   return ; 
             col[5]=indice6;
             diagP[5]=5+1+indice6;
             diagN[5]=5+1-indice6;
              if(N==6){ sol=1;
                       goto next;
                       }
              indice7=threadIdx.x;
             for(int j=0; j<6; ++j)
           if(col[j]==indice7 || diagP[j]==6+1+indice7 || diagN[j]==6+1-indice7)   return ; 
             col[6]=indice7;
             diagP[6]=6+1+indice7;
             diagN[6]=6+1-indice7;
              if(N==7){ sol=1;
                       goto next;
                       }
              indice8=threadIdx.y;
            for(int j=0; j<7; ++j)
           if(col[j]==indice8 || diagP[j]==7+1+indice8 || diagN[j]==7+1-indice8)   return ; 
             col[7]=indice8;
             diagP[7]=7+1+indice8;
             diagN[7]=7+1-indice8;
              if(N==8){ sol=1;
                       goto next;
                       }               
            sol=recursive(8,col,diagP,diagN); 
        
        
  next : 
          
          atomicAdd(solution_ge,sol);
       
          
}




    
int main(int argc, char *argv[])
{     
       cudaEvent_t start, stop;
       int tab_solution_hos ;
       int* tab_solution_dev ;
         cudaMalloc((void**)&tab_solution_dev, sizeof(int));
       float elapsedTime;
        dim3 grid(N,N);
        dim3 bloc(N*N,N*N,N*N);
       cudaEventCreate(&start);
       cudaEventRecord(start,0);
       solution1<<<bloc,grid>>>(tab_solution_dev);
       cudaEventCreate(&stop);
       cudaEventRecord(stop,0);
       cudaEventSynchronize(stop);
       cudaMemcpy(&tab_solution_hos,tab_solution_dev,sizeof(int),cudaMemcpyDeviceToHost);
       cudaDeviceSynchronize();
       cudaFree(&tab_solution_dev);
       cudaEventElapsedTime(&elapsedTime, start,stop);
       printf("\nNombre de solutions: %d\n",tab_solution_hos);
      printf("Elapsed time : %f ms\n" ,elapsedTime);                  
      
        return 0;
}