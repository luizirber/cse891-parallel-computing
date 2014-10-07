SHELL=/bin/bash

ALL_MODULES=OpenMPI MVAPICH MVAPICH GNU Intel

all: exec/hello-GNU-MVAPICH exec/hello-GNU-MVAPICH2 exec/hello-GNU-OpenMPI exec/hello-Intel-MVAPICH2 exec/hello-Intel-OpenMPI

COMPILE:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc src/mpi-hello.c -o ${EXEC_TARGET}

exec/hello-GNU-MVAPICH: export TMODULES = GNU MVAPICH
exec/hello-GNU-MVAPICH: export EXEC_TARGET = exec/hello-GNU-MVAPICH
exec/hello-GNU-MVAPICH: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-GNU-MVAPICH2: export TMODULES = GNU MVAPICH2
exec/hello-GNU-MVAPICH2: export EXEC_TARGET = exec/hello-GNU-MVAPICH2
exec/hello-GNU-MVAPICH2: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-GNU-OpenMPI: export TMODULES = GNU OpenMPI
exec/hello-GNU-OpenMPI: export EXEC_TARGET = exec/hello-GNU-OpenMPI
exec/hello-GNU-OpenMPI: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-Intel-OpenMPI: export TMODULES = Intel OpenMPI
exec/hello-Intel-OpenMPI: export EXEC_TARGET = exec/hello-Intel-OpenMPI
exec/hello-Intel-OpenMPI: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-Intel-MVAPICH2: export TMODULES = Intel MVAPICH2
exec/hello-Intel-MVAPICH2: export EXEC_TARGET = exec/hello-GNU-MVAPICH2
exec/hello-Intel-MVAPICH2: src/mpi-hello.c
	$(MAKE) COMPILE

clean:
	- rm -rf *.o exec/

.PHONY: COMPILE
