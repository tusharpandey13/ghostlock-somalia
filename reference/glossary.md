# Glossary

Short definitions for terms used in this repository. Each entry says what the
term means on this device, not the general textbook definition.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## P0

The physical address where the bootloader places the kernel image, specifically
`phys(_text)`. On `somalia` it is not fixed:

```text
P0 = 0x80080000 + (TOS byte << 15)
```

The TOS byte is chosen per normal boot, so P0 changes on every boot. The value is
recovered from the boot's KASLR slide. [VALIDATED]

Do not confuse P0 with `PHYS_OFFSET` (`0x80000000`), which is the DRAM base.

## KASLR

Kernel address space layout randomization. The kernel's virtual base is
randomized per boot. On this device the slide is random across boots and a
panic prints it only for the boot that is about to die. A current-boot leak is
needed; a previous boot's slide is useless. [VALIDATED]

The loader's physical placement is a separate per-boot choice. It is not the
kernel's KASLR decision, but it changes the same addresses.

## CFI / KCFI

Control-flow integrity. Android arm64 uses KCFI, a compiler-inserted type hash
checked on indirect calls. `CONFIG_CFI_CLANG=y` and `CONFIG_CFI_PERMISSIVE` is
not set, so a type mismatch traps. [VALIDATED]

KCFI does not protect return addresses or data pointers. A forged data table is
accepted only if every function pointer in it is a genuine function of the
expected type. The GhostLock chain uses genuine kernel text addresses for this
reason.

## AVB

Android Verified Boot. A signed chain from the bootloader through `vbmeta` to
the boot and system partitions. The unit is locked with a green verified-boot
state, `SHA256_RSA4096`, and rollback indices. AVB is why a modified boot image
or a persistent system change is not part of this work. [VALIDATED]

## dm-verity

The block-layer integrity check used on the read-only system volumes. Eight
erofs volumes are mounted through dm-verity: `system`, `system_ext`, `vendor`,
`odm`, `product`, `mi_ext`, `vendor_dlkm`, and `system_dlkm`. A write to any of
them would fail the hash tree. [VALIDATED]

## TOS

TrustZone OS, the secure-world firmware on the SoC. On this device the normal
boot path calls it with command `0x50` and a 4-byte buffer. The first reply byte
sets the kernel's physical load and also seeds the kernel's virtual KASLR. The
`trustos_a` and `trustos_b` partitions hold the secure OS image in a `DHTB`
container. [VALIDATED for the boot call and partitions, BLOCKED for payload
internals]

## tracefs

The tracing filesystem, mounted at `/sys/kernel/tracing`. On this build
`trace_marker` is writable and `trace_pipe_raw` is readable by the `shell`
group. Writing to `trace_marker` records a `_THIS_IP_` value, and reading the
raw pipe reveals the kernel text address for the current boot. That is the
KASLR leak used here. [VALIDATED]

`kprobe_events` is mode `0777`, but the per-probe `enable` files are `0640`
root:readtracefs, so `shell` can create a probe and cannot enable it.

## pselect

The `pselect6` syscall. GhostLock uses it as the stack-reclaim vehicle: the
kernel copies a user `fd_set` onto the stack, the syscall blocks, and a stale
waiter or stack object is reclaimed after the copy. The chain's target is
`stack_fds` word 0. [VALIDATED in the lab, VALIDATED on the phone with the
derived P0]

## PI waiter

A priority-inheritance futex waiter, `struct futex_pi_state` and its
`rt_mutex_waiter`. GhostLock is a use-after-free in this path:
`remove_waiter()` operates on `current` instead of the waiter's task at the
shipped commit. The freed waiter is reclaimed with controlled data, which gives
the rb-tree write that follows. [VALIDATED by source inspection and by the root
runs]

## crosscache

Cross-cache reclaim. A freed object from one slab cache is reallocated from a
different cache or page type by shaping allocations. In this chain it is used to
turn the freed futex waiter into controlled memory and then to place a forged
fops table. [VALIDATED in the QEMU lab and on the phone]

## SELinux

The mandatory access control layer. The device boots Enforcing. After a
successful root, the payload writes `selinux_state.enforcing = 0`, which makes
the current boot Permissive. The change is in memory and does not survive a
reboot. [VALIDATED]

`u:r:kernel:s0` is the context of the root server. A normal `adb shell` remains
`u:r:shell:s0` at uid 2000.

## per-boot

A property that lasts for one boot and is lost on reboot. On this device the
following are per-boot:

- root and the root credential patch,
- Permissive SELinux,
- the abstract-socket root server,
- P0 and the KASLR slide,
- any process started by the exploit.

The installed launcher files can persist on `/data`, but they are not root until
a shell-context bootstrap runs them again. [VALIDATED]

## Related pages

- [Offsets](offsets.md)
- [Hashes](hashes.md)
- [Device overview](../docs/device/overview.md)
- [Boot chain](../docs/device/boot-chain.md)
- [SELinux](../docs/device/selinux.md)
