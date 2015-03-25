#!/bin/sh

# Find the executable
ANISE_DIR=$(readlink -f $0)
SCRIPT_PATH=${ANISE_DIR%/*}

# Adjust the library path
if [ -n "$LD_LIBRARY_PATH" ]; then
    LD_LIBRARY_PATH=$SCRIPT_PATH/nodes/extras:$LD_LIBRARY_PATH
else
    LD_LIBRARY_PATH=$SCRIPT_PATH/nodes/extras
fi
export LD_LIBRARY_PATH

# Execute the framework from its original folder
exec $SCRIPT_PATH/anise.bin "$@"
