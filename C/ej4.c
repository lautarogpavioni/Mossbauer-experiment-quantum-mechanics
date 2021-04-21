#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int L=5, red[L][L]; /*mg: mag inicial, L longitud de red */
    int vecSum, j , i, spin,s[L][L], delmg;
    double mg=0, E, delE, B, prob, E_t,E_t2, p, ce, delmgt,delEt;
    double mg_t, mg_t2, su,E_i;

    FILE *fp;
    
    srand(time(NULL));  //semilla

    fp=fopen("ej4_red_5_h.txt","w+");
    
    B=0.5;     //el campo magnetico (segundo cociente h de la ec en la practica)


     /* me armo la red inicial*/
  
    for (int i=0;i<L;i++){
        for (int j=0; j<L; j++){
            //red[i][j]=2*(rand()%2)-1;  //le doy un valor random -1 o 1 a cada slot de la red
            red[i][j]=1;
            mg+=red[i][j];   //calculo la magnetizacion total de la red
            //fprintf(fpt,"%d\n",mg);
            
        }
    }
    E=0;
    
    ce=0;
    su=0;

    for(int i=0; i<L;i++){
        for (int j=0; j<L; j++){
            //hago la suma de las casillas alrededor de la casilla i,j:
            vecSum=red[(i+1)%L][j]+red[(i-1+L)%L][j]+red[i][(j+1)%L]+red[i][(j-1+L)%L];
            //calculo la energia total como la suma de la E de cada casilla i,j:
            E+=(-red[i][j] *vecSum/2. - B*mg/(L*L)); //el - es porque a temperatura inicial cero
            // la E tiene que ser minima
            
        }
    }
    E_i=E;
    for (double T=0.1; T<10;T+=0.05){ //hago la iteracion sobre las T
        E_t=0;
        E_t2=0;
        mg_t=0;
        mg_t2=0;
        for(int n=0; n<1000000; n++){  //por cada valor de T hago n iteraciones
    
            j=rand()%L; //damos coordenada aleatoria entre 0 y 30 
            i=rand()%L;
        
            //hago la suma de espines de las celdas vecinas de la celda i,j:
            vecSum=red[(i+1)%L][j]+red[(i-1+L)%L][j]+red[i][(j+1)%L]+red[i][(j-1+L)%L];
    
            spin=red[i][j];
                        
            //fprintf(fpt,"%d, %d\n",spin, vecSum);

            //calculo el delmagnetizacion (si se cumple s<0 entonces es =1 si es falso =0)
            delmg=-spin*2;
            //fprintf(fpt,"%d\n",delmg);

            //delta de energia
            delE=(2*red[i][j]*vecSum-B*delmg);  // ?? el *2
            if(delE<=0){
                red[i][j]*=-1; //confirmamos el flip
                E+=delE;   //hacemos el cambio a la energia total
                mg+=delmg;  //hacemos el cambio a la magnetizacion total
                
                
            }
            if(delE>0){
                prob=exp(-delE/T);
                if(prob>(float)rand()/(float)RAND_MAX){
                    red[i][j]*=-1; //confirmamos el flip
                    E+=delE;
                    mg+=delmg;
                    
                    
                }
            }
            
        }
        
        //hacemos el equilibrio, paso monte carlo:

        for(int n=0; n<1000; n++){  //por cada valor de T hago n iteraciones
            delEt=0;
            delmgt=0;


            for (int p=0; p<(L*L);p++){
                            
                j=rand()%L; //damos coordenada aleatoria entre 0 y 30 
                i=rand()%L;
            
                //hago la suma de espines de las celdas vecinas de la celda i,j:
                vecSum=red[(i+1)%L][j]+red[(i-1+L)%L][j]+red[i][(j+1)%L]+red[i][(j-1+L)%L];
        
                spin=red[i][j];
                            
                //fprintf(fpt,"%d, %d\n",spin, vecSum);

                //calculo el delmagnetizacion (si se cumple s<0 entonces es =1 si es falso =0)
                delmg=-spin*2;
                //fprintf(fpt,"%d\n",delmg);

                //delta de energia
                delE=(2*red[i][j]*vecSum-B*delmg);  // ?? el *2
                if(delE<=0){
                    red[i][j]*=-1; //confirmamos el flip
                    delEt+=delE;   //hacemos el cambio a la energia total
                    delmgt+=delmg;  //hacemos el cambio a la magnetizacion total
                    
                }
            
                if(delE>0){
                    prob=exp(-delE/T);
                    if(prob>(float)rand()/(float)RAND_MAX){
                        red[i][j]*=-1; //confirmamos el flip
                        delEt+=delE;
                        delmgt+=delmg;
                    }
                }
                
                
            } 
            E+=delEt;
            mg+=delmgt;
            mg_t+=mg/1000;
            mg_t2+=mg*mg/1000;
            E_t+=E/1000;
            E_t2+=E*E/1000; 
            
        }
        
        
        //valor medio:
        double Lx;
        double x;
        ce= E_t2 - E_t*E_t ;
        su=mg_t2 - mg_t*mg_t;
        Lx= pow(L,-7/4.)*su;
        x= (T-2.2692)*L;
        fprintf(fp,"%lf, %lf\n",x,Lx);

    }
    fclose(fp);
    
}