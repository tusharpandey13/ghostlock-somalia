# QEMU lab

The lab boots the exact vendor kernel image under QEMU on an arm64 host. It is
good enough to validate the leak, the pselect route, the forged fops table, and
the credential patch. It is not the phone. The loader, physical addresses,
timing, and hardware are different.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence](../methodology/evidence.md).

## Exact vendor Image

| Item | Value | Tag |
|---|---|---|
| Source | `boot.img` from the device's OS3.0.13.0 build | VALIDATED |
| Kernel | uncompressed arm64 `Image`, 4 KiB pages | VALIDATED |
| UTS release | `6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k` | VALIDATED |
| SHA-256 | `81181e8321e655acbce601faff94ab1610f9b825869a56a1c0e5bfcf9db1126c` | VALIDATED |

The kernel in `boot.img` is byte-identical to the extracted `Image`. Booting it
in QEMU reproduces the phone's tracefs marker offset `0x1f4ccc` and the same
`_THIS_IP_` arithmetic.

## QEMU setup

- QEMU 11.1.2 with HVF acceleration on an arm64 host
- `-M virt -cpu max -smp 4 -m 2048 -accel hvf`
- kernel: the vendor `Image`
- initrd: a small static initramfs built with zig 0.17.0 for
  `aarch64-linux-musl`
- console: `ttyAMA0`, `rdinit=/init`

The initramfs is about 26 KB. It mounts proc, sysfs, and tracefs. `devtmpfs`
does not mount because the vendor kernel builds it as a module and no module is
present. That does not affect the leak or the chain.

The leak reproduced inside the VM with the same low-12 value `0xccc`. The VM's
slide is 2 MiB aligned and different from any phone slide, which is expected.

## GKI source parity

A second lab kernel was built from the Android common `android15-6.6` branch at
commit `e56cf6b09cca2151bcee244b3d334fb68685ff57`. The build used Homebrew
clang 23.1.4 and LLD 23.1.4 with `LLVM=1`. It boots under the same QEMU machine
and also enables KASLR.

This build is useful for source-level checks and for a second data point on the
leak. Its tracefs offset is `0x19ac64`, not the phone's `0x1f4ccc`, because the
compiler schedule differs. That is why the offset belongs in a target header.

## Initramfs and the umask bug

The first GhostLock VM run failed at `cfi-open` with `EACCES`. The cause was the
lab init, not CFI and not the driver.

The kernel spawns init with `umask(0022)`. The lab called
`mknod("/dev/ashmem", S_IFCHR | 0666, ...)`, and `mknod` applied the umask. The
node landed at `0644`. The init then dropped to uid 2000, which is "other" on a
root-owned node, so `open(O_RDWR)` failed at the VFS permission check before
`misc_open` or any forged table was reached.

The fix is `umask(0)` before the `mknod`. The node then has mode `0666`, the
open returns a real descriptor, and the chain proceeds. This is a lab harness
bug. The phone's `/dev/ashmem` already has the right mode and does not need it.

## Root in the VM

With the umask fixed, the stock chain reached full root in the VM on attempt 1:

- `open-ashmem` returned a real descriptor
- the pselect route captured the forged fops table
- the configfs handlers gave kernel read/write
- the pipe primitive reached uid 0
- the credential patch reported `uid 2000 -> 0`, full capabilities, and the
  SELinux byte cleared

The forged fops slots are genuine kernel text, so KCFI passed. The virtual-data
variant that avoids the physical alias reached the CFI stage but its root
re-route was blocked by `CONFIG_HARDENED_USERCOPY`. The pipe path is the working
one. See [cfi.md](../exploit/cfi.md).

## Forced-miss harness

The retry stall was reproduced with a lab-only hook in `try_cfi_stage()`, gated
on `GHOSTLOCK_FORCE_FIRST_MISS=1` and `cfi_attempts == 1`. The hook returns the
same `captured=0`, `pre_fops=0` shape as a real miss, so the route enters the
refresh path deterministically.

The lab init reads `ghostlock.force_miss` from `/proc/cmdline` and can fork a
monitor. The monitor dumps per-task `comm`, `state`, `wchan`, syscall, and stack
for pid 1 and its children. That is how the parked waiter was identified as
sitting in `wait4` on the leak child.

Results:

- Unpatched, 26 attempts: second groom took about 38 seconds, then a later
  attempt panicked with a user-memory access outside uaccess.
- Process-wide watchdog: bounded the scan but delivered the signal to the wrong
  thread and corrupted teardown.
- One attempt per process: clean miss, outer retry, correct exit, no stall.
- One attempt, no forced miss: normal attempt-1 root.

The full analysis is in [pselect-race.md](../exploit/pselect-race.md).

## What the lab cannot show

- Phone P0. QEMU's loader places the image at a different physical address.
  The VM used a known local value, `0x40200000`, and the chain's alias worked
  there. The phone needs the per-boot formula in
  [p0-derivation.md](../exploit/p0-derivation.md).
- Phone timing. HVF is fast and the race is won cleanly on attempt 1. TCG and
  phone behavior differ.
- Vendor hardware. There is no Mali GPU, PMIC, modem, display, or full Android
  userspace. The chain does not need them, but a result that depends on them
  cannot be tested here.
- Phone SELinux policy. The VM boots with no policy loaded. The phone's policy
  is what makes the tracefs leak and `/dev/ashmem` reachable from shell.

## Limits

The lab validates the chain's logic and the CFI claim. It does not validate a
phone run, a phone P0, or stability. A VM root is not a device root.

No binaries, initramfs images, kernel images, or firmware are published in this
repository. The build inputs and hashes are in
[toolchain.md](toolchain.md).
