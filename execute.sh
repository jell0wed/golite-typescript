#!/bin/bash

function printUsage() {
    cat<<EOF
Execute TypeScript files

Usage: 
    execute.sh [FILE]
EOF
    
}

function main() {

    if [[ ${#@} -ne 1 ]]
    then
        printUsage
    else

        # Input and output files
        IN_FILE="$1"
        OUT_FILE="${IN_FILE%.*}.js"

        # Compile TypeScript
        tsc \
            ./etc/typescript/stubs.ts \
            "$IN_FILE"

        if [[ $? -ne 0 ]]
        then
            echo "Error compiling ..."
        else

            # Run code
            node "$OUT_FILE"
        fi
    fi
}

main $1
