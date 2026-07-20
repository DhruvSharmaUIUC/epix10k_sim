#!/bin/bash

#SBATCH --account=lcls:default  
#SBATCH --partition=milano
#SBATCH --qos=preemptable
#SBATCH --constraint=OS_VER:8.6
#
#SBATCH --job-name=ePix_Simulations
#SBATCH --output=ePix_Simulations-%j.out
#SBATCH --error=ePix_Simulations-%j.txt
#
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=6g
#
#SBATCH --time=0-00:30:00
#
./sparse_sims.sh