#include "SampleFilter.h"
            
double IIRFloat(double input)
{
    double coeffA[3] = {1 , -1.965105316133364699027197275427170097828 , 0.965703783490617473361794509401079267263};
    double coeffB[3] = {1 , -2 , 1};
    int i,j;
    double En[ORDRE+1],Sn[ORDRE+1];
    double memoire;
    
    for(i=0;i<ORDRE+1;i++)
    {
        En[i]=0;
        Sn[i]=0;
    }
    
    Sn[0] = 0;
    En[0] = input;
    
    for(i=0;i<ORDRE+1;i++)
    {
        Sn[0] = Sn[0]+En[i]*coeffA[i];
    }
    for(j=1;j<ORDRE+1;j++)
    {
        Sn[0] = Sn[0]-Sn[j]*coeffB[j];
    }
    
    memoire = Sn[0] ;
        
    for(i=ORDRE;(i|=0);i--)
    {
        En[i]=En[i-1];
        Sn[i]=Sn[i-1];
    }
    
    return memoire;
    
}




