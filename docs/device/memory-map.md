# Memory map

Physical view from a rooted boot, plus the KASLR rules that make a fixed address
useless. The ranges below are tied to one recorded boot (`P0 = 0x80828000`) and
are shown as offsets from P0. A later boot has a different P0 and a different
virtual slide.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Physical RAM

`/proc/iomem` is readable as root. [VALIDATED]

```text
80000000-fff6ffff : System RAM
fff80000-17fffefff : System RAM
```

The DRAM base is `0x80000000`. The two System RAM ranges cover about 4 GiB with a
small hole near `0xfff70000`. The device is the 4 GB variant, so the high end of
the second range is the top of usable RAM. [VALIDATED for the map, INFERRED for
the SKU capacity]

## Kernel image ranges, relative to P0

One boot had `P0 = 0x80828000`. On that boot: [VALIDATED]

```text
Kernel code  0x80838000-0x82527fff   = P0 + 0x00010000 .. P0 + 0x01cfffff
Kernel data  0x82938000-0x82bd7fff   = P0 + 0x02110000 .. P0 + 0x023affff
```

| Range | Offset from P0 | Size |
|---|---|---|
| Kernel code | `0x00010000` to `0x01cfffff` | 30,343,168 bytes (28.94 MiB) |
| Kernel data | `0x02110000` to `0x023affff` | 2,752,512 bytes (2.63 MiB) |

The code range starts at `_stext`, which is `_text + 0x10000`. The loader's
`Start linux at PC` value is `phys(_text)`, so it is 64 KiB below the first
`Kernel code` page. That relationship is the runtime check that tied the
loader formula to the physical map. [VALIDATED]

The gap between code and data (`0x01d00000` to `0x0210ffff`) is not labelled in
`/proc/iomem`. The exact section split is not needed for the P0 derivation and
is not reproduced here.

## KASLR

The kernel virtual base is randomized on every boot. `CONFIG_RANDOMIZE_BASE=y`.
A panic log prints the slide, for example:

```text
Kernel Offset: 0x10001d8000 from 0xffffffc080000000
```

That line means the pre-panic `_text` was `0xffffffd0801d8000`. A second boot
printed a different offset (`0x1000430000`), so the slide is random per boot,
not reusable across a watchdog reboot. [VALIDATED]

The link-time base is `KIMAGE_VADDR = 0xffffffc080000000`. The runtime base is
`KIMAGE_VADDR + slide`. Only offsets from `_text` are stable across boots.

There are two independent per-boot choices:

1. The loader places the image at `phys(_text) = 0x80080000 + (TOS byte << 15)`.
   This is the physical load and is not a kernel KASLR decision. [VALIDATED]
2. The kernel picks a virtual slide from `/chosen/kaslr-seed`, which the same
   TOS byte seeds. [VALIDATED]

The tracefs `_THIS_IP_` leak recovers the virtual base for the running boot.
The P0 formula converts that base back into the physical load. See
[../exploit/p0-derivation.md](../exploit/p0-derivation.md) and
[../../reference/offsets.md](../../reference/offsets.md#p0-kernel-physical-load).

## Device MMIO

Selected physical windows from `/proc/iomem`. They are stable platform
addresses, not kernel addresses. [VALIDATED]

| Block | Physical range |
|---|---|
| GIC distributor | `0x12000000` |
| GIC redistributor | `0x12040000` |
| UFS controller | `0x22000000` |
| GPU | `0x23100000` |
| PMIC SPI | `0x64600000` |
| EFUSE | `0x64160000`, `0x65007800` |
| Watchdog | `0x641e0000` |
| UART (early console) | `0x20210000` |

## What is not available

| Channel | State | Tag |
|---|---|---|
| `/proc/kcore` | not configured (`CONFIG_PROC_KCORE` not set) | VALIDATED |
| `/proc/kallsyms` addresses | zeroed at `kptr_restrict=2` | VALIDATED |
| `/proc/modules` bases | zeroed at `kptr_restrict=2` | VALIDATED |
| `/dev/mem`, `/dev/kmem` | absent or not configured | VALIDATED |
| debugfs | present but unmounted by default; a read of `clk_summary`/`regulator_summary` panicked the unit | VALIDATED |
| dmesg from `shell` | denied by SELinux | VALIDATED |
| `/proc/iomem` from `shell` | root-only label and permission | VALIDATED |
| `/proc/iomem` from root | readable | VALIDATED |

The physical map is useful for analysis and for the P0 check. It is not a
general kernel read primitive on its own, and it does not reveal the virtual
slide.

## Related pages

- [Device overview](overview.md)
- [Boot chain](boot-chain.md)
- [Mali](mali.md)
- [Offsets](../../reference/offsets.md)
- [Glossary](../../reference/glossary.md)
