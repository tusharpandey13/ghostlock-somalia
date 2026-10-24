# Partitions

Sanitized block map for the `_b` boot. It gives names, sizes, and roles. It does
not include partition UUIDs, serial numbers, or raw partition content.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Layout

- GPT disk `sda`: 61,176 MiB, 90 named entries. [VALIDATED]
- `sdb` and `sdc`: 4 MiB each, the `spl_a` and `spl_b` boot ROM / SPL slots.
  They are separate block devices, not `sda` partitions. [VALIDATED]
- Active slot: `_b`. Slot A is not bootable (`tries_remaining: 0`). [VALIDATED]

The 90 named entries include both A and B copies for most firmware partitions,
the dynamic `super` partition, and the state partitions. The count is from
`/dev/block/by-name` as root. The GPT table and the by-name directory agree on
the 90 names that were captured.

## Key partitions

Sizes are in MiB unless noted.

| Partition | Device | Size | Role | Tag |
|---|---|---|---|---|
| `prodnv` | `sda1` | 63 | writable ext4 vendor NV mount; holds `persist` subdirectory | VALIDATED |
| `misc` | `sda4` | 1 | bootloader control block; held `bootonce-bootloader` | VALIDATED |
| `trustos_a` / `trustos_b` | `sda7` / `sda8` | 6 each | TrustZone OS, `DHTB` container, `0x43d000`-byte payload (about 4.24 MiB) | VALIDATED |
| `uboot_a` / `uboot_b` | `sda11` / `sda12` | 8 each | EDK2 firmware volume | VALIDATED |
| `uboot_log` | `sda13` | 16 | bootloader log, historical `Start linux at PC` lines | VALIDATED |
| `teecfg_a` / `teecfg_b` | `sda40` / `sda41` | 1 each | TEE configuration, `DHTB`, 120 KiB payload | VALIDATED |
| `boot_a` / `boot_b` | `sda44` / `sda45` | 100 each | Android boot image, kernel only | VALIDATED |
| `vendor_boot_a` / `vendor_boot_b` | `sda46` / `sda47` | 100 each | vendor boot, ramdisk plus DTB | VALIDATED |
| `init_boot_a` / `init_boot_b` | `sda48` / `sda49` | 8 each | init ramdisk, no kernel | VALIDATED |
| `super` | `sda54` | 9,728 | dynamic partition container | VALIDATED |
| `vbmeta_a` / `vbmeta_b` | `sda58` / `sda59` | 2 each | top-level AVB metadata | VALIDATED |
| `metadata` | `sda60` | 64 | f2fs metadata, OTA and boot state | VALIDATED |
| `userdata` | `sda90` | 49,225 | f2fs user data, `dm-75` on this boot | VALIDATED |
| `persist` | `sda25` | 32 | raw persist partition, not mounted | VALIDATED |
| `rescue` | `sda85` | 128 | writable ext4 rescue partition | VALIDATED |
| `blackbox` | `sda56` | 256 | writable f2fs logs | VALIDATED |
| `cache` | `sda55` | 64 | writable f2fs cache | VALIDATED |

`trustos` and `teecfg` are `DHTB` version 1 images with a 32-byte digest, marker
words, and size fields. Their payloads start at `0x1000` and read as opaque, not
as ELF. Only headers were examined. [VALIDATED for headers, BLOCKED for payload
semantics]

## Boot image payload sizes

| Partition | Kernel bytes | Ramdisk bytes | Notes |
|---|---|---|---|
| `boot_a` | 36,596,224 (`0x22e6a00`) | 0 | PE/EFI stub at `0x1000` |
| `boot_b` | 36,801,024 (`0x2318a00`) | 0 | active slot |
| `init_boot_a` | 0 | 2,665,713 | LZ4 ramdisk |
| `init_boot_b` | 0 | 2,665,832 | active slot |
| `vendor_boot_a` | 0 | 29,589,842 | page 4096, ramdisk address `0x05400000` |
| `vendor_boot_b` | 0 | 29,591,121 | active slot, same ramdisk address |

Both ramdisks start with the LZ4 legacy magic `0x184c2102`. The boot images are
Android header v4; the vendor images are vendor header v4. [VALIDATED]

## Dynamic partitions

The `_b` dynamic partitions appear as device-mapper nodes:

```text
dm-0  odm_b
dm-1  product_b
dm-2  system_b
dm-3  system_dlkm_b
dm-4  system_ext_b
dm-5  vendor_b
dm-6  vendor_dlkm_b
dm-7  mi_ext_b
```

`dm-8` through `dm-15` are the `*-cow` snapshot devices. `dm-75` is the active
`userdata` mapping. The exact `dm-*` numbers can change with boot state; the
names in `/dev/block/mapper` are the stable identifiers. [VALIDATED]

## Slot B caveat

Slot B is the only working Android boot path on this unit. Do not write any of
these:

- `boot_b`
- `init_boot_b`
- `vendor_boot_b`
- `vbmeta_b`
- any `_b` dynamic partition
- `misc` while the `bootonce-bootloader` intent is present

A bad write to any of them can remove the last bootable slot. The device also
has a pending one-shot bootloader command in `misc`, so a reboot may land in the
bootloader instead of Android. Clearing that intent is a write and is outside
the read-only posture used here. [VALIDATED]

## Writable state versus verified system

Read-only, dm-verity protected: `system`, `system_ext`, `vendor`, `odm`,
`product`, `mi_ext`, `vendor_dlkm`, `system_dlkm`.

Writable: `/data` (`userdata`), `/metadata`, `/cache`, `/blackbox`,
`/mnt/vendor` (`prodnv`), `/mnt/rescue`, and the subdirectories under them.

The kernel exploit writes only kernel memory. It does not touch any of these
partitions. The one persistent userspace change from the later boot work was an
ADB public-key append under `/data/misc/adb`, with a backup, and that file is
not reproduced here. [VALIDATED]

## Related pages

- [Device overview](overview.md)
- [Boot chain](boot-chain.md)
- [AVB and verified boot](avb.md)
- [Memory map](memory-map.md)
