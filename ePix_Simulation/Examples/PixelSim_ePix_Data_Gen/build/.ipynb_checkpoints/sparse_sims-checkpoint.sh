#!/bin/bash
cd ~/SULI2026/ePix_Simulation/Examples/PixelSim_ePix_Data_Gen/build

OUTPUT_DIR="../outputs"
N_RUNS=20

# --- Configurable electron range ---
ELECS_MIN=8000
ELECS_MAX=14000
ELECS_STEP=2000

mkdir -p "${OUTPUT_DIR}"
mkdir -p "./macros"

# Calculate total jobs for display
N_STEPS=$(( (ELECS_MAX - ELECS_MIN) / ELECS_STEP + 1 ))
TOTAL_JOBS=$(( N_STEPS * N_RUNS ))

echo "============================================="
echo " Electron range: ${ELECS_MIN} to ${ELECS_MAX} (step ${ELECS_STEP})"
echo " Runs per step:  ${N_RUNS}"
echo " Total jobs:     ${TOTAL_JOBS}"
echo " Launching all simulation runs in parallel"
echo "============================================="

for ELECS in $(seq ${ELECS_MIN} ${ELECS_STEP} ${ELECS_MAX}); do
    for i in $(seq 1 ${N_RUNS}); do
        RUN_MACRO="./macros/run_${ELECS}_e-_${i}.mac"
        OUTPUT_NAME="sim_${ELECS}_e-_run_$(printf "%03d" ${i})"

        cat > "${RUN_MACRO}" << EOF
/run/initialize
/run/setCut 1 um
/analysis/setFileName ${OUTPUT_DIR}/${OUTPUT_NAME}
/run/beamOn ${ELECS}
EOF

        srun --ntasks=1 --exclusive ./PixelSim "${RUN_MACRO}" &
    done
done

# Wait for ALL background processes to finish
wait

echo "============================================="
echo " All runs complete!"
echo " Root files generated:"
ls ${OUTPUT_DIR}/*.root 2>/dev/null | wc -l
echo "============================================="