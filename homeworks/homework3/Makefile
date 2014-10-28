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

SUBMIT_THREADS:
	echo "cd ${WORKDIR}/\$$p; module unload ${ALL_MODULES}; module load GNU; module load TAU; export OMP_NUM_THREADS=\$$p ; ./${EXECUTABLE};" | cat pbs/header-threads.sub - pbs/footer-threads.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:${WTIME_MINUTES}:00,nodes=1:ppn=20:mics=2,feature=phi -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

SUBMIT_SERIAL:
	echo "cd ${WORKDIR}; module unload ${ALL_MODULES}; module load GNU; module load TAU; ./${EXECUTABLE};" | cat pbs/header-serial.sub - pbs/footer-serial.sub | tee ${WORKDIR}/script.sub | \
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
      $(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_THREADS; \
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
      $(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$$n EXECUTABLE=${EXEC_BASE}-$$n SUBMIT_THREADS; \
    done

###############
#
# stdlib qsort
#
###############

exec/qsort-GNU: $(addprefix exec/qsort-GNU-,$(VECTOR_SIZE))

workdir/qsort.pbs: $(foreach v,$(VECTOR_SIZE),\
                     $(subst VSIZE,$v,workdir/qsort/VSIZE/qsort-GNU-VSIZE.pbs))

exec/qsort-GNU-%: export TMODULES = GNU TAU
exec/qsort-GNU-%: export EXEC_SRC = src/qsort.c
exec/qsort-GNU-%: src/qsort.c
	$(MAKE) NSIZE=$* EXEC_TARGET=exec/qsort-GNU-$* COMPILE

workdir/qsort/%/qsort-GNU-%.pbs: export TMODULES = GNU TAU
workdir/qsort/%/qsort-GNU-%.pbs: export WTIME_MINUTES=5
workdir/qsort/%/qsort-GNU-%.pbs: export WORKDIR_BASE=workdir/qsort
workdir/qsort/%/qsort-GNU-%.pbs: exec/qsort-GNU-%
	mkdir -p ${WORKDIR_BASE}/$*
	cp $< ${WORKDIR_BASE}/$*
	$(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$* EXECUTABLE=$< SUBMIT_SERIAL

################
#
# Data analysis
#
################

workdir/%/tauprofile: workdir/%/profile.0.0.0
	module load TAU; \
	cd $(@D); \
	pprof > tauprofile

TAU_PROFILES_ILOOP: $(foreach v,$(VECTOR_SIZE),\
                      $(subst VSIZE,$v,$(foreach t,$(THREADS),\
                        $(subst THREAD,$t,workdir/omp_iloop/VSIZE/THREAD/tauprofile))))

TAU_PROFILES_JLOOP: $(foreach v,$(VECTOR_SIZE),\
                      $(foreach t,$(THREADS),\
                        $(subst VSIZE,$v,$(subst THREAD,$t,workdir/omp_jloop/VSIZE/THREAD/tauprofile))))

TAU_PROFILES_QSORT: $(foreach v,$(VECTOR_SIZE),\
                      $(subst VSIZE,$v,workdir/qsort/VSIZE/1/tauprofile))

clean:
	- rm -rf *.o exec/

.PHONY: COMPILE
