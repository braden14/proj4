#!/bin/bash

sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=5G --nodes=2 --cpus-per-task=4 shell_script_pthreads.sh