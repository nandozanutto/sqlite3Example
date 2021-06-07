#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>
#include <stdint.h> 
#include <float.h> 
#include <math.h>
#include <string.h>


typedef union
{     
    int32_t i;
    float f;  
    struct 
    {   // Bitfields for exploration. Do not use in production code. 
        uint32_t mantissa : 23; 
        uint32_t exponent : 8; 
        uint32_t sign : 1; 
    } parts; 
} Float_t; 

/* imprime a union Float_t como ponto flutuante, hexadecimal e 
 * suas partes na forma de inteiros */


typedef struct 
{
    int index;
    int isUnitary;
    Float_t a;//limite inferior
    Float_t b;//limite superior
} Var;



float getMinMax(float a, float b, float c, float d, int minWasAsked){
    float num[4], max, min;
    num[0] = a;
    num[1] = b;
    num[2] = c;
    num[3] = d;
    max =num[0]; 
    min = num[0];
    for(int i=1; i<4; i++){
        if(isgreater(num[i], max))//comparação segura da lib math.h
            max = num[i];
        else if(isgreater(min, num[i]))
            min = num[i];
    }
    if(minWasAsked)
        return min;
    else
        return max;

}

Float_t next(Float_t num, int isInferior){
    if((num.parts.exponent == 0) && (num.parts.mantissa == 0) && (isInferior)){
        num.i = 0x80000001;//caso em que a mantissa entra em uma exceção
        return num;
    }
    if((num.parts.exponent == 0) && (num.parts.mantissa == 0) && (!isInferior)){
        num.i = 0x00000001;//caso em que a mantissa entra em uma exceção
        return num;
    }
    
    if(isInferior)
        num.parts.sign ? num.i++ : num.i--;
    else
        num.parts.sign ? num.i-- : num.i++;
    
    return num;

}

int isUnitary(Float_t A, Float_t B, int maxULPs)
{
    if(A.i == 0x80000001 && B.i == 0x00000001) return 1;
    // Different signs means they do not match.
    if (A.parts.sign != B.parts.sign)
    {
        // Check for equality to make sure +0==-0
        if (A.f == B.f)
            return 1;
        return 0;
    }
 
    // Find the difference in ULPs.
    int ulpsDiff = abs(A.i - B.i)-1;
    
    if (ulpsDiff < maxULPs)
        return 1;
    return 0;
}

void printFloat_t( Var num)
{
   printf("X%d = [%20.8e, %20.8e]\n",
            num.index,num.a.f, num.b.f); 
} 
int main() 
{
// #pragma STDC FENV_ACCESS ON 
    // fesetround(FE_DOWNWARD);//SETANDO PARA ARREDONDAR PARA ""BAIXO""
    
    // printf("\nEpsilon: %1.15f\n", FLT_EPSILON); 

    Var variables[100];
    int m, n;
    Float_t num;
    char op;
    int var1, var2, var3;
    scanf("%d %d", &m, &n);
    while ((getchar()) != '\n');//limpando buffer
    for(int i=0; i<m; i++){
        scanf("x%*d %f", &num.f); //pulando caracteres desnecessários
        variables[i].index = i+1;
        variables[i].b.f = num.f;
        variables[i].a.f = num.f;
        variables[i].a = next(variables[i].a, 1);
        variables[i].b = next(variables[i].b, 0);
        while ((getchar()) != '\n');//limpando buffer

    }
    for(int i=0; i<n; i++){
        scanf("x%d = x%d %c x%d", &var1, &var2, &op, &var3);
        while ((getchar()) != '\n');//limpando buffer
        if(op == '+'){
            variables[var1-1].index = i+1+m;
            variables[var1-1].a.f = variables[var2-1].a.f + variables[var3-1].a.f;
            variables[var1-1].a = next(variables[var1-1].a, 1);
            variables[var1-1].b.f = variables[var2-1].b.f + variables[var3-1].b.f;
            variables[var1-1].b = next(variables[var1-1].b, 0);
            
            
        }
        else if(op == '-'){
            variables[var1-1].index = i+1+m;
            variables[var1-1].a.f = variables[var2-1].a.f - variables[var3-1].b.f;
            variables[var1-1].a = next(variables[var1-1].a, 1);
            variables[var1-1].b.f = variables[var2-1].b.f - variables[var3-1].a.f;
            variables[var1-1].b = next(variables[var1-1].b, 0);
        }
        else if(op == '*'){
            variables[var1-1].index = i+1+m;
            Float_t aXc, aXd, bXc, bXd;
            aXc.f = variables[var2-1].a.f*variables[var3-1].a.f;
            aXd.f = variables[var2-1].a.f*variables[var3-1].b.f;
            bXc.f = variables[var2-1].b.f*variables[var3-1].a.f;
            bXd.f = variables[var2-1].b.f*variables[var3-1].b.f;
            
            aXc = next(aXc, 1);
            aXd = next(aXd, 1);
            bXc = next(bXc, 1);
            bXd = next(bXd, 1);

            float min = getMinMax(aXc.f, aXd.f, bXc.f, bXd.f, 1);
            variables[var1-1].a.f = min;
            //*****************************
            aXc.f = variables[var2-1].a.f*variables[var3-1].a.f;
            aXd.f = variables[var2-1].a.f*variables[var3-1].b.f;
            bXc.f = variables[var2-1].b.f*variables[var3-1].a.f;
            bXd.f = variables[var2-1].b.f*variables[var3-1].b.f;

            
            aXc = next(aXc, 0);
            aXd = next(aXd, 0);
            bXc = next(bXc, 0);
            bXd = next(bXd, 0);

            float max = getMinMax(aXc.f, aXd.f, bXc.f, bXd.f, 0);
            variables[var1-1].b.f = max;

        }
        else if(op == '/'){
            variables[var1-1].index = i+1+m;
            if(isgreaterequal(0.0, variables[var3-1].a.f) && isgreaterequal(variables[var3-1].b.f, 0.0)){
                //caso em que 0 pertence ao intervalo de y
                variables[var1-1].a.f = -INFINITY;
                variables[var1-1].b.f = INFINITY;
                continue;
            }

            
            Float_t aX1d, aX1c, bX1d, bX1c;
            aX1d.f = variables[var2-1].a.f*(1.0/variables[var3-1].b.f);
            aX1c.f = variables[var2-1].a.f*(1.0/variables[var3-1].a.f);
            bX1d.f = variables[var2-1].b.f*(1.0/variables[var3-1].b.f);
            bX1c.f = variables[var2-1].b.f*(1.0/variables[var3-1].a.f);

            aX1d = next(aX1d, 1);
            aX1c = next(aX1c, 1);
            bX1d = next(bX1d, 1);
            bX1c = next(bX1c, 1);

            float min = getMinMax(aX1d.f, aX1c.f, bX1d.f, bX1c.f, 1);
            variables[var1-1].a.f = min;
            //***************************
            aX1d.f = variables[var2-1].a.f*(1.0/variables[var3-1].b.f);
            aX1c.f = variables[var2-1].a.f*(1.0/variables[var3-1].a.f);
            bX1d.f = variables[var2-1].b.f*(1.0/variables[var3-1].b.f);
            bX1c.f = variables[var2-1].b.f*(1.0/variables[var3-1].a.f);

            aX1d = next(aX1d, 0);
            aX1c = next(aX1c, 0);
            bX1d = next(bX1d, 0);
            bX1c = next(bX1c, 0);

            float max = getMinMax(aX1d.f, aX1c.f, bX1d.f, bX1c.f, 0);
            variables[var1-1].b.f = max;



        }

    }
    
    
    
    for(int i=0; i<m+n; i++){
        printFloat_t(variables[i]);
        variables[i].isUnitary = isUnitary(variables[i].a, variables[i].b, 2);
    }
    printf("\nNão unitários:\n");
    for(int i=0; i<m+n; i++){
        if(!variables[i].isUnitary)
            printFloat_t(variables[i]);
    }

}

