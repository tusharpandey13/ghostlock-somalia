# Documentation map

Start with the narrative, then use this map as the reference layer.

## Top level

- [WRITEUP.md](../WRITEUP.md) — the technical story and P0 derivation.
- [TIMELINE.md](../TIMELINE.md) — the complete sanitized operation timeline.
- [STATUS.md](../STATUS.md) — current claims, limits, and redaction policy.
- [README.md](../README.md) — landing page and verification paths.
- [PUBLISHING.md](../PUBLISHING.md) — canonical-source and release checklist.
- [SECURITY.md](../SECURITY.md), [NOTICE](../NOTICE).

## Device

- [overview.md](device/overview.md) — platform, kernel, security state.
- [boot-chain.md](device/boot-chain.md) — EDK2 loader and P0 flow.
- [avb.md](device/avb.md) — verified boot and the persistence boundary.
- [partitions.md](device/partitions.md) — sanitized block map.
- [memory-map.md](device/memory-map.md) — kernel ranges and P0.
- [selinux.md](device/selinux.md) — policy, contexts, hardening notes.
- [kernel-config.md](device/kernel-config.md) — relevant build config.
- [mali.md](device/mali.md) — Mali driver and exposed surfaces.
- [affected.md](device/affected.md) — affected build and claim boundary.
- [matrix.md](device/matrix.md) — comparison with public ports.

## Exploit

- [ghostlock.md](exploit/ghostlock.md) — CVE and chain.
- [kaslr-leak.md](exploit/kaslr-leak.md) — tracefs leak.
- [p0-derivation.md](exploit/p0-derivation.md) — per-boot physical load.
- [pselect-race.md](exploit/pselect-race.md) — route and retry design.
- [cfi.md](exploit/cfi.md) — KCFI and forged fops.
- [cred-patch.md](exploit/cred-patch.md) — what root means.
- [reliability.md](exploit/reliability.md) — run history and limits.

## Lab and incidents

- [lab/qemu.md](lab/qemu.md) — QEMU parity and forced-miss harness.
- [lab/toolchain.md](lab/toolchain.md) — toolchain and hashes.
- [lab/reproduce.md](lab/reproduce.md) — reproduction appendix.
- [incidents/debugfs.md](incidents/debugfs.md) — external abort.
- [incidents/module-put.md](incidents/module-put.md) — binder teardown panic.

## Methodology and evidence

- [methodology/evidence.md](methodology/evidence.md) — tags and sanitization.
- [methodology/safety.md](methodology/safety.md) — safety boundary.
- [methodology/disclosure.md](methodology/disclosure.md) — CVE/vendor status.
- [../reference/](../reference/) — offsets, hashes, glossary.
- [../evidence/](../evidence/) — sanitized excerpts and manifest.
