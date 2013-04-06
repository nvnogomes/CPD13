Na pasta raiz do trabalho (WA1/)

Estrutura de ficheiros
WA1/
 D seq      - código mandel sequencial
 D openMPI  - código mandel openmpi
 D openMP2I - codigo mandel openMPI + openMP
 D openCL   - código mandel openCL
 D bin      - binários e objectos biblioteca
 D lib      - biblioteca de funções
 F mandel_X - imagem gerada através da tecnologia X


COMPILAÇÃO

Compilar programa em separado
	 make <comp_versao>

comp_versao:
	Sequencial - seq
	MPI: mpi
	dmp
	smp
	mp2i
	cl

Compilação regras gerais
	all
	clean
	
	
Correr todos (excepto MPI)
	./bin/<versao>

versao:
	mandelbrot_seq
	mandelbrot_smp <threads>
	mandelbrot_dmp <threads>
	mandelbrot_mp2i *
	mandelbrot_cl *
	
* ainda não realizados



Correr mpi localmente
	mpirun -n <threads> ./bin/mandelbrot_mpi

Correr mpi no condor
-Arrancar a máquina virtual é preciso fazer:
	lamboot my-hosts

-Desligar a máquina virtual:
	lamhalt





