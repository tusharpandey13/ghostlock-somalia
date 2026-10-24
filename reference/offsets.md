# Offsets

Symbol and structure offsets for the `somalia` kernel image and target. All
values are offsets from a base, not live kernel addresses. A live base changes
with KASLR and with the bootloader's physical placement, so none of the runs in
this repository should be copied without the matching per-boot base.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Bases

| Name | Value | Meaning | Tag |
|---|---|---|---|
| `KIMAGE_TEXT_BASE` / `KIMAGE_VADDR` | `0xffffffc080000000` | link-time `_text` virtual base | VALIDATED |
| `PHYS_OFFSET` | `0x80000000` | DRAM base, not the kernel load | VALIDATED |
| `P0_BASE` | `0x80080000` | fixed loader base | VALIDATED |
| `P0_STEP` | `0x8000` | 32 KiB per TOS byte | VALIDATED |
| `P0_RANGE` | `0x80080000` to `0x80878000` | 256 possible loads | VALIDATED |
| `PAGE_SHIFT` | `12` | 4 KiB pages | VALIDATED |

`PHYS_OFFSET` and `P0_BASE` are different things. Treating `0x80000000` as the
kernel load is the error that made the first two fixed-P0 runs miss.

## Kernel text

| Symbol | Offset from `_text` | Tag |
|---|---|---|
| `_text` | `0x00000000` | VALIDATED |
| `_stext` | `0x00010000` | VALIDATED |

The loader's `Start linux at PC` value is `phys(_text)`. `/proc/iomem` starts
`Kernel code` at `_stext`, 64 KiB above it. On a boot with `P0 = 0x80828000`,
the code range began at `0x80838000`.

## Ashmem

| Symbol / field | Offset from `_text` | Tag |
|---|---|---|
| `ashmem_fops` | `0x012f78c0` | VALIDATED |
| `ashmem_misc` | `0x0228c558` | VALIDATED |
| `ashmem_misc.fops` (`ASHMEM_MISC_FOPS_OFF`) | `0x0228c568` | VALIDATED |
| `ashmem_ioctl` | `0x00c8dc94` | VALIDATED |
| `compat_ashmem_ioctl` | `0x00c8e350` | VALIDATED |
| `ashmem_mmap` | `0x00c8e3a4` | VALIDATED |
| `ashmem_open` | `0x00c8e5c4` | VALIDATED |
| `ashmem_release` | `0x00c8e64c` | VALIDATED |
| `ashmem_show_fdinfo` | `0x00c8e6d8` | VALIDATED |

`ASHMEM_MISC_FOPS_OFF` is `ashmem_misc + 0x10`, the `fops` pointer field inside
`struct miscdevice`. It is not the fops table itself. `misc_open` reads this
field, which is why the field is the write target in the chain. [VALIDATED]

The raw `ashmem_misc` bytes confirm `minor = 255` (`MISC_DYNAMIC_MINOR`),
`name = "ashmem"`, and `fops = ashmem_fops`. There is no second ashmem fops table
in this image. [VALIDATED]

## CFI callable offsets

The forged fops table must hold genuine kernel text pointers. Each entry below is
the exact start of the named function, so its KCFI type preamble is the real one.
[VALIDATED]

| Callable | Offset from `_text` |
|---|---|
| `ASHMEM_IOCTL_CALLABLE` (`ashmem_ioctl`) | `0x00c8dc94` |
| `ASHMEM_COMPAT_IOCTL_CALLABLE` (`compat_ashmem_ioctl`) | `0x00c8e350` |
| `ASHMEM_MMAP_CALLABLE` (`ashmem_mmap`) | `0x00c8e3a4` |
| `ASHMEM_OPEN_CALLABLE` (`ashmem_open`) | `0x00c8e5c4` |
| `ASHMEM_RELEASE_CALLABLE` (`ashmem_release`) | `0x00c8e64c` |
| `ASHMEM_SHOW_FDINFO_CALLABLE` (`ashmem_show_fdinfo`) | `0x00c8e6d8` |
| `COPY_SPLICE_READ_CALLABLE` (`copy_splice_read`) | `0x00415e78` |
| `CONFIGFS_READ_ITER_CALLABLE` (`configfs_read_iter`) | `0x00492250` |
| `CONFIGFS_BIN_WRITE_ITER_CALLABLE` (`configfs_bin_write_iter`) | `0x0049277c` |
| `CALL_USERMODEHELPER_EXEC_WORK_CALLABLE` (`call_usermodehelper_exec_work`) | `0x000d1034` |

The `struct file_operations` layout for this image puts `show_fdinfo` at `0xd8`,
not `0xd0`. A 6.1-style table would be one slot off. [VALIDATED]

## CFI stage and data offsets

| Name | Offset from `_text` | Tag |
|---|---|---|
| `SELINUX_ENFORCING_OFF` | `0x0236a2e0` | VALIDATED |
| `SELINUX_BLOB_SIZES_OFF` | `0x01685430` | VALIDATED |
| `SECURITY_HOOK_HEADS_OFF` | `0x01684cf8` | VALIDATED |
| `KMALLOC_CACHES_OFF` | `0x01684838` | VALIDATED |
| `ANON_PIPE_BUF_OPS_OFF` | `0x01176b48` | VALIDATED |
| `INIT_TASK_OFF` | `0x0212e280` | VALIDATED |
| `ROOT_TASK_GROUP_OFF` | `0x02328980` | VALIDATED |
| `SYSTEM_UNBOUND_WQ_OFF` | `0x0211ae60` | VALIDATED |

`SELINUX_ENFORCING_OFF` is the `selinux_state.enforcing` byte. BTF shows
`enforcing` at field 0 and `initialized` at field 1. The upstream offset map
encoded the pre-6.6 layout with a `+1`, which pointed at `initialized`. The value
above is the corrected one. [VALIDATED]

## Structure offsets

| Field | Offset | Tag |
|---|---|---|
| `task_struct.pid` | `0x00000618` | VALIDATED |
| `task_struct.tgid` | `0x0000061c` | VALIDATED |
| `task_struct.real_cred` | `0x00000818` | VALIDATED |
| `task_struct.cred` | `0x00000820` | VALIDATED |
| `task_struct.comm` | `0x00000830` | VALIDATED |
| `task_struct.files` | `0x00000860` | VALIDATED |
| `cred.uid` | `0x00000008` | VALIDATED |
| `cred.cap_*` | `0x00000030` | VALIDATED |
| `cred.security` | `0x00000080` | VALIDATED |
| `file.f_op` | `0x000000c0` | VALIDATED |
| `file.private_data` | `0x000000d8` | VALIDATED |
| `struct page` size | `0x00000040` | VALIDATED |
| `struct miscdevice.fops` | `0x00000010` | VALIDATED |
| `struct file_operations.show_fdinfo` | `0x000000d8` | VALIDATED |

## Tracefs IP offset

| Name | Offset | Tag |
|---|---|---|
| `SLIDE_TRACE_MARK_IP_OFF` | `0x001f4ccc` | VALIDATED |

This is not a symbol start. It is the offset of the `_THIS_IP_` value recorded by
a write to `trace_marker`. The leak works as:

```text
_text = leaked_ip - 0x1f4ccc
slide = _text - 0xffffffc080000000
```

The offset is specific to this exact `Image`, compiler, and config. A source-
parity GKI build at the same commit produced `0x19ac64`, so a different kernel
build needs its own value. The device offset was confirmed with two stable reads
in the same boot. [VALIDATED]

## P0 kernel physical load

```text
phys(_text) = 0x80080000 + (byte << 15)
byte        = first byte of the TOS command-0x50 reply, 0 .. 255
```

Recover it from a virtual slide:

```text
slide = _text_runtime - 0xffffffc080000000
byte  = (((slide >> 21) & 3) << 6)
      | ((((slide & 0x1fffff) - 0x80000) & 0x1fffff) >> 15)
P0    = 0x80080000 + (byte << 15)
```

The script [`ghostlock_p0_from_slide.py`](../scripts/ghostlock_p0_from_slide.py)
implements the same arithmetic and asserts the range and step.

Why the slide carries the physical bits: the same TOS byte 0 seeds both the
loader's PCD 31 and `/chosen/kaslr-seed`. The arm64 KASLR code takes the low 21
bits of its virtual offset from the physical placement, so the virtual slide is
enough to invert the loader choice. [VALIDATED mechanism, VALIDATED on-device by
three roots]

Examples from recorded boots, all historical and not reusable:

| `_text` | Slide | P0 |
|---|---|---|
| `0xffffffd0803e0000` | `0x10003e0000` | `0x803e0000` |
| `0xffffffd080628000` | `0x1000628000` | `0x80828000` |
| `0xffffffd080720000` | `0x1000720000` | `0x80720000` |

Do not reuse any of these. The byte changes on every normal boot.

## Derivation notes

- Symbol offsets come from `System.map` as `symbol_VA - _text`, with the link
  base `0xffffffc080000000`.
- The callable offsets are symbol starts. The chain needs genuine function
  prologues so KCFI accepts the forged table.
- `ASHMEM_MISC_FOPS_OFF` is the `fops` field, not the symbol. The `+0x10` comes
  from the `struct miscdevice` layout in BTF.
- `SELINUX_ENFORCING_OFF` was corrected from the upstream map by checking BTF
  field order, not by trusting the pre-6.6 rule.
- The tracefs offset comes from disassembly of the `Image` and was verified on
  the phone, not copied from another device.
- The P0 formula comes from the vendor `SprdBoot.efi` disassembly: TOS command
  `0x50`, PCD token 31, `byte << 15`, plus the fixed `0x80080000` base.

No serial number, IMEI, MAC address, or live boot base is included in this
table.

## Related pages

- [Device overview](../docs/device/overview.md)
- [Boot chain](../docs/device/boot-chain.md)
- [Memory map](../docs/device/memory-map.md)
- [Mali](../docs/device/mali.md)
- [Hashes](hashes.md)
- [Glossary](glossary.md)
