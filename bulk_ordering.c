#include <stdio.h>
#include <strings.h> 

/* This program asks about the name of the input file and the output file (*.txt). 
Then it reads the input from the input file and writes the output in the output file.
*/

/* Please enter the input with the following format in the input file:

Number of periods: n

Discount threshold: Q

Demands: d[0], ... , d[n-1]

Expensive prices(non-increasing): p1[0], ... p1[n-1]

Cheap prices(non-increasing): p2[0], ... p2[n-1]

Holding costs: h[0], ..., h[n-1]

Inventory bounds: B[0],...,B[n-1]

for example:

3
2
1 2 3 
2 2 2
1 1 1
1 1 1
2 2 2

You can continue for more instances.
*/

/* 

The outputs:

dd[n][n]:     dd[i-1][j-1]= d_{i,j} in the paper 

hh[n][n]:     hh[i-1][j-1]= h_{i,j} in the paper  

HH[n][n]:     HH[i-1][j-1]= H_{i,j} in the paper  

A[n][n]:      A[i-1][j-1] = A_{i,j} in the paper 

QQ[n][n]:     QQ[i-1][j-1]= 1 if there is a cheap order at time j for the sequence starting from time i.

	          QQ[i-1][j-1]= 0 if there is not an order at time j for the sequence starting from time i. It is also 0 when i>j.

	          QQ[i-1][j-1]= -1 if time j is out of the sequence starting from time i. 

II[n][n]:     II[i-1][j-1]= The inventory value at the beginning of time j for the sequence starting from time i. It is -1 if j>i is out of the sequence. It is 0 when i>j.

hold[n][n]:   hold[i-1][j-1]= The holding + production cost up to time j for the sequence starting from time i. It is -1 if j>i is out of the sequence. It is 0 when i>j.

Cost[n][n]:   Cost[i-1][j-1]= cost(i,j) in the paper.

last[n][n]:   last[i-1][j-1] = The value of x_j for the interval (i,j). It is -1 if cost(i,j)=inf.

cheap[n][n]:  cheap[i-1][j-1] = The value of the last cheap order before time j for the interval (i,j). It is -1 if there is not a cheap order before time j or cost(i,j)=inf.

t[n][n]:      t[i-1][j-1] + 1 = The index of the last cheap order before time j for the interval (i,j). It is -1 if there is not a cheap order before time j or cost(i,j)=inf.

OPT:          The value of optimal cost (opt(1) in the paper)

OPTx[n]:      The value of optimal orders

OPTi[n]:      The vector of opt(i)

S[n]:         S[i-1]= j-1 when (i,j) is the first regeneration interval in the solution of the problem starting from time i.
 
 */

int prepro_h(int i, int j, int* h);
int prepro_d(int i, int j, int* d);
int prepro_A(int n, int i, int j, int a, int dd[][n], int B[]);
int prepro_H(int n, int i, int j, int dd[][n], int h[]);
int c1(int n, int t, int j, int I, int hold_Q, int h[], int d[], int p1[], int p2[],  int B[], int A[][n], int dd[][n], int hh[][n], int HH[][n]);
int c2(int n, int t, int j, int I, int hold_Q, int h[], int d[], int p2[],  int B[], int A[][n], int dd[][n], int hh[][n], int HH[][n]);
int f1(int x_t, int x_j, int I, int h, int H, int p_2_t, int p_1_j);
int f2(int x_t, int x_j, int I, int h, int H, int p_2_t, int p_2_j);

int n, Q, I0=0, x, y, inf;

int main(){

    int  i, j, k, a, tt, s, cost_1, cost_2, last_cheap1, last_order1, last_cheap2, last_order2;
    FILE *fr, *fw;  
    char str[]= "inf", namer[20], namew[20]; 

    printf("please enter the file's name for the input in the format *.txt\n");
    scanf("%s" , namer);
    printf("please enter the file's name for the input in the format *.txt\n");
    scanf("%s" , namew);

    fr = fopen(namer, "r"); 
    fw = fopen(namew, "w");
 
    while(fscanf(fr, "%d ", &n)!=EOF){

        fscanf(fr, "%d",&Q);
        int d[n],p1[n], p2[n], h[n], B[n];
        int dd[n][n], hh[n][n], HH[n][n], A[n][n], hold[n][n]; 
        int QQ[n][n], II[n][n], I;
        int Cost[n][n], cheap[n][n], last[n][n], t[n][n], lastQ[2], lastI[2], cost1[2], last_cheap[2], last_order[2];
        int OPT[n+1], OPTx[n], S[n];

        for(i=0; i<n; i++){

            fscanf(fr,"%d",&d[i]);
        }

        for(i=0; i<n; i++){

            fscanf(fr,"%d",&p1[i]);
        }
        
        for(i=0; i<n; i++){

            fscanf(fr,"%d",&p2[i]);
        }
                       
        for(i=0; i<n; i++){

            fscanf(fr,"%d",&h[i]);
        }
        
        for(i=0; i<n; i++){

            fscanf(fr,"%d", &B[i]);
        }

        for (i=0; i<n; i++){

            for (j=0; j<n; j++) {

                dd[i][j]=prepro_d(i,j,d);
                hh[i][j]=prepro_h(i,j,h);
            }
        }

        for (i=0; i<n; i++){

            for (j=0; j<n; j++) {

                HH[i][j]=prepro_H(n, i,j, dd, h);
                A[i][j]=prepro_A(n,i,j, a, dd, B);
                a=A[i][j];
            }
        }

        inf= dd[0][n-1] * p1[0]+1;

        /* calculting the sequence*/

        for (i=0; i<n; i++){

            I=0;
            tt=-1;

            for (j=0; j<i; j++){   

                QQ[i][j]=0;
                II[i][j]=I;
                hold[i][j]=0;
            }    

            if((Q >= d[i]) && (Q-d[i]<= B[i])){

                QQ[i][i]=1;
                II[i][i]= I;
                hold[i][i]=0;
                s=i;

            }
            else{
                    
                if( d[i] > Q ){

                    QQ[i][i]=1;
                    II[i][i] = I;
                    hold[i][i]=0;

                    for(k=i+1; k<n; k++){

                        QQ[i][k]=-1;
                        II[i][k]=-1;
                        hold[i][k]=-1;
                    }
                    continue;  
                }
                else{

                    for(k=i; k<n; k++){

                        QQ[i][k]=-1;
                        II[i][k]=-1;
                        hold[i][k]=-1;
                    }
                    II[i][i] = I;
                    hold[i][i]=0;
                    continue;
                }
            }
                                
            k=s+1;    
            while(k<n){

                if ((Q > A[s][k]-I) || (Q+I < dd[s][k])){

                    I= Q+I-dd[s][k-1];
                    if ((Q+I >= d[k]) && (Q+I-d[k] <= B[k])){
         
                        s=k;
                        QQ[i][k]=1;
                        II[i][k]=I;
                        if (tt < s){

                            hold[i][k]= hold[i][k-1]+ h[k-1] * II[i][k] + p2[s] * Q;
                            tt=s;
                        }
                        else{
                                    
                            hold[i][k]= hold[i][k-1]+ h[k-1] * II[i][k];
                        }
                    }
                    else{

                        if( Q+I < d[k]) {

                            s=k;
                            QQ[i][k]=1;                 
                            II[i][k]=I;
                            if (tt < s){

                                hold[i][k]= hold[i][k-1]+ h[k-1] * II[i][k] + p2[s] * Q;
                                tt=s;
                            }
                            else{
                                    
                                hold[i][k]= hold[i][k-1]+ h[k-1] * II[i][k];
                            }
                            k=k+1;
                            while(k<n){

                                QQ[i][k]=-1;
                                II[i][k]=-1;
                                hold[i][k]=-1;
                                k++;
                            }
                            continue;
                        }
                        else{

                            while(k<n){

                                QQ[i][k]=-1;
                                II[i][k]=-1;
                                hold[i][k]=-1;
                                k++;                         
                            }  
                            continue;                     
                        }
                    }
                }
                else{

                    QQ[i][k]= 0;
                    II[i][k]= I + Q - dd[s][k-1];
                    if (tt < s){

                        hold[i][k]= hold[i][k-1]+ h[k-1] * II[i][k] + p2[s] * Q;
                        tt=s;
                    }
                    else{
                                    
                        hold[i][k]= hold[i][k-1]+ h[k-1] * II[i][k];
                    }
                }
                k=k+1;
            }

        }
            
/* calculating cost(i,j) */

        for (i=0; i<n; i++) {

            lastQ[0]=-1;
            lastQ[1]=-1;
            lastI[0]=-1;
            lastI[1]=-1;

         
            for (j=0; j<n; j++){

                if(j<i){
                
                    Cost[i][j]=inf;
                    cheap[i][j]=-1;
                    last[i][j]=-1;  
                    t[i][j]=-1;
                }
                else{
                    if((lastQ[0]==-1)&&(lastQ[1]==-1)){

                        if (QQ[i][j]==-1){

                            Cost[i][j]= hold[i][j] + p1[j] * (d[j]-II[i][j]);
                            cheap[i][j]=-1;
                            last[i][j]=d[j]-II[i][j];  
                            t[i][j]=-1;                    

                            for(k=j+1;k<n;k++){

                                Cost[i][k]=inf;
                                cheap[i][k]=-1;
                                last[i][k]=-1;  
                                t[i][k]=-1;
                            }
                            break;
                        }
                        else{

                            cost_1=inf;

                            if (d[j]-II[i][j] < Q){

                                if ((Q + II[i][j]-d[j] <= B[j]) && (j==n-1)){

                                    cost_1= hold[i][j] + p2[j] * Q + h[j] * (Q + II[i][j]-d[j]);
                                }

                                cost_2= hold[i][j] + p1[j] * (d[j]-II[i][j]);

                                if(cost_1< cost_2){

                                    Cost[i][j] =cost_1;   
                                    last[i][j]=Q;  
                                    cheap[i][j]=-1;
                                    t[i][j]=-1;
                                }
                                else{

                                    Cost[i][j] =cost_2;   
                                    last[i][j]= d[j]-II[i][j];                             
                                    cheap[i][j]=-1;
                                    t[i][j]=-1;                            
                                } 
                            }  
                            else{

                                Cost[i][j]= hold[i][j] + p2[j] * (d[j]-II[i][j]);
                                last[i][j]=d[j]-II[i][j];  
                                cheap[i][j]=-1;
                                t[i][j]=-1;
                            }
                
                        }
               
                    }           
                    else{
                    
                        cost1[0]=inf;
                        cost1[1]=inf;

                        for (k=0; k<2; k++){

                            if (lastQ[k] != -1){

                                cost_1= c1(n, lastQ[k] , j, lastI[k], hold[i][lastQ[k]],  h, d, p1, p2, B, A, dd, hh, HH);  
                                last_cheap1= x;
                                last_order1= y; 
                                cost_2= c2(n, lastQ[k] , j, lastI[k], hold[i][lastQ[k]],  h, d, p2, B, A, dd, hh, HH);  
                                last_cheap2= x;
                                last_order2= y; 

                                if(cost_1 < cost_2){

                                    cost1[k]= cost_1;
                                    last_cheap[k]= last_cheap1;
                                    last_order[k]= last_order1;
                                }
                                else{   

                                    cost1[k]= cost_2;
                                    last_cheap[k]= last_cheap2;
                                    last_order[k]= last_order2;    
                                }

                            }
                        }

                        if ((cost1[0] < cost1[1])){

                            Cost[i][j]= cost1[0];
                            last[i][j]= last_order[0];
                            cheap[i][j]=last_cheap[0];
                            t[i][j]=lastQ[0];
                        }
                        else{

                            if (cost1[1] != inf ){

                                Cost[i][j]= cost1[1];
                                last[i][j]= last_order[1];
                                cheap[i][j]=last_cheap[1];
                                t[i][j]=lastQ[1];
                            }

                            else{

                                Cost[i][j]= inf;
                                last[i][j]= -1;
                                cheap[i][j]= -1;
                                t[i][j]=-1;
                            }
                        }
                    }
                }          
                if (QQ[i][j]==1){

                    lastQ[0]=lastQ[1];
                    lastQ[1]=j;
                    lastI[0]=lastI[1];
                    lastI[1]=II[i][j];
                }   
            }
        }  

        /* calculating the optimal cost and the optimal orders */

        OPT[n]=0;
    

        for(i=n-1; i>=0; i=i-1){
            OPT[i]=inf;
            for(j=i; j<n; j++){
                if((Cost[i][j]+OPT[j+1]) <= OPT[i]){
                    OPT[i]= Cost[i][j]+OPT[j+1];
                    S[i]=j;
                }
            }
        }

        j=0;
    
        while (j<= n-1){
            k=0;
            for(i= j ; i<= S[j]; i++){
                a=t[j][S[j]];
                if(i<a){
                    OPTx[i] = QQ[j][j+k] * Q;
                    k++;
                }
                else{ 
                    if(i==a){

                        OPTx[i]= cheap[j][S[j]];
                    }
                    else{

                        OPTx[i]=0;
                    }
                }
            }  
            OPTx[S[j]]= last[j][S[j]];
            j=S[j]+1;
        }

        fprintf(fw, "\ndd: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", dd[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nhh: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", hh[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nA: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", A[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nHH: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", HH[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nQQ: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", QQ[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nII: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", II[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nhold: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", hold[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nCost: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                if(Cost[i][j]==inf){

                    fprintf(fw,"%5s\t", str);
                }
                else{

                    fprintf(fw,"%5d\t", Cost[i][j]);
                }
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nlast: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", last[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\ncheap: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){

                fprintf(fw,"%5d\t", cheap[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nt: \n");
        for (i=0; i<n; i++){  

            for (j=0; j<n; j++){
            
                fprintf(fw,"%5d\t", t[i][j]);
            }
            fprintf(fw,"\n");
        }

        fprintf(fw,"\nOPT: \n");
        fprintf(fw,"%5d\t", OPT[0]);
        fprintf(fw,"\nOPTx: \n");

        for (i=0; i<n; i++){      

            fprintf(fw,"%5d\t", OPTx[i]);       
        }

        fprintf(fw,"\nOPTi: \n");
        for (i=0; i<n; i++){      

            fprintf(fw,"%5d\t", OPT[i]);       
        }

        fprintf(fw,"\nS: \n");
        for (i=0; i<n; i++){      

            fprintf(fw,"%5d\t", S[i]);       
        }

        fprintf(fw,"\n");
        fprintf(fw,"================================================================================\n");        
    }
   
    fclose(fr); 
    fclose(fw); 
    return 0;
} 


int prepro_d(int i, int j, int* d){

    int k, dd=0;
    if (j<i){

        return 0;
    }
    else{

        for (k=i; k<=j; k++){

            dd=dd+ d[k];
        }
        return dd;
    }
}


int prepro_h(int i, int j, int* h){

    int k, hh=0;
    if (j<i){

        return 0;
    }
    else{

        for(k=i; k<=j; k++){

            hh=hh+h[k];
        }
        return hh;
    }
}

int prepro_H(int n, int i, int j, int dd[][n], int h[]){
    int k, HH=0;
    if (j<i){

        return 0;
    }
    else{

        for(k=i; k<=j; k++){

            HH=HH + h[k] * (dd[i][k]);
        }
        return HH;
    }
}

int prepro_A(int n, int i, int j, int a, int dd[][n], int B[]){
    if (j<i){

        return 0;
    }
    else{

        if (j==i){

            return B[i]+dd[i][i];        
        }

        else{

            if(a-dd[i][j]<=B[j]){
                return a;
            }

            else{

                return dd[i][j]+B[j];    
            }
        }
    }
}

int c1(int n, int t, int j, int I, int hold_Q, int h[], int d[], int p1[], int p2[],  int B[], int A[][n], int dd[][n], int hh[][n], int HH[][n]){
    
    int a, b, c, e, cost1, cost2;    

    a= dd[t][j-1] - I+1;

    if( a < Q){

        a=Q;
    }

    b= A[t][j-1] - I;

    if( a > b){

        return inf;
    }

    c= dd[t][j]- I- (Q-1);

    if ( a < c ){

        a=c;                
    }
    e =  dd[t][j] - I; 

    if ( b > e){

        b=e;
    }

    if ((a > b)){

        cost1 = inf;
    }
    else{

        if (p2[t]+ hh[t][j-1] >= p1[j]) {

            x = a;
            y = dd[t][j] - I - x;
            cost1 = hold_Q + f1(x, y, I, hh[t][j-1], HH[t][j-1], p2[t], p1[j]);
        }
        else{

            x = b;
            y = dd[t][j] - I - x;
            cost1 = hold_Q + f1(x, y, I, hh[t][j-1], HH[t][j-1], p2[t], p1[j]);
        }
    }

    if (j != n-1){

        return cost1;
    }
    else{

        a= dd[t][j] + 1 - I;

        if( a < Q){

            a=Q;
        }

        e = dd[t][j] + B[j] - I;
        b= A[t][j-1] - I;

        if (b > e){

           b=e;
        }

        if (a > b ){

            cost2=inf;
        }
        else{

            cost2= hold_Q + f1(a, 0, I, hh[t][j-1], HH[t][j-1], p2[t], p1[j]) + h[n-1] * (a+I-dd[t][j]);          
        }
        if( cost1 <= cost2){

            return cost1;
        }
        else{

            x=a;
            y=0;
            return cost2;
        }
    }
}


int c2(int n, int t, int j, int I, int hold_Q, int h[], int d[], int p2[],  int B[], int A[][n], int dd[][n], int hh[][n], int HH[][n]){
    
    int a, b, c, e, cost1, cost2;  

    x=Q;  

    a= dd[t][j-1] - I+1;

    if( a > Q){

        return inf;
    }

    b= A[t][j-1] - I;

    if( Q > b){

        return inf;
    }
  
    e =  dd[t][j] - I - Q; 

    if ( Q > e){

        return inf;
    }

    y = dd[t][j] - I - Q;
    cost1 = hold_Q + f2(x, y, I, hh[t][j-1], HH[t][j-1], p2[t], p2[j]);

    if (j != n-1){

        return cost1;
    }
    else{

        c= dd[t][j] + 1 - I - Q;

        if( a > c){

            a=c;      
        }

        if ( a > Q){

            return inf;
        }

        e = dd[t][j] + B[j] - I - Q;
        b= A[t][j-1] - I;

        if (b > e){

            b=e;
        }

        if ( Q > b ){

            return inf;  
        }
   
        cost2= hold_Q + f2(Q, Q, I, hh[t][j-1], HH[t][j-1], p2[t], p2[j]) + h[n-1] * (Q+I+Q-dd[t][j]);          
        
        if( cost1 <= cost2){

            return cost1;
        }
        else{
            
            y=Q;
            return cost2;
        }
    }
 }

int f1(int x_t, int x_j, int I, int h, int H, int p_2_t, int p_1_j){

    return (p_2_t +h) * x_t + p_1_j * x_j + h * I - H ;
}

int f2(int x_t, int x_j, int I, int h, int H, int p_2_t, int p_2_j){

    return (p_2_t + h) * x_t + p_2_j * x_j + h * I - H ;
}


