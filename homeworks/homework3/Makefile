SHELL=/bin/bash

ALL_MODULES=GNU Intel TAU
THREADS=1 2 4 8 10 16 20
PHI_THREADS=1 2 4 8 16 32 64 128 240
VECTOR_SIZE=10 100 1000 10000 100000

all:

################
#
# Generic rules
#
################

COMPILE:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load GNU && \
	module load TAU && \
	tau_cc.sh -optCompInst -optTrackGOMP -fopenmp -lgomp -std=gnu99 -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}
	#$(CC) -std=gnu99 -fopenmp -lgomp -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}

SUBMIT_SERIAL:
	echo "cd ${WORKDIR}/\$$p; module unload ${ALL_MODULES}; module load GNU; module load TAU; export OMP_NUM_THREADS=\$$p ; ./${EXECUTABLE};" | cat pbs/header-serial.sub - pbs/footer-serial.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:${WTIME_MINUTES}:00,nodes=1:ppn=20 -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

################
#
# OpenMP i loop
#
################

exec/omp_iloop-GNU: export TMODULES = GNU TAU
exec/omp_iloop-GNU: export EXEC_SRC = src/omp_iloop.c
exec/omp_iloop-GNU: src/omp_iloop.c
	mkdir -p exec
	for n in ${VECTOR_SIZE}; do \
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/omp_iloop-GNU-$$n COMPILE; \
	done

workdir/omp_iloop.pbs: export TMODULES=GNU TAU
workdir/omp_iloop.pbs: export EXEC_BASE=omp_iloop-GNU
workdir/omp_iloop.pbs: export WTIME_MINUTES=05
workdir/omp_iloop.pbs: export WORKDIR_BASE=workdir/omp_iloop
workdir/omp_iloop.pbs:
	for n in ${VECTOR_SIZE}; do \
      for p in ${THREADS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WTIME_MINUTES=${WTIME_MINUTES} NUM_THREADS=$$p WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_SERIAL; \
    done

################
#
# OpenMP j loop
#
################

exec/omp_jloop-GNU: export TMODULES = GNU TAU
exec/omp_jloop-GNU: export EXEC_SRC = src/omp_jloop.c
exec/omp_jloop-GNU: src/omp_jloop.c
	mkdir -p exec
	for n in ${VECTOR_SIZE}; do \
	  $(MAKE) NSIZE=$$n EXEC_TARGET=exec/omp_jloop-GNU-$$n COMPILE; \
	done

workdir/omp_jloop.pbs: export TMODULES = GNU TAU
workdir/omp_jloop.pbs: export EXEC_BASE=omp_jloop-GNU
workdir/omp_jloop.pbs: export WTIME_MINUTES=15
workdir/omp_jloop.pbs: export WORKDIR_BASE=workdir/omp_jloop
workdir/omp_jloop.pbs:
	for n in ${VECTOR_SIZE}; do \
      for p in ${THREADS}; do \
		mkdir -p ${WORKDIR_BASE}/$$n/$$p ; \
		cp exec/${EXEC_BASE}-$$n ${WORKDIR_BASE}/$$n/$$p ; \
      done; \
      $(MAKE) WTIME_MINUTES=${WTIME_MINUTES} NUM_THREADS=$$p WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_SERIAL; \
    done

################
#
# Data analysis
#
################

workdir/%/tauprofile: workdir/%/profile.0.0.0
	module load TAU; \
	cd $(@D); \
	pprof > tauprofile

TAU_PROFILES_ILOOP: workdir/omp_iloop/10/1/tauprofile \
workdir/omp_iloop/10/2/tauprofile \
workdir/omp_iloop/10/4/tauprofile \
workdir/omp_iloop/10/8/tauprofile \
workdir/omp_iloop/10/10/tauprofile \
workdir/omp_iloop/10/16/tauprofile \
workdir/omp_iloop/10/20/tauprofile \
workdir/omp_iloop/100/1/tauprofile \
workdir/omp_iloop/100/2/tauprofile \
workdir/omp_iloop/100/4/tauprofile \
workdir/omp_iloop/100/8/tauprofile \
workdir/omp_iloop/100/10/tauprofile \
workdir/omp_iloop/100/16/tauprofile \
workdir/omp_iloop/100/20/tauprofile \
workdir/omp_iloop/1000/1/tauprofile \
workdir/omp_iloop/1000/2/tauprofile \
workdir/omp_iloop/1000/4/tauprofile \
workdir/omp_iloop/1000/8/tauprofile \
workdir/omp_iloop/1000/10/tauprofile \
workdir/omp_iloop/1000/16/tauprofile \
workdir/omp_iloop/1000/20/tauprofile \
workdir/omp_iloop/10000/1/tauprofile \
workdir/omp_iloop/10000/2/tauprofile \
workdir/omp_iloop/10000/4/tauprofile \
workdir/omp_iloop/10000/8/tauprofile \
workdir/omp_iloop/10000/10/tauprofile \
workdir/omp_iloop/10000/16/tauprofile \
workdir/omp_iloop/10000/20/tauprofile \
workdir/omp_iloop/100000/1/tauprofile \
workdir/omp_iloop/100000/2/tauprofile \
workdir/omp_iloop/100000/4/tauprofile \
workdir/omp_iloop/100000/8/tauprofile \
workdir/omp_iloop/100000/10/tauprofile \
workdir/omp_iloop/100000/16/tauprofile \
workdir/omp_iloop/100000/20/tauprofile

TAU_PROFILES_JLOOP: workdir/omp_jloop/10/1/tauprofile \
workdir/omp_jloop/10/2/tauprofile \
workdir/omp_jloop/10/4/tauprofile \
workdir/omp_jloop/10/8/tauprofile \
workdir/omp_jloop/10/10/tauprofile \
workdir/omp_jloop/10/16/tauprofile \
workdir/omp_jloop/10/20/tauprofile \
workdir/omp_jloop/100/1/tauprofile \
workdir/omp_jloop/100/2/tauprofile \
workdir/omp_jloop/100/4/tauprofile \
workdir/omp_jloop/100/8/tauprofile \
workdir/omp_jloop/100/10/tauprofile \
workdir/omp_jloop/100/16/tauprofile \
workdir/omp_jloop/100/20/tauprofile \
workdir/omp_jloop/1000/1/tauprofile \
workdir/omp_jloop/1000/2/tauprofile \
workdir/omp_jloop/1000/4/tauprofile \
workdir/omp_jloop/1000/8/tauprofile \
workdir/omp_jloop/1000/10/tauprofile \
workdir/omp_jloop/1000/16/tauprofile \
workdir/omp_jloop/1000/20/tauprofile \
workdir/omp_jloop/10000/1/tauprofile \
workdir/omp_jloop/10000/2/tauprofile \
workdir/omp_jloop/10000/4/tauprofile \
workdir/omp_jloop/10000/8/tauprofile \
workdir/omp_jloop/10000/10/tauprofile \
workdir/omp_jloop/10000/16/tauprofile \
workdir/omp_jloop/10000/20/tauprofile \
workdir/omp_jloop/100000/1/tauprofile \
workdir/omp_jloop/100000/2/tauprofile \
workdir/omp_jloop/100000/4/tauprofile \
workdir/omp_jloop/100000/8/tauprofile \
workdir/omp_jloop/100000/10/tauprofile \
workdir/omp_jloop/100000/16/tauprofile \
workdir/omp_jloop/100000/20/tauprofile

clean:
	- rm -rf *.o exec/

.PHONY: COMPILE
