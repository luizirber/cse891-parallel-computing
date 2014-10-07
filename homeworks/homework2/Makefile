SHELL=/bin/bash

ALL_MODULES=OpenMPI MVAPICH MVAPICH GNU Intel
RESOURCES=-l walltime=00:01:00,nodes=4:ppn=1 #-A ged-intel11

#all: exec/hello-GNU-MVAPICH exec/hello-GNU-MVAPICH2 exec/hello-GNU-OpenMPI exec/hello-Intel-MVAPICH2 exec/hello-Intel-OpenMPI

all: exec/alg2-GNU-OpenMPI

COMPILE:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	mpicc -std=c99 ${EXEC_SRC} -o ${EXEC_TARGET}

SUBMIT:
	echo "mpiexec ${EXECUTABLE}" | cat pbs/header.sub - pbs/footer.sub | \
    qsub ${RESOURCES} -N ${EXECUTABLE} -o workdir/${EXECUTABLE}.pbs -e workdir/${EXECUTABLE}.err

###########################
#
# Vector reduction (alg 2)
#
###########################

exec/alg2-GNU-OpenMPI: export TMODULES = GNU OpenMPI
exec/alg2-GNU-OpenMPI: export EXEC_TARGET = exec/hello-GNU-OpenMPI
exec/alg2-GNU-OpenMPI: export EXEC_SRC = src/alg2.c
exec/alg2-GNU-OpenMPI: src/alg2.c
	$(MAKE) COMPILE

workdir/alg2-GNU-OpenMPI.pbs: export EXECUTABLE = exec/hello-GNU-OpenMPI
workdir/alg2-GNU-OpenMPI.pbs:
	mkdir -p workdir/exec
	$(MAKE) SUBMIT

###########################
#
# Vector reduction (alg 2)
#
###########################

exec/hello-GNU-MVAPICH: export TMODULES = GNU MVAPICH
exec/hello-GNU-MVAPICH: export EXEC_TARGET = exec/hello-GNU-MVAPICH
exec/hello-GNU-MVAPICH: export EXEC_SRC = src/mpi-hello.c
exec/hello-GNU-MVAPICH: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-GNU-MVAPICH2: export TMODULES = GNU MVAPICH2
exec/hello-GNU-MVAPICH2: export EXEC_TARGET = exec/hello-GNU-MVAPICH2
exec/hello-GNU-MVAPICH2: export EXEC_SRC = src/mpi-hello.c
exec/hello-GNU-MVAPICH2: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-GNU-OpenMPI: export TMODULES = GNU OpenMPI
exec/hello-GNU-OpenMPI: export EXEC_TARGET = exec/hello-GNU-OpenMPI
exec/hello-GNU-OpenMPI: export EXEC_SRC = src/mpi-hello.c
exec/hello-GNU-OpenMPI: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-Intel-OpenMPI: export TMODULES = Intel OpenMPI
exec/hello-Intel-OpenMPI: export EXEC_TARGET = exec/hello-Intel-OpenMPI
exec/hello-Intel-OpenMPI: export EXEC_SRC = src/mpi-hello.c
exec/hello-Intel-OpenMPI: src/mpi-hello.c
	$(MAKE) COMPILE

exec/hello-Intel-MVAPICH2: export TMODULES = Intel MVAPICH2
exec/hello-Intel-MVAPICH2: export EXEC_TARGET = exec/hello-GNU-MVAPICH2
exec/hello-Intel-MVAPICH2: export EXEC_SRC = src/mpi-hello.c
exec/hello-Intel-MVAPICH2: src/mpi-hello.c
	$(MAKE) COMPILE

clean:
	- rm -rf *.o exec/

.PHONY: COMPILE
