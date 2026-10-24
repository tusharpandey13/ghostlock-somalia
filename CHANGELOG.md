# Changelog

## v1.0 — 2026-10-24

Initial public research release.

- Device-specific GhostLock port for the Redmi A7 Pro 5G (Unisoc T8300).
- Per-boot P0 derivation from the tracefs KASLR slide.
- Sanitized root-run evidence for the first root, persistent-shell run, and
  selective-fd re-root.
- QEMU parity notes, CFI reasoning, and the accepted one-attempt-per-process
  pselect patch.
- Incident records for the debugfs external abort and the binder
  `module_put`/`__fput` panic.
- Public redaction policy, evidence tags, licenses, and upstream credits.

Status at release: root demonstrated per boot on this unit; the selective-fd
build is under observation; no cross-reboot persistence.
