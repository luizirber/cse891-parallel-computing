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

COMPILE_INTEL:
	mkdir -p exec/
	module unload $(ALL_MODULES) && \
	module load Intel && \
	module load TAU && \
	tau_cc.sh -optCompInst -optTrackGOMP -fopenmp -std=c99 -o ${EXEC_TARGET} -DVECTOR_SIZE=${NSIZE} ${EXEC_SRC}

SUBMIT_THREADS:
	echo "mkdir -p ${WORKDIR}/\$$p/; cp exec/${EXECUTABLE} ${WORKDIR}/\$$p/; cd ${WORKDIR}/\$$p/; module unload ${ALL_MODULES}; module load GNU; module load TAU; export OMP_NUM_THREADS=\$$p ; ./${EXECUTABLE};" | cat pbs/header-threads.sub - pbs/footer-threads.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:${WTIME_MINUTES}:00,nodes=1:ppn=20:mics=2,feature=phi:gbe -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

SUBMIT_THREADS_INTEL:
	echo "mkdir -p ${WORKDIR}/\$$p/; cp exec/${EXECUTABLE} ${WORKDIR}/\$$p/; cd ${WORKDIR}/\$$p/; module unload ${ALL_MODULES}; module load Intel; module load TAU; export OMP_NUM_THREADS=\$$p ; ./${EXECUTABLE};" | cat pbs/header-threads-intel.sub - pbs/footer-threads.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:${WTIME_MINUTES}:00,nodes=1:ppn=20:mics=2,feature=phi:gbe -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

SUBMIT_SERIAL:
	echo "cd ${WORKDIR}/1; module unload ${ALL_MODULES}; module load ${TMODULES}; module load TAU; ./${EXECUTABLE};" | cat pbs/header-serial.sub - pbs/footer-serial.sub | tee ${WORKDIR}/script.sub | \
    qsub -l walltime=00:${WTIME_MINUTES}:00,nodes=1:ppn=20 -N ${EXECUTABLE} -o ${WORKDIR}/${EXECUTABLE}.pbs -e ${WORKDIR}/${EXECUTABLE}.err

################
#
# OpenMP i loop
#
################

exec/omp_iloop-GNU: $(addprefix exec/omp_iloop-GNU-,$(VECTOR_SIZE))

workdir/omp_iloop.pbs: $(foreach v,$(VECTOR_SIZE),\
                         $(subst VSIZE,$v,workdir/omp_iloop/VSIZE/omp_iloop-GNU-VSIZE.pbs))

exec/omp_iloop-GNU-%: export TMODULES = GNU TAU
exec/omp_iloop-GNU-%: src/omp_iloop.c
	$(MAKE) NSIZE=$* EXEC_SRC=$< EXEC_TARGET=exec/omp_iloop-GNU-$* COMPILE

workdir/omp_iloop/%.pbs: export TMODULES = GNU TAU
workdir/omp_iloop/%.pbs: export WTIME_MINUTES=05
workdir/omp_iloop/%.pbs: export WORKDIR_BASE=workdir/omp_iloop
workdir/omp_iloop/%.pbs:
	export NSIZE=`echo $(*) | cut -d "/" -f1` ; \
	mkdir -p ${WORKDIR_BASE}/$${NSIZE} ; \
	$(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$${NSIZE} EXECUTABLE=omp_iloop-GNU-$${NSIZE} SUBMIT_THREADS

################
#
# OpenMP j loop
#
################

exec/omp_jloop-GNU: $(addprefix exec/omp_jloop-GNU-,$(VECTOR_SIZE))

workdir/omp_jloop.pbs: $(foreach v,$(VECTOR_SIZE),\
                         $(subst VSIZE,$v,workdir/omp_jloop/VSIZE/omp_jloop-GNU-VSIZE.pbs))

exec/omp_jloop-GNU-%: export TMODULES = GNU TAU
exec/omp_jloop-GNU-%: src/omp_jloop.c
	$(MAKE) NSIZE=$* EXEC_SRC=$< EXEC_TARGET=exec/omp_jloop-GNU-$* COMPILE

workdir/omp_jloop/%.pbs: export TMODULES = GNU TAU
workdir/omp_jloop/%.pbs: export WTIME_MINUTES=15
workdir/omp_jloop/%.pbs: export WORKDIR_BASE=workdir/omp_jloop
workdir/omp_jloop/%.pbs:
	export NSIZE=`echo $(*) | cut -d "/" -f1` ; \
	mkdir -p ${WORKDIR_BASE}/$${NSIZE} ; \
	$(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$${NSIZE} EXECUTABLE=omp_jloop-GNU-$${NSIZE} SUBMIT_THREADS

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

workdir/qsort/%.pbs: export TMODULES = GNU
workdir/qsort/%.pbs: export WTIME_MINUTES=05
workdir/qsort/%.pbs: export WORKDIR_BASE=workdir/qsort
workdir/qsort/%.pbs:
	export NSIZE=`echo $(*) | cut -d "/" -f1` ; \
	mkdir -p ${WORKDIR_BASE}/$${NSIZE}/1 ; \
	cp exec/qsort-GNU-$${NSIZE} ${WORKDIR_BASE}/$${NSIZE}/1 ; \
	$(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$${NSIZE} EXECUTABLE=qsort-GNU-$${NSIZE} SUBMIT_SERIAL

#############
#
# MIC i loop
#
#############

exec/mic_iloop-Intel: $(addprefix exec/mic_iloop-Intel-,$(VECTOR_SIZE))

workdir/mic_iloop.pbs: $(foreach v,$(VECTOR_SIZE),\
                         $(subst VSIZE,$v,workdir/mic_iloop/VSIZE/mic_iloop-Intel-VSIZE.pbs))

exec/mic_iloop-Intel-%: export TMODULES = Intel TAU
exec/mic_iloop-Intel-%: src/mic_iloop.c
	$(MAKE) NSIZE=$* EXEC_SRC=$< EXEC_TARGET=exec/mic_iloop-Intel-$* COMPILE_INTEL

workdir/mic_iloop/%.pbs: export TMODULES = Intel TAU
workdir/mic_iloop/%.pbs: export WTIME_MINUTES=05
workdir/mic_iloop/%.pbs: export WORKDIR_BASE=workdir/mic_iloop
workdir/mic_iloop/%.pbs:
	export NSIZE=`echo $(*) | cut -d "/" -f1` ; \
	mkdir -p ${WORKDIR_BASE}/$${NSIZE} ; \
	$(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$${NSIZE} EXECUTABLE=mic_iloop-Intel-$${NSIZE} SUBMIT_THREADS_INTEL

############
#
# MIC qsort
#
############

exec/mic_qsort-Intel: $(addprefix exec/mic_qsort-Intel-,$(VECTOR_SIZE))

workdir/mic_qsort.pbs: $(foreach v,$(VECTOR_SIZE),\
                     $(subst VSIZE,$v,workdir/mic_qsort/VSIZE/mic_qsort-Intel-VSIZE.pbs))

exec/mic_qsort-Intel-%: export TMODULES = Intel
exec/mic_qsort-Intel-%: export EXEC_SRC = src/mic_qsort.c
exec/mic_qsort-Intel-%: src/mic_qsort.c
	$(MAKE) NSIZE=$* EXEC_TARGET=exec/mic_qsort-Intel-$* COMPILE_INTEL

workdir/mic_qsort/%.pbs: export TMODULES = Intel
workdir/mic_qsort/%.pbs: export WTIME_MINUTES=05
workdir/mic_qsort/%.pbs: export WORKDIR_BASE=workdir/mic_qsort
workdir/mic_qsort/%.pbs:
	export NSIZE=`echo $(*) | cut -d "/" -f1` ; \
	mkdir -p ${WORKDIR_BASE}/$${NSIZE}/1 ; \
	cp exec/mic_qsort-Intel-$${NSIZE} ${WORKDIR_BASE}/$${NSIZE}/1 ; \
	$(MAKE) WTIME_MINUTES=${WTIME_MINUTES} WORKDIR=${WORKDIR_BASE}/$${NSIZE} EXECUTABLE=mic_qsort-Intel-$${NSIZE} SUBMIT_SERIAL

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
	- rm -rf *.o

.PHONY: COMPILE
.SUFFIXES:
