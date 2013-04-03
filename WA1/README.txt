Na pasta WA1/

Compilar programa em separada
	genericamente make <comp_versao>

comp_versao:
	seq
	mpi
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
	
Correr mpi
	mpirun -n <threads> ./bin/mandelbrot_mpi

As imagens geradas pelos programas são criadas na pasta WA1/.
