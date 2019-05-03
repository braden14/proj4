#!/bin/bash -l

#SBATCH --mem-per-cpu=5G
#SBATCH --time=5:00:00
#SBATCH --partition=killable.q
#SBATCH --constraint=elves
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=3way-mpi

module load OpenMPI
mpirun /homes/nils/cis520/proj4/3way-mpi/mpi_out