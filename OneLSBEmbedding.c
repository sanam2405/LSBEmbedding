/*

Name : Manas Pratim Biswas

Team 1
1. Nidhi Shah               002011001053(A1)
2. Dibyabrata Panja         002011001028(A1)
3. Manas Pratim Biswas      002011001025(A1)
4. Md Bosirullah Mondal     002011001030(A1)



In this assignment, we have embeded secret information using LSB 
substitution. We have performed 1-LSB substitution and 2-LSB
substitution with OPAP. Then we have calculated the MSE and PSNR
for the above processes and ultimately written resultant the 2-D 
array in a PGM file  

*/


// Header files
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

// Global Variable Declaration
const int N = 256;

int originalArray[256][256];            // stores the original 2-D Matrix
int oneLSB_SecretArray[256][256];       // stores the 1-LSB Secret Array
int oneLSB_EmbeddedArray[256][256];     // stores the 1-LSB Embedded Array
int i,j;                                // used to run loops
double oneLSB_MSE;                      // stores the 1-LSB MSE
double oneLSB_PSNR;                     // stores the 1-LSB PSNR

int oneLSBSubstitution(int n, int bit)
{
    if(n&1)
    {
        if(bit==1)
        return n;
        else
        return n-1;
    }
    else
    {
        if(bit==1)
        return n+1;
        else
        return n;
    }
}

void readOriginalArray()
{
    FILE *ptr;
    ptr = NULL;
    ptr = fopen("256x256_cover1.txt","r");

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        fscanf(ptr,"%d",&originalArray[i][j]);
    }
    
    fclose(ptr);
}

void readOneLSBSecretInfo()
{
    
    FILE *ptr;
    ptr = NULL;
    ptr = fopen("Secret_info1.txt","r");

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        fscanf(ptr,"%d",&oneLSB_SecretArray[i][j]);    
    }
    
    fclose(ptr);
}

void embedOneLSB()
{
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            oneLSB_EmbeddedArray[i][j] = oneLSBSubstitution(originalArray[i][j],oneLSB_SecretArray[i][j]);
        }
    }

}

void oneLSB_Calculations()
{
        int squaredError = 0;
        
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                int difference = originalArray[i][j]-oneLSB_EmbeddedArray[i][j];
                squaredError += difference*difference;
            }
        }

        oneLSB_MSE = (1.0*squaredError)/(N*N);
        oneLSB_PSNR = 10.0*log10(1.0*(N-1)*(N-1)/oneLSB_MSE);

        printf("The MSE for 1-LSB Substition is : %.2f\n",oneLSB_MSE);
        printf("The PSNR for 1-LSB Substition is : %.2f\n",oneLSB_PSNR);
}

void oneLSB_EmbeddedPGMFile()
{
    FILE *pgm;
    pgm = NULL;
    int w = 256, h = 256;
    pgm = fopen("pgmImg1.pgm","wb");
    fprintf(pgm,"P2\n");
    fprintf(pgm,"%d %d\n",w,h);
    fprintf(pgm,"255\n");
    
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            fprintf(pgm,"%d ",oneLSB_EmbeddedArray[i][j]);
        }
        fprintf(pgm,"\n");
    }

    fclose(pgm);
}

int main()
{
    readOriginalArray();   
    readOneLSBSecretInfo();
    embedOneLSB();
    oneLSB_Calculations();
    oneLSB_EmbeddedPGMFile(); 
}
