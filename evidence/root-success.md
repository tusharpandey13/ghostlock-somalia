# Root success excerpt

Selected sanitized lines from the first derived-P0 phone run. This is historical
evidence for a per-boot result, not a current device state and not a
persistence claim. The full log stays in the private audit store.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence policy](../docs/methodology/evidence.md).

## Run summary

| Item | Value | Tag |
|---|---|---|
| Date | 2026-10-23 | VALIDATED |
| P0 | `0x80828000` | VALIDATED |
| Helper exit | 0 after 56 seconds | VALIDATED |
| pselect attempts | captured on the first attempt | VALIDATED |
| Kernel panic | none during the run | VALIDATED |
| Boot changed | no | VALIDATED |

The log SHA-256 was
`e01ca1ab08284a922188b80f40f6924bad82c56f1a657489942d20297ccb15da`.

## Selected log lines

Long identifiers and process IDs are redacted. The control-flow text and the
credential fields are preserved.

```
cfi op step=1 open-ashmem              ret=878 errno=0 reopens=0
cfi op step=4 miscfops-restore-write   ret=8 errno=0 captured=1 n=1 reopens=0
cfi op step=11 install-root-attempt-begin ... attempt=2/12
root cred patched uid=0/0 sid=1/1
root child result done=1 uid_after=0 setgid=0/0 setuid=0/0 setenforce=0/0
    su=0/38 daemon=-1 wallpaper=0/38 selinux=1->0
pselect route done=1 calls=1 success=1 step=0 errno=0
pipe physrw pid=<redacted> done=1 root=1 kaslr=1 verify=skipped
    uid=2000->0 sid=1914/1914->1/1 selinux=1->0 setgid=0 setuid=0
```

The important values:

- `captured=1`: the forged fops table was installed
- `root cred patched uid=0/0`: the child credential is uid 0
- `uid_after=0`: the child still had uid 0 after the patch
- `setgid=0/0`, `setuid=0/0`, `setenforce=0/0`: the userspace calls succeeded
- `selinux=1->0`: the global enforcing byte changed to Permissive
- `uid=2000->0`: the pipe primitive reached the credential patch
- `su=0/38 daemon=-1`: no su daemon, `38` is `ENOSYS`

## Capabilities

A separate read from the root channel on the same boot reported:

```
Uid: 0  Gid: 0
CapEff: 000001ffffffffff
CapPrm: 000001ffffffffff
CapBnd: 000001ffffffffff
Seccomp: 0
NoNewPrivs: 0
```

The capability sets are full. The root channel was a separate per-boot process,
not the root child from the run above.

## What this shows

The public GhostLock chain, with a P0 derived from the tracefs leak, reached a
uid-0 kernel credential on this device. The CFI stage captured on the first
pselect attempt. The run completed without a panic and without changing the boot
image.

## What this does not show

- A persistent root shell. The root child exited. A fresh `adb shell id` stayed
  uid 2000.
- A stable root channel. The root socket is per boot and dies with a reboot.
- Cross-reboot persistence. The credential, the Permissive byte, and P0 are all
  per boot.
- A disk write, a package install, a mount, or a boot-image change.
- That the P0 value is reusable. `0x80828000` belongs to that boot only.

## Links

- [GhostLock overview](../docs/exploit/ghostlock.md)
- [P0 derivation](../docs/exploit/p0-derivation.md)
- [Credential patch](../docs/exploit/cred-patch.md)
- [Incident evidence](incidents.md)
