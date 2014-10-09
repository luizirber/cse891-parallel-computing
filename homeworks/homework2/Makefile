SHELL=/bin/bash

ALL_MODULES=OpenMPI MVAPICH MVAPICH GNU Intel TAU
PROCS=1 4 16 64 256
VECTOR_SIZE=10 100 5000

#all: exec/alg2-GNU-OpenMPI exec/alg3-GNU-OpenMPI
all: exec/alg3-GNU-OpenMPI

COMPILE:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	tau_cc.sh -optCompInst -std=c99 -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}

SUBMIT:
	echo "cd ${WORKDIR}/\$$p; module unload ${ALL_MODULES}; module load ${TMODULES}; mpiexec -np \$$p tau_exec ./${EXECUTABLE};" | cat pbs/header.sub - pbs/footer.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:20:00,nodes=256:ppn=1 -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

############
# Local dev
############

COMPILE_LOCAL:
	mkdir -p exec/
	mpicc -std=c99 -lm -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}

SUBMIT_LOCAL:
	echo "cd ${WORKDIR}/\$$p; module unload ${ALL_MODULES}; module load ${TMODULES}; mpiexec -np \$$p tau_exec ./${EXECUTABLE};" | cat pbs/header.sub - pbs/footer.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:20:00,nodes=256:ppn=1 -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

###########################
#
# Vector reduction (alg 2)
#
###########################

exec/alg2-GNU-OpenMPI: export TMODULES = GNU OpenMPI TAU
exec/alg2-GNU-OpenMPI: export EXEC_SRC = src/alg2.c
exec/alg2-GNU-OpenMPI: src/alg2.c
	mkdir -p exec
	for n in ${VECTOR_SIZE}; do \
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/alg2-GNU-OpenMPI-$$n COMPILE; \
	done

workdir/alg2-GNU-OpenMPI.pbs: export TMODULES = GNU OpenMPI TAU
workdir/alg2-GNU-OpenMPI.pbs: export EXEC_BASE=alg2-GNU-OpenMPI
workdir/alg2-GNU-OpenMPI.pbs: export WORKDIR_BASE = workdir/alg2
workdir/alg2-GNU-OpenMPI.pbs:
	for n in ${VECTOR_SIZE}; do \
      for p in ${PROCS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT; \
    done

###########################
#
# Vector reduction (alg 3)
#
###########################

exec/alg3-GNU-OpenMPI: export TMODULES = GNU OpenMPI TAU
exec/alg3-GNU-OpenMPI: export EXEC_SRC = src/alg3.c
exec/alg3-GNU-OpenMPI: src/alg3.c
	mkdir -p exec
	for n in ${VECTOR_SIZE}; do \
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/alg3-GNU-OpenMPI-$$n COMPILE; \
	done

workdir/alg3-GNU-OpenMPI.pbs: export TMODULES = GNU OpenMPI TAU
workdir/alg3-GNU-OpenMPI.pbs: export EXEC_BASE=alg3-GNU-OpenMPI
workdir/alg3-GNU-OpenMPI.pbs: export WORKDIR_BASE = workdir/alg3
workdir/alg3-GNU-OpenMPI.pbs:
	for n in ${VECTOR_SIZE}; do \
      for p in ${PROCS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT; \
    done

run_local: exec/alg3-GNU-OpenMPI
	for n in ${VECTOR_SIZE}; do \
      for p in ${PROCS}; do \
		  echo -n "$$n $$p: " ; \
	      mpiexec -np $$p exec/alg3-GNU-OpenMPI-$$n; \
	  done; \
    done;

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
