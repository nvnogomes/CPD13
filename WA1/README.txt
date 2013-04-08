Na pasta raiz do trabalho (WA1/)

1- Estrutura de ficheiros
WA1/
 D seq      - código mandel sequencial
 D openMPI  - código mandel openmpi
 D openMP2I - codigo mandel openMPI + openMP
 D openCL   - código mandel openCL
 D bin      - binários e objectos biblioteca
 D lib      - biblioteca de funções
 F mandel_X - imagem gerada através da tecnologia X
 F my-hosts - maquinas definidas para tecnologia openMPI
 F timer.sh - script utilizado para calcular a média do tempo de execução
 F assign_desc - enunciado do problema
 F Makefile - definição de regras de compilação

Dentro de cada pasta das diferentes tecnologias existe um ficheiro de texto com a estratégia usada e tempos retirados.


2- Makefile

Compilar programa em separado
	 make <comp_versao>

comp_versao:
	Sequencial: 	seq
	openMPI:	mpi
	openMP dyn: 	dmp
	openMP sta:	smp
	openMPI + MP:	mp2i
	openCL:		cl

Compilação regras gerais
	all
	clean
	
	
3- Correr programas (excepto MPI)
	./bin/<versao>

versao:
	mandelbrot_seq
	mandelbrot_smp <threads>
	mandelbrot_dmp <threads>
	mandelbrot_mp2i
	mandelbrot_cl
	


4- Obtenção de tempos
Os tempos apresentados são a média de 10 execuções consecutivas do programa.
Estes tempos são obtidos com recurso à função walltime dada na aula prática. Os tempos foram retirados apenas no calculo da cor em cada pixel, deixando de fora a construção do ficheiro de imagem.



Correr mpi localmente
	mpirun -n <threads> ./bin/mandelbrot_mpi

Correr mpi no condor
-Arrancar a máquina virtual é preciso fazer:
	lamboot my-hosts

-Desligar a máquina virtual:
	lamhalt





