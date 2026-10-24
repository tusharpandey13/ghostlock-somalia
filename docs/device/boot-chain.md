# Boot chain

The `somalia` boot chain is a Unisoc EDK2/UEFI flow, not a U-Boot flow with a
`kernel_addr_r` environment. The kernel load address is picked at boot from a
TrustZone reply, which is why a fixed P0 does not work on this device.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Components

| Stage | Artifact | Notes | Tag |
|---|---|---|---|
| Boot ROM / SPL | `spl_a` on `sdb`, `spl_b` on `sdc`, 4 MiB each | separate block devices, not `sda` partitions | VALIDATED |
| U-Boot / EDK2 payload | `uboot_a`, `uboot_b`, 8 MiB each | `edk2-sign.bin` is a `DHTB` container with an LZMA-compressed EDK2 firmware volume | VALIDATED |
| Kernel loader | `SprdBoot.efi` inside the EDK2 volume | the code that loads the kernel and prints the entry PC | VALIDATED |
| Boot image | `boot_b`, 100 MiB partition | Android header v4, kernel only, no ramdisk | VALIDATED |
| Init ramdisk | `init_boot_b`, 8 MiB partition | LZ4 ramdisk, no kernel | VALIDATED |
| Vendor boot | `vendor_boot_b`, 100 MiB partition | vendor header v4, ramdisk plus DTB | VALIDATED |
| Bootloader log | `uboot_log`, 16 MiB partition | keeps historical `Start linux at PC` lines | VALIDATED |

The kernel inside `boot_b` is a PE/EFI-stub image (`MZ` at offset `0x1000`) and
is byte-identical to the extracted `Image` used for offline analysis.
[VALIDATED]

## Flow

1. The SPL starts the EDK2 firmware from `uboot_a` or `uboot_b`.
2. On a normal boot, `SprdBoot.efi` calls the TrustZone protocol with command
   `0x50` and a 4-byte output buffer.
3. The loader takes the first byte of the reply, shifts it left by 15, and
   writes the result to PCD token 31. [VALIDATED by disassembly]
4. The loader reads the kernel from the boot image into
   `0x80080000 + PCD31`. [VALIDATED]
5. The loader enters the kernel at the same expression and passes the DTB
   address `0x86000000` in `x0`. [VALIDATED]

The relevant log strings in the loader are:

```text
%a kernel read OK, page size = 0x%llx, locate to 0x%lx
Start linux at PC:0x%lx, dt_addr:%p
```

`uboot_log` records the entry line for each boot. One historical line read
`Start linux at PC:0x80828000`, and `/proc/iomem` on that rooted boot showed
`Kernel code` at `0x80838000`, exactly `P0 + 0x10000`. That agreement is the
strongest runtime confirmation of the formula. [VALIDATED]

## Physical load formula

```text
phys(_text) = 0x80080000 + (byte << 15)
byte        = first byte of the TOS command-0x50 reply, 0..255
step        = 0x8000 (32 KiB)
range       = 0x80080000 .. 0x80878000
```

`0x80080000` is the fixed loader base. `0x80000000` is below it and can never be
the load. `0x80200000` is correct only for byte `0x30`, one value out of 256, so
it is not a default. [VALIDATED]

The exact byte is not stored in the boot images or DTB. It is chosen per normal boot and
also seeds `/chosen/kaslr-seed`, so the same byte can be recovered from the
kernel's virtual KASLR slide. The derivation and the exploit consequence are in
[the exploit P0 notes](../exploit/p0-derivation.md) and
[offsets.md](../../reference/offsets.md#p0-kernel-physical-load). The script
[`ghostlock_p0_from_slide.py`](../../scripts/ghostlock_p0_from_slide.py) does
the arithmetic.

## Image header facts

- `boot.img` uses Android header v4. Header v4 has no `kernel_addr`,
  `ramdisk_addr`, or `tags_addr` fields. The earlier `kernel_addr=0` reading was
  a header-layout error, not a load address. [VALIDATED]
- `vendor_boot.img` uses vendor header v4 and carries `kernel_addr=0x8000`,
  `ramdisk_addr=0x05400000`, and `dtb_addr=0x1f00000`. The `0x8000` value is the
  AOSP mkbootimg default and is not consumed as the kernel load. [VALIDATED]
- The DTB memory node starts at `0x80000000`, and the first 20 KiB of DRAM are
  reserved for `ddrbist`, `sysdumpinfo`, `pdbg`, `debugkinfo`, and `pmic`. A
  kernel at `0x80000000` would overwrite those reservations. This supports the
  loader evidence but is not the primary proof. [VALIDATED data, INFERRED
  conclusion]
- Both slots round the kernel size up to a 4 KiB page when logging. For
  `boot_b`, the kernel is `0x2318a00` bytes and the loader logs
  `page size = 0x2319000`. [VALIDATED]

## What the loader does not provide

- There is no U-Boot environment and no `kernel_addr_r` variable to read.
- The boot image header does not contain the load address.
- The physical load is not a compile-time constant.
- A fixed P0 from one boot is invalid on the next boot.

The kernel virtual base is KASLR-randomized independently. The loader's physical
placement and the kernel's virtual slide both change per boot. The virtual slide
is what the tracefs leak recovers; the P0 formula turns that slide back into the
physical load. See [memory-map.md](memory-map.md).

## Related pages

- [Device overview](overview.md)
- [AVB and verified boot](avb.md)
- [Partitions](partitions.md)
- [Memory map](memory-map.md)
- [Offsets](../../reference/offsets.md)
- [Glossary](../../reference/glossary.md)
- [Exploit P0 notes](../exploit/p0-derivation.md)
