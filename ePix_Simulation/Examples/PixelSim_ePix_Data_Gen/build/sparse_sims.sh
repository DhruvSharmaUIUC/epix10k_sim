#!/bin/bash
cd ~/SULI2026/ePix_Simulation/Examples/PixelSim_ePix/build

OUTPUT_DIR="../outputs"
N_RUNS=1
ELECS_PER_RUN=1000

mkdir -p "${OUTPUT_DIR}"
mkdir -p "./macros"

echo "============================================="
echo " Launching ${N_RUNS} simulation runs in parallel"
echo "============================================="

for i in $(seq 1 ${N_RUNS}); do
    RUN_MACRO="./macros/run_${i}.mac"

    cat > "${RUN_MACRO}" << EOF
/run/initialize
/run/setCut 1 um
/analysis/setFileName ${OUTPUT_DIR}/sparse_sim_run_$(printf "%03d" ${i})
/run/beamOn ${ELECS_PER_RUN}
EOF

    # Launch each simulation in parallel via srun
    srun --ntasks=1 --exclusive ./PixelSim "${RUN_MACRO}" &
done

# Wait for ALL background processes to finish
wait

echo "============================================="
echo " All runs complete!"
ls ${OUTPUT_DIR}/*.root 2>/dev/null | wc -l
echo " root files generated."
echo "============================================="