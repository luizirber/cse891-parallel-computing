SHELL=/bin/bash

ALL_MODULES=OpenMPI MVAPICH MVAPICH GNU Intel
TMODULES = GNU OpenMPI

all: exec/hello-GNU-MVAPICH exec/hello-GNU-MVAPICH2 exec/hello-GNU-OpenMPI exec/hello-Intel-MVAPICH2 exec/hello-Intel-OpenMPI

exec/hello-GNU-MVAPICH: TMODULES = GNU MVAPICH
exec/hello-GNU-MVAPICH: src/mpi-hello.c
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc src/mpi-hello.c -o $@

exec/hello-GNU-MVAPICH2: TMODULES = GNU MVAPICH2
exec/hello-GNU-MVAPICH2: src/mpi-hello.c
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc src/mpi-hello.c -o $@

exec/hello-GNU-OpenMPI: TMODULES = GNU OpenMPI
exec/hello-GNU-OpenMPI: src/mpi-hello.c
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc src/mpi-hello.c -o $@

exec/hello-Intel-OpenMPI: TMODULES = Intel OpenMPI
exec/hello-Intel-OpenMPI: src/mpi-hello.c
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc src/mpi-hello.c -o $@

exec/hello-Intel-MVAPICH2: TMODULES = Intel MVAPICH2
exec/hello-Intel-MVAPICH2: src/mpi-hello.c
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc src/mpi-hello.c -o $@

clean:
	- rm *.o
