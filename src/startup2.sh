#!/bin/bash
# (See https://arc-ts.umich.edu/greatlakes/user-guide/ for command details)

# Set up batch job settings
#SBATCH --job-name=stack
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=36
#SBATCH --exclusive
#SBATCH --time=00:05:00
#SBATCH --account=eecs587f22_class
#SBATCH --partition=standard

{ time ./proj 1000 1000 board_1000.txt 100 wordList_100.txt > proj.stdout ; } 2> proj_time.txt