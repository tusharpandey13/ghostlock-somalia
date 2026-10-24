# AVB and verified boot

The unit ships with Android Verified Boot enabled, a locked bootloader, and
green verified-boot state. AVB and dm-verity are the main reason root cannot
persist across a reboot.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Runtime state

| Property | Value | Tag |
|---|---|---|
| `ro.boot.verifiedbootstate` | `green` | VALIDATED |
| `ro.boot.flash.locked` / `flash.locked` | `1` | VALIDATED |
| `ro.boot.vbmeta.device_state` | `locked` | VALIDATED |
| `ro.boot.veritymode` | `enforcing` | VALIDATED |
| `ro.boot.secureboot` | `1` | VALIDATED |
| `ro.boot.avb_version` | `1.3` | VALIDATED |
| `ro.boot.vbmeta.digest` | `102262d5206025d16280da9ab8ab3b12f5d3ee016737089b6c28b45b6188c8c1` | VALIDATED |
| `ro.boot.vbmeta.size` | `52288` | VALIDATED |
| `fastboot getvar unlocked` | `no` | VALIDATED |
| `fastboot getvar fuse` | `yes` | VALIDATED |
| `fastboot getvar anti` | `1` | VALIDATED |

The `vbmeta` digest matches the aggregate digest over the 15 AVB chains on this
image. The `52288` size is the vbmeta chain size reported for that digest.

`adb root` is refused because the build is a user build (`ro.debuggable=0`), so
AVB is not being bypassed by a debuggable daemon. [VALIDATED]

## Header facts

| Fact | Value | Tag |
|---|---|---|
| Magic | `AVB0` | VALIDATED |
| Header version | 1.1 | VALIDATED |
| libavb version | 1.0 | VALIDATED |
| avbtool | 1.3.0 | VALIDATED |
| Algorithm | type 2, `SHA256_RSA4096` | VALIDATED |
| Authentication block | 576 bytes | VALIDATED |
| Auxiliary block | 18,240 bytes | VALIDATED |
| Public key | 1,032 bytes at aux offset `0x4318` | VALIDATED |
| Descriptors | start at aux offset 0, length `0x4318` | VALIDATED |

The public key is described by size and offset only. The key material itself is
not reproduced here and is not redistributed. No vendor signing keys are in this
repository. [VALIDATED]

## Chained partitions and rollback indices

The top-level vbmeta chains to 14 partitions with the following rollback
indices. [VALIDATED]

| Partition | Rollback index |
|---|---|
| `boot` | 1 |
| `vbmeta_system` | 2 |
| `vbmeta_system_ext` | 3 |
| `vbmeta_vendor` | 4 |
| `vbmeta_product` | 5 |
| `dtbo` | 6 |
| `vbmeta_odm` | 7 |
| `init_boot` | 8 |
| `nr_modem` | 11 |
| `nr_phy` | 12 |
| `l_agdsp` | 13 |
| `pm_sys` | 14 |
| `ch_sys` | 15 |
| `vendor_boot` | 17 |

The indices are not contiguous. A rollback counter for a partition that is not
listed here is not present in the top-level chain that was parsed. Only the
top-level descriptors were parsed; the per-partition hash descriptors inside
each chained vbmeta were not enumerated in this pass. [VALIDATED for the
top-level chain, UNVALIDATED for deeper descriptors]

## Embedded security patch levels

The vbmeta properties are build-time snapshots and are older than the running
system in places. [VALIDATED values, INFERRED interpretation]

| Property | Value |
|---|---|
| `boot.security_patch` | `2024-08-05` |
| `system.security_patch` | `2026-02-01` |
| `vendor.security_patch` | `2026-02-01` |
| `mi_ext.security_patch` | `2026-05-01` |
| Running system security patch | `2026-08-01` |

The `mi_ext` fingerprint inside vbmeta names `OS3.0.10.0.WBWINXM`, while the
running vendor build is `OS3.0.13.0.WBWINXM`. The vbmeta string is a snapshot
from an older build. It does not mean the running system is the older build.

## dm-verity

Eight read-only erofs volumes are mounted through dm-verity. [VALIDATED]

```text
system  system_ext  vendor  odm  product  mi_ext  vendor_dlkm  system_dlkm
```

Each is mounted read-only. The writable state is on `/data`, `/metadata`,
`/cache`, `/blackbox`, `/mnt/vendor`, and `/mnt/rescue`. A root payload that
writes only kernel memory avoids all of them. A persistent root file or service
would need a writable system partition and would break verity, which the locked
boot state does not allow.

## Slot state

Slot A is marked `tries_remaining: 0` and `verity_corrupted: 0`. The bootloader
selects slot B. [VALIDATED]

That changes the recovery model:

- There is no good A slot to fall back to.
- A damaged `boot_b`, `init_boot_b`, `vendor_boot_b`, `vbmeta_b`, or `_b`
  dynamic partition removes the last Android boot path.
- The `misc` partition held a `bootonce-bootloader` intent plus the `BCAB`
  bootloader control block. A reboot can enter the bootloader instead of
  Android. [VALIDATED]

No partition was written during this work. The device stayed locked and green
through the root runs and both recorded panics.

## Consequences for root

AVB does not stop an in-memory kernel credential patch. It does stop every
persistence path that would normally follow one:

- No modified boot image.
- No custom `su` in a system partition.
- No Magisk or KernelSU boot patch.
- No verified-boot bypass.

Root therefore lasts for one boot. A reboot restores Enforcing SELinux, changes
P0, and drops any root server. [VALIDATED]

See [boot-chain.md](boot-chain.md) and [partitions.md](partitions.md).
