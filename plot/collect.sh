#!/bin/bash

# Save the initial directory
START_DIR=$(pwd)

# Ensure the logs directory exists
mkdir -p "$START_DIR/logs"

# Recursive function to process directories
process_directory() {
    local parent_dir=$1
    local parent_name=$2  # Pass the name of the parent directory
    local depth=$3        # Directory depth

    # Loop through subdirectories
    for subdir in "$parent_dir"/*; do
        if [[ -d "$subdir" ]]; then
            local subdir_name=$(basename "$subdir")
            if [[ $depth -eq 1 ]]; then
                # First level directory, pass the full directory name to the next level
                process_directory "$subdir" "$subdir_name" 2
            elif [[ $depth -eq 2 ]]; then
                # Second level directory, extract the numeric suffix
                local num_suffix=$(echo "$subdir_name" | grep -o -E '[0-9]+$')
                if [[ -n $num_suffix ]]; then
                    # Enter directory and process files
                    pushd "$subdir" > /dev/null
                    # Process .gz files
                    for gzfile in PeriodicPower.log.gz PeriodicThermal.log.gz; do
                        if [[ -f "$gzfile" ]]; then
                            local base=${gzfile%%.*}  # Extract the base name, e.g., from PeriodicPower.log.gz to PeriodicPower
                            gzip -cd "$gzfile" > "$START_DIR/logs/${base}_${parent_name}_${num_suffix}.log"
                        fi
                    done
                    popd > /dev/null
                fi
            fi
        fi
    done
}

# Start processing from the current directory, with an initial depth of 1
process_directory "$(pwd)" "" 1

echo "Processing complete! All relevant files have been moved to $START_DIR/logs directory."
