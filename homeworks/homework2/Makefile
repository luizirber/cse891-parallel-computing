SHELL=/bin/bash

ALL_MODULES=OpenMPI MVAPICH MVAPICH GNU Intel
PROCS=1 4 16 64 256
VECTOR_SIZE=10 100 5000

#all: exec/hello-GNU-MVAPICH exec/hello-GNU-MVAPICH2 exec/hello-GNU-OpenMPI exec/hello-Intel-MVAPICH2 exec/hello-Intel-OpenMPI

all: exec/alg2-GNU-OpenMPI

COMPILE:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} TAU && \
	tau_cc.sh -optCompInst -std=c99 -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}

SUBMIT:
	echo "cd ${WORKDIR}; module load TAU; mpiexec tau_exec ./${EXECUTABLE}" | cat pbs/header.sub - pbs/footer.sub | \
    qsub -l walltime=00:01:00,nodes=${NODES}:ppn=1 -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

###########################
#
# Vector reduction (alg 2)
#
###########################

exec/alg2-GNU-OpenMPI: export TMODULES = GNU OpenMPI
exec/alg2-GNU-OpenMPI: export EXEC_SRC = src/alg2.c
exec/alg2-GNU-OpenMPI: src/alg2.c
	mkdir -p exec
	for n in ${VECTOR_SIZE}; do \
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/alg2-GNU-OpenMPI-$$n COMPILE; \
	done

workdir/alg2-GNU-OpenMPI.pbs: export EXEC_BASE=alg2-GNU-OpenMPI
workdir/alg2-GNU-OpenMPI.pbs: export WORKDIR_BASE = workdir/alg2
workdir/alg2-GNU-OpenMPI.pbs:
	for p in ${PROCS}; do \
      for n in ${VECTOR_SIZE}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
        $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n/$$p EXECUTABLE=${EXEC_BASE}-$$n NODES=$$p SUBMIT; \
      done; \
    done

######################
#
# Hello world example
#
######################

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
