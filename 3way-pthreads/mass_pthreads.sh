#!/bin/bash


sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=5G --nodes=2 --cpus-per-task=1 shell_script_pthreads.sh
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=5G --nodes=2 --cpus-per-task=2 shell_script_pthreads.sh
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=5G --nodes=2 --cpus-per-task=4 shell_script_pthreads.sh
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=5G --nodes=2 --cpus-per-task=8 shell_script_pthreads.sh
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=5G --nodes=2 --cpus-per-task=16 shell_script_pthreads.sh