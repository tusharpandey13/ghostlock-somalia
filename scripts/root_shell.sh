#!/bin/sh
# Host wrapper for the GhostLock persistent root channel.
#
#   ./scripts/root_shell.sh [port]        # interactive
#   ROOT_SERIAL=... ROOT_PORT=... ./scripts/root_shell.sh
#
# Requires the device root server (abstract socket "ghostlock-root") to be
# alive on the current boot. No disk state is read or written on the device;
# the socket is kernel-only.
#
# For scripted one-shot commands use ./scripts/root_exec.py instead: piping a
# command list straight into nc can race (nc exits at stdin EOF before the
# remote shell replies).
set -eu

SERIAL=${ROOT_SERIAL:?set ROOT_SERIAL to the device serial}
PORT=${1:-${ROOT_PORT:-12346}}

adb -s "$SERIAL" forward "tcp:$PORT" localabstract:ghostlock-root >/dev/null
exec nc 127.0.0.1 "$PORT"
