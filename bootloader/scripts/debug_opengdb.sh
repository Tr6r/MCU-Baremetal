#!/usr/bin/env bash
set -euo pipefail

bootloader_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
elf="$bootloader_dir/build/bootloader.elf"
gdb="${GDB:-$HOME/Documents/workspace/tools/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gdb}"
openocd="${OPENOCD:-openocd}"

gdb_args=(
    -ex 'target remote localhost:3333'
    -ex 'monitor reset halt'
)

add_breakpoints() {
    local functions=(
        main
        Default_Handler
    )
    local function_name

    for function_name in "${functions[@]}"; do
        gdb_args+=(-ex "hbreak $function_name")
    done
}

add_breakpoints
gdb_args+=(-ex 'continue')

make -C "$bootloader_dir" OUTPUT_FORMAT=elf
command -v "$gdb" >/dev/null
command -v "$openocd" >/dev/null

openocd_log="$(mktemp)"
openocd_pid=""
cleanup() {
    if [[ -n "$openocd_pid" ]]; then
        kill "$openocd_pid" 2>/dev/null || true
        wait "$openocd_pid" 2>/dev/null || true
    fi
    rm -f "$openocd_log"
}
trap cleanup EXIT

"$openocd" -f interface/stlink.cfg -f target/stm32f1x.cfg >"$openocd_log" 2>&1 &
openocd_pid=$!

ready=0
for ((attempt = 0; attempt < 100; attempt++)); do
    if grep -q 'Listening on port 3333 for gdb connections' "$openocd_log"; then
        ready=1
        break
    fi
    if ! kill -0 "$openocd_pid" 2>/dev/null; then
        cat "$openocd_log" >&2
        exit 1
    fi
    sleep 0.2
done
if [[ "$ready" -ne 1 ]]; then
    cat "$openocd_log" >&2
    exit 1
fi

"$gdb" "$elf" "${gdb_args[@]}"
