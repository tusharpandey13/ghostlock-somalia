# Timeline

Sanitized operation journey for the `somalia` port of GhostLock
(CVE-2026-43499). Dates only, no operator or tool identifiers. Boot IDs are
truncated; P0 values are per-boot and must never be reused. Root is per-boot
throughout.

## 2026-10-22 to 2026-10-23: firmware and the Mali start

- Firmware for `OS3.0.13.0.WBWINXM` downloaded, hashed, and extracted.
- Kernel `Image`, `System.map`, `vmlinux`, BTF, and kallsyms recovered offline.
- Boot chain mapped: Unisoc EDK2/UEFI flow, `SprdBoot.efi` as the kernel loader.
- Mali module `mali_kbase.ko` identified as `r54p1-12eac0`, with the
  `kbase_do_syncset()` page-UAF against the per-context shrinker.
- The race is validated by two controlled crash-oracle panics in
  `kbase_sync_single`, both auto-recovered by the hardware watchdog.
- The write primitive stays blocked, so the project pivots to GhostLock.

## 2026-10-23: the pivot and the leak

- GhostLock CVE-2026-43499 selected: public, hardware-verified 6.6 chain.
- A current-boot KASLR leak is validated from a plain `shell` through tracefs
  (`trace_marker` to `trace_pipe_raw`, `_THIS_IP_` offset `0x1f4ccc`).
- The QEMU lab boots the exact vendor `Image`; the stock chain reaches full root
  in-VM on attempt 1.

## 2026-10-23: P0 misses and the derivation

- Two phone runs with fixed P0 `0x80000000` and `0x80200000` both miss at the
  CFI open step; no panic, no root.
- The firmware audit explains the misses: the loader places the kernel at
  `0x80080000 + (TOS byte << 15)`, so `0x80000000` is always wrong and
  `0x80200000` is right for one byte in 256.
- The same TOS byte seeds `/chosen/kaslr-seed`, so the tracefs slide contains
  the physical load. The P0 formula is derived and written up.

## 2026-10-23: first root

- Leak `_text = 0xffffffd080628000`, derive P0 `0x80828000`, rebuild the
  cred-only payload, one bounded attempt.
- `captured=1`, `root cred patched uid=0/0`, `selinux=1->0`,
  `pipe physrw root=1`, uid 2000 to 0. Helper exits 0 after 56 seconds. No
  panic.
- A second run starts an abstract-socket root server that survives the payload
  as a uid-0 process. Root recon begins.

## 2026-10-23: recon findings

- `uboot_log` and `/proc/iomem` independently confirm P0 `0x80828000`.
- Slot A found unbootable (`tries_remaining: 0`); slot B is the only boot path.
- `misc` holds a pending one-shot bootloader intent, so reboots are risky.
- `kptr_restrict=2` masks kernel symbols even as root.

## 2026-10-23 to 2026-10-24: debugfs panic

- A read-only recon pass mounts debugfs and reads `clk_summary` and
  `regulator_summary`.
- The unit panics with a synchronous external abort and self-reboots. Root,
  Permissive, and P0 are lost with the boot.

## 2026-10-24: re-root and the runtime launcher

- Re-root on the new boot: leak `_text = 0xffffffd080720000`, P0 `0x80720000`.
- A self-contained launcher is installed that leaks KASLR, derives P0, runs the
  payload, and starts the server, with no host-side rebuild.
- The server is re-rooted with a close-fd fix.

## 2026-10-24: wireless bootstrap

- Termux plus `android-tools` provide an on-device ADB client.
- The client connects to the phone's own wireless adbd over loopback, opens a
  shell session, and runs the launcher, closing the on-device bootstrap.
- A Termux public key is appended to `/data/misc/adb/adb_keys`, with a backup.

## 2026-10-24: the binder panic

- A dual-socket build adds a private Termux `su`/`sudo` path. Smoke tests pass.
- The device panics during cleanup: `module_put+0x18` from `__fput`, on a
  binder thread, with a corrupted `f_op->owner`.
- The suspected cause is `close_inherited_fds()` in the server closing an
  inherited exploit fd with a forged fops table. Not proven.
- The unit reboots. Root, Permissive, and P0 are lost again.

## 2026-10-24: the selective-fd fix, under observation

- The server is changed to close only pipe and regular fds, not
  char/ashmem/socket fds.
- Re-root on the new boot, P0 `0x801f0000`. `captured=1`, uid 2000 to 0,
  `pipe physrw root=1`.
- Both channels work: private `~/.gl/root.sock` for `su -c id`, abstract
  `@ghostlock-root` for host forwarding.
- No new panic in the short watch window. The fix is under observation, not
  proven stable. The prior dual-socket build stays do-not-use.
