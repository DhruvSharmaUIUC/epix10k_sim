#!/bin/bash
# generate_training_set.sh
# Generates ~50 Geant4 simulation output files with different random seeds

cd ~/ePix_Simulation/Examples/PixelSim_ePix/build

OUTPUT_DIR="../outputs"
N_RUNS=2
ELECS_PER_RUN=1000

mkdir -p "${OUTPUT_DIR}"
mkdir -p "./macros"

echo "============================================="
echo " Generating ${N_RUNS} simulation runs"
echo " Output directory: ${OUTPUT_DIR}"
echo "============================================="

for i in $(seq 1 ${N_RUNS}); do
    
    RUN_MACRO="./macros/run_${i}.mac"
    
    # Generate a Geant4 macro for this run
    cat > "${RUN_MACRO}" << EOF
/run/initialize
/run/setCut 1 um
/analysis/setFileName ${OUTPUT_DIR}/sparse_sim_run_$(printf "%03d" ${i})
/run/beamOn ${ELECS_PER_RUN}
EOF
    echo "[Run ${i}/${N_RUNS}]"
    ./PixelSim "${RUN_MACRO}"
    if [ -f "${OUTPUT_DIR}/sparse_sim_run_$(printf "%03d" ${i}).root" ]; then
        echo "  ✓ Output generated successfully"
    else
        echo "  ✗ ERROR: Output file not found!"
    fi
done


echo ""
echo "============================================="
echo " Generation complete!"
echo " Files in ${OUTPUT_DIR}/:"
ls -la ${OUTPUT_DIR}/*.root 2>/dev/null | wc -l
echo " root files generated."
echo "============================================="