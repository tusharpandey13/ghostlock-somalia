# Status

Last updated 2026-10-24. This file is the single source of truth for what is
currently true and what is not.

## Current state

- Boot `5e626925...`, slot `_b`, verified boot green.
- P0 `0x801f0000`, derived from this boot's tracefs KASLR leak.
- Rooted and Permissive: the kernel credential patch (`uid 2000 -> 0`) and the
  in-memory `selinux_state.enforcing = 0` write are in effect for this boot.
- Reached with the selective-fd fix (close only pipe and regular fds). This
  build is under observation, not proven stable.
- No cross-reboot persistence. A reboot restores Enforcing, changes P0, and
  drops every root server.

## What is demonstrated

- GhostLock CVE-2026-43499 took a uid-2000 shell to a uid-0 kernel credential
  patch on three separate boots.
- Full capability set, SELinux Permissive for the boot, in memory only.
- A per-boot abstract-socket root server as uid 0 (`u:r:kernel:s0`).
- On-device wireless bootstrap through Termux, no host USB required.
- A derived per-boot physical load (P0) formula, so each boot roots itself
  without a host-side rebuild.
- A three-run history and a public evidence manifest: [evidence/root-runs.md](evidence/root-runs.md), [evidence/MANIFEST.md](evidence/MANIFEST.md).

## What is not demonstrated

- Persistent root. Nothing survives a reboot as root.
- A boot-image change, a bootloader unlock, or any dm-verity bypass.
- A stable on-device `su`/`sudo`. The private-socket path is under observation.
- Cross-device reproducibility. This is one unit.

## Next work

- Keep watching the selective-fd build for a `module_put`-class panic.
- Treat `gl-root` and `su` as under observation until the watch window closes.
- Keep the device off-network for any further exploit window; the OTA freeze is
  not in place.
- Restore `/data/misc/adb/adb_keys` from the 729-byte backup when the Termux
  authorization is no longer needed.
- Do not reboot. Do not read debugfs `clk_summary` or `regulator_summary`. Slot
  A is unbootable.

## Redaction policy

This repository is a sanitized subset of the full record. The following never
appear here: serial numbers, IMEI, MAC addresses, IP addresses and SSIDs,
account identifiers, ADB keys, host paths, operator or tool identifiers, and
any private repository URL. Raw captures, vendor images, and owner-specific
logs are kept out of the public tree. If you find an identifier in a file that
should not be there, report it through the security channel in SECURITY.md
rather than opening a public issue.
