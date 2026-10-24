# Device-specific tooling

This directory holds the reusable, sanitized parts of the port:

- `target/` — the somalia target header and kernel-image offsets. These are
  offsets and constants, not a payload.
- `patch/pselect-attempts1.patch` — the accepted one-pselect-attempt-per-process
  change. It avoids the slow in-process refresh; it is not the rejected SIGALRM
  watchdog.
- `termux/` — per-boot `gl-root`, private-socket `su`/`sudo` wrappers.
- `root.sh` — runtime-P0 launcher skeleton.
- `lab/README.md` — QEMU validation notes.

The upstream GhostLock exploit source is not vendored here. Use the public
NebuSec/Apache-2.0 sources and the target/offset material here. No prebuilt
`.so`, APK, firmware, ADB key, or device identifier is included.
