# Redmi A7 Pro 5G (`somalia`)

Public reference for the device used in this work. It describes the stock
platform and the state observed during an owner-authorized audit. It does not
contain a serial number, IMEI, MAC address, IP address, account name, ADB key,
or firmware.

**Result on this unit:** GhostLock (CVE-2026-43499) reached a uid-0 kernel
credential patch and a per-boot Permissive SELinux state from a uid-2000 shell
on several boots. Root is per-boot. A fix for a later teardown panic is under
observation, not proven stable. No cross-reboot persistence is claimed.
[VALIDATED for the root result, UNVALIDATED for the current fix's stability]

## Identity

| Fact | Value | Tag |
|---|---|---|
| Marketing name | Redmi A7 Pro 5G | VALIDATED |
| Codename | `somalia` | VALIDATED |
| Product | `somalia_in` | VALIDATED |
| Model | `26020RN1AI` | VALIDATED |
| SoC | Unisoc T8300 / UMS9632 | INFERRED from product material and firmware strings |
| CPU | 2x Cortex-A78 + 6x Cortex-A55 | VALIDATED by MIDR; part `0xd41` is A78 |
| GPU | Mali-G57 MC2, r0p1, GPU ID `0x09000901` | VALIDATED |
| RAM / storage | 4 GB / 128 GB, retail variant | INFERRED from the product listing and `MemTotal` |
| Android | 16 | VALIDATED |
| Build ID | `BP2A.250605.031.A3` | VALIDATED |
| Vendor build | `OS3.0.13.0.WBWINXM` | VALIDATED |
| Fingerprint | `Redmi/somalia_in/somalia:16/BP2A.250605.031.A3/OS3.0.13.0.WBWINXM:user/release-keys` | VALIDATED |
| Security patch | `2026-08-01` | VALIDATED |
| Build type | `user`, `ro.debuggable=0`, `ro.secure=1`, `adb root` refused | VALIDATED |

## Kernel

| Fact | Value | Tag |
|---|---|---|
| Banner | `6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k` | VALIDATED |
| Source commit in banner | `e56cf6b09cca2151bcee244b3d334fb68685ff57` | VALIDATED |
| Build time | `Thu May 28 07:48:46 UTC 2026` | VALIDATED |
| Compiler | Android clang 18.0.0, LLD 18.0.0, PGO/BOLT/LTO/MLGO | VALIDATED |
| `CONFIG_FUTEX_PI` | `y` | VALIDATED |
| `CONFIG_PROC_KCORE` | not set | VALIDATED |
| KASLR | on, virtual slide random per boot | VALIDATED |
| CFI | KCFI on (`CONFIG_CFI_CLANG=y`) | VALIDATED |

The kernel, its config, and the offset table are covered in
[memory-map.md](memory-map.md), [kernel-config.md](kernel-config.md), and
[../../reference/offsets.md](../../reference/offsets.md).

## SoC topology

- Two Cortex-A78 cores in `policy6`, 614.4 MHz to 2.21 GHz, governor `uscfreq`.
- Six Cortex-A55 cores in `policy0`, 614.4 MHz to 2.041 GHz, same governor.
- The big-core MIDR part is `0xd41`, which is Cortex-A78. An earlier note called
  it A76; that label was wrong. [VALIDATED]
- GPU `23100000.gpu`: Mali-G57 MC2, devfreq states 950, 760, 512, 384 MHz,
  `simple_ondemand`. [VALIDATED]
- Storage is UFS with 4 KiB logical blocks. [VALIDATED]
- The retail unit is the 4 GB RAM variant. `MemTotal` is about 3.5 GiB after
  reservations. [VALIDATED for the observed unit, INFERRED for the SKU]

## Boot and security state

| Fact | Value | Tag |
|---|---|---|
| Bootloader | locked | VALIDATED |
| Verified boot | green | VALIDATED |
| Active slot | `_b` | VALIDATED |
| Slot A | unbootable (`tries_remaining: 0`) | VALIDATED |
| A/B | virtual A/B with VABC | VALIDATED |
| Dynamic partitions | yes (`super`) | VALIDATED |
| dm-verity | 8 read-only erofs volumes | VALIDATED |
| SELinux at boot | Enforcing | VALIDATED |
| SELinux after root | Permissive, in memory, until reboot | VALIDATED |
| `kptr_restrict` | 2 | VALIDATED |
| Debugfs | present, not mounted by default | VALIDATED |

Slot B is the only bootable slot. Do not write `boot_b`, `init_boot_b`,
`vendor_boot_b`, `vbmeta_b`, or any `_b` dynamic partition. The `misc`
partition also held a pending one-shot `bootonce-bootloader` intent, so a reboot
can land in the bootloader rather than Android. [VALIDATED]

See [boot-chain.md](boot-chain.md), [avb.md](avb.md), and
[partitions.md](partitions.md) for the details.

## What this device is not

- No unlock, flash, or boot-image write was performed.
- The audit did not extract or publish vendor signing keys.
- The observed root is a credential patch plus an in-memory SELinux byte write.
  It does not mount a root filesystem, install `su`, or survive a reboot.
- The current build that re-roots the unit after a teardown panic is still under
  observation. Treat a claim of stability as open.

## Related pages

- [Boot chain](boot-chain.md)
- [AVB and verified boot](avb.md)
- [Partitions](partitions.md)
- [Memory map](memory-map.md)
- [SELinux](selinux.md)
- [Kernel config](kernel-config.md)
- [Mali](mali.md)
- [Offsets](../../reference/offsets.md)
- [Hashes](../../reference/hashes.md)
- [Glossary](../../reference/glossary.md)
- [Sanitized recon excerpts](../../evidence/recon.md)
