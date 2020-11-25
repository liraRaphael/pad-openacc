# PAD OpenACC

## Sobre
Nos, como alunos da FT, iremos fazer um projeto com a linguagem C(voltado para programação de alto desempenho), usando a biblioteca OpenACC


## Como usar

### Compilando

Use o seguinte comando no sistema **Linux**:

1. vá até a pasta pad-openmp/src

2. > make
  
### Após compilado

1. Vá até a pasta pad-openmp/bin:

2. > ./programa y w v arqA.dat arqB.dat arqC.dat arqD.dat

Onde:
+ ./programa é o programa que resolverá o problema.
+ y é o número de linhas da primeira matriz.
+ w é o número de colunas da primeira matriz e de linhas da segunda matriz.
+ v é o número de colunas da segunda matriz e de linhas da terceira matriz.
+ arqA.dat é o nome do arquivo que contém a primeira matriz.
+ arqB.dat é o nome do arquivo que contém a segunda matriz.
+ arqC.dat é o nome do arquivo que contém a terceira matriz. 
+ arqD.dat é o nome do arquivo que contém a matriz resultante da computação.

Observe um exemplo a seguir.

1. vá até a pasta pad-openmp/bin

2. >  ./programa 10 10 10 arqA.dat arqB.dat arqC.dat arqD.dat

### Observação
Caso não haja os problema com algum dos parametros, o programa retornará erro

## Integrantes
Teleprog

+ Raphael Lira dos Santos 223865
+ Elziele Da Rocha 196396
