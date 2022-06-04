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
int twoLSB_SecretArray[256][256][2];    // stores the 2-LSB Secret Array
int twoLSB_EmbeddedArray[256][256];     // stores the 2-LSB Embedded Array
int twoLSB_OPAPArray[256][256];         // stores the 2-LSB OPAP Array
int i,j;                                // used to run loops
double twoLSB_MSE;                      // stores the 2-LSB MSE
double twoLSB_PSNR;                     // stores the 2-LSB PSNR
double twoLSB_OPAP_MSE;                 // stores the 2-LSB OPAP MSE
double twoLSB_OPAP_PSNR;                // stores the 2-:SB OPAP PSNR

int twoLSBSubstitution(int n, int twoLSB)
{
    int lastTwoBits = n%4;
    n-=lastTwoBits;
    n+=twoLSB;
    return n;
}

int findTwoLSB(int secondBit, int firstBit)
{
    if(secondBit==0&&firstBit==0)
    return 0;
    else if(secondBit==0&&firstBit==1)
    return 1;
    else if(secondBit==1&&firstBit==0)
    return 2;
    else
    return 3;
}

int calculateOPAP(int vDash, int v)
{
    int delta = vDash - v;
    int vDoubleDash;

    if(delta > 2 && delta < 4)
    {
        if(vDash >= 4)
        vDoubleDash = vDash-4;
        else
        vDoubleDash = vDash;
    }

    else if(delta >= -4 && delta <= -2)
    {
        if(vDash < 256-4)
        vDoubleDash = vDash + 4;
        else
        vDoubleDash = vDash;
    }

    else
    {
        vDoubleDash = vDash;
    }

    return vDoubleDash; 
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

void readTwoLSBSecretInfo()
{
    
    FILE *ptr;
    ptr = NULL;
    ptr = fopen("Secret_info8.txt","r");

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            fscanf(ptr,"%d",&twoLSB_SecretArray[i][j][1]);
            fscanf(ptr,"%d",&twoLSB_SecretArray[i][j][0]);
        }    
    }
    
    fclose(ptr);
}

void embedTwoLSB()
{
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            int firstBit, secondBit;
            firstBit = twoLSB_SecretArray[i][j][0];
            secondBit = twoLSB_SecretArray[i][j][1];
            int twoLSB = findTwoLSB(secondBit,firstBit);

            twoLSB_EmbeddedArray[i][j] = twoLSBSubstitution(originalArray[i][j],twoLSB);
        }
    }

}

void embedTwoLSBwithOPAP()
{
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                int vDash = twoLSB_EmbeddedArray[i][j];
                int v = originalArray[i][j];
                twoLSB_OPAPArray[i][j] = calculateOPAP(vDash,v);
            }
        }
}

void twoLSB_Calculations()
{
        int squaredError = 0;
        
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                int difference = originalArray[i][j]-twoLSB_EmbeddedArray[i][j];
                squaredError += difference*difference;
            }
        }

        twoLSB_MSE = (1.0*squaredError)/(N*N);
        twoLSB_PSNR = 10.0*log10(1.0*(N-1)*(N-1)/twoLSB_MSE);

        squaredError = 0;
        
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                int difference = originalArray[i][j]-twoLSB_OPAPArray[i][j];
                squaredError += difference*difference;
            }
        }

        twoLSB_OPAP_MSE = (1.0*squaredError)/(N*N);
        twoLSB_OPAP_PSNR = 10.0*log10(1.0*(N-1)*(N-1)/twoLSB_OPAP_MSE);

        printf("The MSE for 2-LSB Substition is : %.2f\n",twoLSB_MSE);
        printf("The PSNR for 2-LSB Substition is : %.2f\n",twoLSB_PSNR);
        printf("The MSE for 2-LSB Substition with OPAP is : %.2f\n",twoLSB_OPAP_MSE);
        printf("The PSNR for 2-LSB Substition with OPAP is : %.2f\n",twoLSB_OPAP_PSNR);
}

void twoLSB_EmbeddedPGMFile()
{
    FILE *pgm;
    pgm = NULL;
    int w = 256, h = 256;
    pgm = fopen("pgmImg2.pgm","wb");
    fprintf(pgm,"P2\n");
    fprintf(pgm,"%d %d\n",w,h);
    fprintf(pgm,"255\n");
    
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            fprintf(pgm,"%d ",twoLSB_OPAPArray[i][j]);
        }
        fprintf(pgm,"\n");
    }

    fclose(pgm);
}

int main()
{
    readOriginalArray();        
    readTwoLSBSecretInfo();
    embedTwoLSB();
    embedTwoLSBwithOPAP();
    twoLSB_Calculations();
    twoLSB_EmbeddedPGMFile();
}
