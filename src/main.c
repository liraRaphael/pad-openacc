#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#define posicao(I, J, COLUNAS) ((I)*(COLUNAS) + (J))


/**
 *
 *  - TRABALHO DE PAD - Código otimizado com OpenACC
 *  TELEPROG
 *    - Raphael Lira dos Santos 223865
 *    - Elziele Da Rocha 196396
 *
**/





/***
 *
 *
 *
 * - CABEÇALHO DAS FUNÇÕES
 *
 *
**/
float random_number();
float * alocar(int dimensaoA,int dimensaoB);
float * gerarMatriz(char * path,int dimensaoA,int dimensaoB);
float * lerArquivo(char * path,int dimensaoA,int dimensaoB);
void calculaMatrizDABC();
void reducaoMatrizD();



/***
 *
 *
 *
 * - Declaração das variaveisVariavies 
 *
 *
**/
int 
    y,w,v, //variavel que guardará os valores da coluna
	i,j,k; // variaveis de controle

//aloca e le os arquivos do vetor
float 
	* matrizA, 
	* matrizB, 
	* matrizC, 
	* matrizD,
	* matrizAB;

double
	reducao; //salvara o resultado da redução
	
/***
 *
 *
 *
 * - FUNÇÕES
 *
 *
**/
//gerador de numeros aleatórios
float random_number(){
	return ((float) (rand() % 2000)/100)  - 10;
}


// aloca dinamicamente como proposto pelo professor
float * alocar(int dimensaoA,int dimensaoB){
	float * ponteiro;
	ponteiro = (float *) calloc(dimensaoA * dimensaoB,sizeof(float));	
	return ponteiro;
}


// gera a matriz de forma aleatória
float * gerarMatriz(char * path,int dimensaoA,int dimensaoB){
	
  	// manipula o arquivo para escrita
	FILE * arquivo;
	arquivo = fopen(path,"w");
	
  	//aloca e define o tamanho total
	float * matriz = alocar(dimensaoA,dimensaoB);	
	int MAX = dimensaoA * dimensaoB;
	
	int i = 0;
		
  	//faz o loop, atribuindo valor aleatório e salva o arquivo
	for(i = 0;i<MAX;i++){
		matriz[i] = random_number();
		fprintf(arquivo,"%.2f\n",matriz[i]);
	}
	
	fclose(arquivo);
	
	return matriz;
	
}


// le o arquivo 
float * lerArquivo(char * path,int dimensaoA,int dimensaoB){
	
  	//abre o arquivo para leitura
	FILE * arquivo;
	arquivo = fopen(path,"r");
	
  	//caso o arquivo não exista, gera um novo
	if(!arquivo){		
		return NULL;
	}
		
		
  	// aloca a matriz dinamicamente
	float * matriz = alocar(dimensaoA,dimensaoB);	
	int MAX = dimensaoA * dimensaoB;
	
	int i = 0;
	
 
   	// faz o loop de leitura
	for(i = 0;i<MAX;i++){
		fscanf(arquivo,"%f\n", &matriz[i]);	
	}
	
	fclose(arquivo);
	
	return matriz;
	
}



/**
*
*
* - Calcula a matriz D = (A x B) x C
*
*/
void calculaMatrizDABC(){
  
	#pragma acc parallel loop gang collapse(3)
	for(i=0;i<y;i++){	       							
		for(j=0;j<v;j++){      						
			for(k=0;k<w;k++){	
				matrizAB[posicao(i,j,v)] += (matrizA[posicao(i,k,w)] * matrizB[posicao(k,j,v)]) ;										
			}
		}					
	}

	#pragma acc parallel loop gang collapse(2)
	for(i=0;i<y;i++){	 
	    for(j=0;j<v;j++){	      
			  matrizD[i] += matrizAB[posicao(i,j,v)] * matrizC[j];							
	    }	
	}
	 
}


void reducaoMatrizD(){

   	reducao = 0;
    
	#pragma acc parallel loop gang reduction(+:reducao)
	for(i=0;i<y;i++){             
		reducao += matrizD[i];
	}
 
 
}



/***
 *
 *
 *
 * - MAIN
 *
 *
**/
int main(int argc,char ** argv){
	
  	// verifica se todos os argumentos estão
	if(argc != 8){
		printf("argumentos invalidos!\n");
		return 1;
	}	
    
	clock_t 
		tIni,tFim;  	
		
	// atribui os valores de dimensão da matriz
   	y = atoi(argv[1]);
  	w = atoi(argv[2]);
  	v = atoi(argv[3]);
	  
	// aloca os dados nas matrizes  	
	matrizA = lerArquivo(argv[4],y,w); 
	matrizB = lerArquivo(argv[5],w,v); 
	matrizC = lerArquivo(argv[6],v,1); 
	
	// gera uma matriz AB limpa
	matrizAB = alocar(y,v);
		
	if(y == 0 || w == 0 || v == 0){
		printf("Valor(es) y,w e/ou v invalido(s)!\n");
		return 1;
	}
	
  	// caso falhe algum arquivo
	if(matrizA == NULL || matrizB == NULL || matrizC == NULL){	
		printf("Matriz(es) não carregada(s)!\n");	
		return 1;
	}  

 	// gera e zera a "vetor" D
	matrizD = alocar(y,1); 	
 
   //grava o tempo incial 
	tIni = clock(); 
	   
	#pragma acc enter data copyin(matrizA[0:y*w],matrizB[0:w*v],matrizC[0:v],matrizD[0:y],matrizAB[0:y*v],y,w,v) create(reducao,i,j,k) 
	
	
	
	calculaMatrizDABC();
	reducaoMatrizD();	 
	 
	
	#pragma acc exit data delete(matrizA,matrizB,matrizC,matrizAB,y,w,v,i,j,k) copyout(reducao,matrizD)
		
	//grava o tempo final
	tFim = clock();
  	// printa a redução e o tempo
	printf("o resultado da reducao foi: %f - o tempo exercido foi de %f segundos\n",reducao,(double) (tFim - tIni)/CLOCKS_PER_SEC);
	
  	// abre o arquivo para gravação da matriz D
	FILE * arquivo;
	arquivo = fopen(argv[7],"w");
	
  	//faz o loop de gravação
	for(i=0;i<y;i++){
		fprintf(arquivo,"%.2f\n",matrizD[i]);
	}
	
	fclose(arquivo);	
	
	return 0;  
  
}
