#!/bin/bash
echo 'Executing Draw Gau Code'
# Setting the x flag will echo every
# command onto stderr. This is
# for debugging, so we can see what's
# going on.
set -x
echo ==ENV=============
# The env command prints out the
# entire execution environment. This
# is also present for debugging purposes.
env
echo ==PWD=============
# We also print out the execution
# directory. Again, for debugging purposes.
pwd
echo ==JOB=============
EXE_DIR=/home/ktraxler/drawgau
# ${parfile} will be substituted by Agave
# I like having my environment variables in
# upper case, that's the only reason for this
# assignment.
FAIL=no
if [ "${parfile}" = "" ]
then
  echo "The parfile was not set"
  # Note: Job failure must be explicitly reported to
  # the Agave framework, otherwise it might
  # mistakenly believe the job is still running.
  
  exit 1
fi
if [ "${PBS_NODEFILE}" = "" ]
then
  # When running on a system managed by Torque
  # this variable should be set. If it's not,
  # that's a problem.
  echo "The PBS_NODEFILE was not set"
  
  exit 2
fi

# By default, the PBS_NODEFILE lists nodes multiple
# times, once for each MPI process that should run
# there. We only want one MPI process per node, so
# we create a new file with "sort -u".
LOCAL_NODEFILE=nodefile.txt
sort -u < ${PBS_NODEFILE} > ${LOCAL_NODEFILE}
PROCS=$(wc -l < ${LOCAL_NODEFILE})

if [ "${PROCS}" = "" ]
then
  echo "PROCS was not set"
  $(${AGAVE_JOB_CALLBACK_FAILURE})
  exit 3
fi

# Execute our MPI command.
mpirun -np ${PROCS} -machinefile ${LOCAL_NODEFILE} ${EXE_DIR}/drawgau ${parfile}