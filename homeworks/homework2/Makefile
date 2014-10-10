SHELL=/bin/bash

ALL_MODULES=OpenMPI MVAPICH MVAPICH GNU Intel TAU
PROCS=1 4 16 64 #256
VECTOR_SIZE=10 100 5000

VECTOR_SIZE_P2=128 512 1024 1600 6144
BLOCK_SIZES=16 32 64

MKL_INCLUDE=/opt/software/ClusterStudio/2011.0/mkl/include
MKL_LIBS=-lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lm



#all: exec/alg2-GNU-OpenMPI exec/alg3-GNU-OpenMPI
all: exec/alg3-GNU-OpenMPI

COMPILE:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load ${TMODULES} && \
	tau_cc.sh -optCompInst -std=gnu99 -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} -DBLOCK_SIZE=${BSIZE} ${EXEC_SRC}

MKL_COMPILE:
	mkdir -p exec/
	$(CC) -Wall -I$(MKL_INCLUDE) $(CFLAGS) -o ${EXEC_TARGET} ${EXEC_SRC} $(MKL_LIBS)

SUBMIT:
	echo "cd ${WORKDIR}/\$$p; module unload ${ALL_MODULES}; module load ${TMODULES}; mpiexec -np \$$p tau_exec ./${EXECUTABLE};" | cat pbs/header.sub - pbs/footer.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:20:00,nodes=256:ppn=1 -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

############
# Local dev
############

COMPILE_LOCAL:
	mkdir -p exec/
	mpicc -std=gnu99 -lblas -lm -o ${EXEC_TARGET} -DBLOCK_SIZE=${BSIZE} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}

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
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/alg3-GNU-OpenMPI-$$n COMPILE_LOCAL; \
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
      $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_LOCAL; \
    done

################################
#
# Vector reduction (MPI_reduce)
#
################################

exec/reduce-GNU-OpenMPI: export TMODULES = GNU OpenMPI TAU
exec/reduce-GNU-OpenMPI: export EXEC_SRC = src/reduce.c
exec/reduce-GNU-OpenMPI: src/reduce.c
	mkdir -p exec
	for n in ${VECTOR_SIZE}; do \
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/reduce-GNU-OpenMPI-$$n COMPILE_LOCAL; \
	done

workdir/reduce-GNU-OpenMPI.pbs: export TMODULES = GNU OpenMPI TAU
workdir/reduce-GNU-OpenMPI.pbs: export EXEC_BASE=reduce-GNU-OpenMPI
workdir/reduce-GNU-OpenMPI.pbs: export WORKDIR_BASE = workdir/reduce
workdir/reduce-GNU-OpenMPI.pbs:
	for n in ${VECTOR_SIZE}; do \
      for p in ${PROCS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_LOCAL; \
    done

################################
#
# Matrix multiplication (serial)
#
################################

exec/matmult-serial-GNU-OpenMPI: export TMODULES = GNU OpenMPI TAU
exec/matmult-serial-GNU-OpenMPI: export EXEC_SRC = src/matmult-serial.c
exec/matmult-serial-GNU-OpenMPI: src/matmult-serial.c
	mkdir -p exec
	for n in ${VECTOR_SIZE_P2}; do \
      for b in ${BLOCK_SIZES}; do \
	    $(MAKE) NSIZE=$$n BSIZE=$$b EXEC_TARGET=exec/matmult-serial-GNU-OpenMPI-$$n-$$b COMPILE_LOCAL; \
	  done; \
	done

workdir/matmult-serial-GNU-OpenMPI.pbs: export TMODULES = GNU OpenMPI TAU
workdir/matmult-serial-GNU-OpenMPI.pbs: export EXEC_BASE=matmult-serial-GNU-OpenMPI
workdir/matmult-serial-GNU-OpenMPI.pbs: export WORKDIR_BASE = workdir/matmult-serial
workdir/matmult-serial-GNU-OpenMPI.pbs:
	for n in ${VECTOR_SIZE_P2}; do \
      for p in ${PROCS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_LOCAL; \
    done

exec/matmult-dgemm-GNU-OpenMPI: export TMODULES = GNU OpenMPI TAU
exec/matmult-dgemm-GNU-OpenMPI: export EXEC_SRC = src/matmult-dgemm.c
exec/matmult-dgemm-GNU-OpenMPI: src/matmult-dgemm.c
	mkdir -p exec
	for n in ${VECTOR_SIZE_P2}; do \
	  $(MAKE) NSIZE=$$n BSIZE=$$b EXEC_TARGET=exec/matmult-dgemm-GNU-OpenMPI-$$n-$$b MKL_COMPILE; \
	done

workdir/matmult-dgemm-GNU-OpenMPI.pbs: export TMODULES = GNU OpenMPI TAU
workdir/matmult-dgemm-GNU-OpenMPI.pbs: export EXEC_BASE=matmult-dgemm-GNU-OpenMPI
workdir/matmult-dgemm-GNU-OpenMPI.pbs: export WORKDIR_BASE = workdir/matmult-dgemm
workdir/matmult-dgemm-GNU-OpenMPI.pbs:
	for n in ${VECTOR_SIZE_P2}; do \
      for p in ${PROCS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_LOCAL; \
    done

##############################################
#
# Local development target (just for testing)
#
##############################################

run_local: exec/matmult-dgemm-GNU-OpenMPI
	for n in ${VECTOR_SIZE}; do \
        for b in ${BLOCK_SIZES}; do \
		  echo -n "$$n $$p $$b: " ; \
	      mpiexec -np $$p exec/matmult-dgemm-GNU-OpenMPI-$$n-$$b; \
	    done; \
	  done; \
    done;

run_once: exec/matmult-dgemm-GNU-OpenMPI
	exec/matmult-dgemm-GNU-OpenMPI-128-16

clean:
	- rm -rf *.o exec/

.PHONY: COMPILE
