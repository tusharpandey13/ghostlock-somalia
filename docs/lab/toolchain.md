# Toolchain and artifacts

Three toolchains were used. The phone payload and helper were built with the
Android NDK. The static lab init and marker were built with zig. The
source-parity GKI kernel was built with Homebrew clang and LLD.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence](../methodology/evidence.md).

## Phone payload: NDK r27c

| Item | Value | Tag |
|---|---|---|
| Package | `android-ndk-r27c-darwin.dmg` | VALIDATED |
| Revision | `27.2.12479018` | VALIDATED |
| DMG SHA-256 | `7ec95a80d91cda7b4b69401d5c4a4c285fed729811fdf7ceba5739bc9d81e36b` | VALIDATED |
| Compiler | clang 18.0.3 | VALIDATED |
| Target | `aarch64-unknown-linux-android35` | VALIDATED |
| Host wrapper | `darwin-x86_64`, run under Rosetta 2 on an arm64 host | VALIDATED |

r27c ships only the `darwin-x86_64` prebuilt. The x86_64 wrapper is therefore
the only supported choice, even on an arm64 host.

Build shape:

```
-DAPP_PAYLOAD=1 -DLIB_KMI=66
-include offset.h
-DTARGET_CONFIG_H="<composed target header>"
-fPIC -O2 -g0 -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare
-shared -pthread -Wl,--no-undefined
```

The source order is load-bearing. The composed target header supplies the
device image offsets, the tracefs marker offset `0x1f4ccc`, and the structure
layouts. The link uses `-Wl,--no-undefined`, so a missing bionic or libdl symbol
fails the build instead of producing a broken payload.

The cred-only variant omits the KernelSU handoff, su daemon, su client, mount,
and chcon code. The su install calls resolve to weak `ENOSYS` stubs. That is why
the run log reports `su=0/38 daemon=-1`.

## Lab binaries: zig 0.17.0

The QEMU init, the leak marker, the monitor, and the static lab exploit were
built with zig 0.17.0 for `aarch64-linux-musl`, static:

```
zig cc -target aarch64-linux-musl -static -O2 -s
```

The lab binaries are test tools, not phone artifacts. A zig/musl build has no
effect on KCFI because the forged fops table contains kernel text addresses.
See [cfi.md](../exploit/cfi.md).

## Source-parity kernel: clang 23.1.4

The GKI 6.6.118 comparison kernel used Homebrew clang 23.1.4, LLD 23.1.4, and
`LLVM=1`. The source was `android15-6.6` at commit
`e56cf6b09cca2151bcee244b3d334fb68685ff57`. GNU text tools and a small
`elf.h` shim were needed on macOS. One host-tool patch to
`scripts/mod/file2alias.c` handled a macOS `uuid_t` typedef conflict. That patch
does not touch the kernel under test.

The build produced a 32.9 MB arm64 4 KiB `Image` with UTS release
`6.6.118-4k-ge56cf6b09cca-dirty`, and it booted in the same QEMU machine.

## Artifact hashes

These hashes identify the artifacts used in the recorded runs. The binaries are
not published in this repository.

| Artifact | Size | SHA-256 | Tag |
|---|---:|---|---|
| Vendor kernel `Image` | 35.1 MiB | `81181e8321e655acbce601faff94ab1610f9b825869a56a1c0e5bfcf9db1126c` | VALIDATED |
| Stock entry `.so` | 178.0 KiB | `4037e15d92714cfc71c9f90718713b3ea7b9e11c58a74cd5b1dc98af707c33c7` | VALIDATED |
| `cve-2026-43499-root` helper | 25.7 KiB | `767f85ad5108e02efe7a77fc957ec0a22cca03e64a524b1228f79b2c55339956` | VALIDATED |
| Cred-only `.so`, P0 `0x80000000` | 171776 B | `5804dc353e086263b2fd2c85f4d07647a2e3677c7e6fd6138c2ad2560214f4f0` | VALIDATED |
| Cred-only `.so`, P0 `0x80200000` | 171792 B | `5b8b141fde2b1daef332a59f579af03d05737761b76fbf843fc3bf872ece2c79` | VALIDATED |
| Cred-only `.so` with root socket | not recorded | `cdab57f08f720c9b1886ecaf662449b1a4d436aa3b9a173e55984bb591d5d33e` | VALIDATED |
| First derived-P0 root run log | 83 lines | `e01ca1ab08284a922188b80f40f6924bad82c56f1a657489942d20297ccb15da` | VALIDATED |

The two fixed-P0 `.so` files differ by 16 bytes. `0x80000000` is a valid AArch64
logical immediate and compiles to `orr`/`and` forms. `0x80200000` is not, so the
compiler emits `movz`/`movk` forms. The size difference is evidence that the
override took effect, not a behavior difference.

## Reproducibility

- The build commands and source lists are recorded, but the binaries are not
  bit-for-bit reproducible across hosts. Compiler patch level, linker version,
  and embedded paths can change the output.
- The hashes above are the reference for the runs described in this tree. A
  rebuild that differs is not automatically wrong, but it is not the artifact
  that produced the quoted logs.
- The target header and the tracefs offset are part of the input. A different
  kernel build needs a new header and a new leak offset.
- The phone payload is per-boot in its P0. The current helper can derive P0 at
  runtime from the leak, so a rebuild per boot is no longer required.

## What is not published

No `.so`, executable, kernel image, firmware, initramfs, vendor blob, dexdump,
or key material is in this repository. The hashes are audit references, not
downloads.
