# Documentation index

Read the narrative first: [WRITEUP.md](../WRITEUP.md). This index maps the
reference pages underneath.

## Device

- [overview.md](device/overview.md): the platform, kernel, and security state.
- [boot-chain.md](device/boot-chain.md): the Unisoc EDK2 loader and the P0 flow.
- [avb.md](device/avb.md): verified boot and why root cannot persist.
- [partitions.md](device/partitions.md): the sanitized block map.

## Exploit

- [ghostlock.md](exploit/ghostlock.md): CVE-2026-43499 and the chain on this
  device.
- [kaslr-leak.md](exploit/kaslr-leak.md): the tracefs `_text` leak.
- [p0-derivation.md](exploit/p0-derivation.md): the per-boot physical load from
  the slide.

## Lab and incidents

- [lab/](lab/): QEMU parity and KCFI notes.
- [incidents/](incidents/): the debugfs panic and the binder `module_put`
  panic.

## Methodology and evidence

- [methodology/](methodology/): evidence tags and safety gates.
- [../reference/](../reference/): offsets, hashes, glossary.
- [../evidence/](../evidence/): sanitized excerpts only.

## Top level

- [../WRITEUP.md](../WRITEUP.md), [../TIMELINE.md](../TIMELINE.md),
  [../STATUS.md](../STATUS.md), [../README.md](../README.md),
  [../SECURITY.md](../SECURITY.md), [../NOTICE](../NOTICE).
