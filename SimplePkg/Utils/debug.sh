#!/bin/bash

SESSION_NAME='ovmf_debug'

declare -a COMMANDS=(
'/opt/intel/udkdebugger/bin/udk-gdb-server'
'./run.sh'
'sleep 1; x86_64-w64-mingw32-gdb -ex "target remote :1234" -x /opt/intel/udkdebugger/script/udk_gdb_script -ex "continue"'
)

tmux new-session -d -s "${SESSION_NAME}" "${COMMANDS[0]}"

for command in "${COMMANDS[@]:1}"; do
    tmux new-window -d "$command"
done

tmux select-window -t 2
tmux attach-session -t "${SESSION_NAME}"