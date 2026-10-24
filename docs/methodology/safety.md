# Safety and authorization

This work was done on one device owned by the operator. Every run needed an
explicit per-session authorization. The default posture was read-only. The
exploit is not a turnkey tool and is not meant for devices the reader does not
own or have written permission to test.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence.md](evidence.md).

## Device constraints

| Constraint | State | Tag |
|---|---|---|
| Bootloader | locked | VALIDATED |
| Verified boot | green | VALIDATED |
| Bootable slot | `_b` only; slot A has no tries left | VALIDATED |
| System and vendor partitions | covered by verified boot and dm-verity | VALIDATED |
| Root persistence | none across reboot | VALIDATED |
| Fallback unit | none | VALIDATED |

Slot A is not a recovery path. If slot B fails to boot, there is no second slot
to fall back to. That fact sets the risk budget: one panic ends the session.

## Per-session authorization

The following actions need a written go for that session, not a standing
permission:

- writing `trace_marker` or changing `tracing_on`
- running the exploit or a credential patch
- mounting debugfs or changing a kernel tunable
- changing network state for a wireless path
- changing ADB authorization files
- running a root server or a private `su` path

Read-only inspection that does not change state can be done under the general
audit authorization. A write that is "probably harmless" still needs the go.

## No partition writes

No partition is written. No boot image is flashed, no `super` image is changed,
no `vbmeta` is edited, and no bootloader unlock is attempted. The exploit writes
kernel memory only.

The one file outside the run directory that was changed in the audit was an ADB
authorization file for a wireless bootstrap. It was backed up first and is
called out in the incident record. It is not part of the exploit and is not
published.

Files under the device's temporary directory can survive a reboot. That is file
persistence, not root persistence. The credential patch, the Permissive state,
the root server, and the boot's P0 do not survive.

## One-panic stop

Stop the session after the first kernel panic. Do not retry in the same session.

1. Capture pstore if it is readable.
2. Record the boot reason and the visible stack.
3. Let the unit reboot on its own if it is going to.
4. Confirm Android boots and the data is intact.
5. Do not run the exploit again until a new session is authorized.

The reason is not superstition. A panic means the kernel state is no longer the
state that was tested. A second run can turn a recoverable crash into a worse
one, especially with only one bootable slot.

## Debugfs and device reads

debugfs clock and regulator summaries are off-limits. A read-only debugfs node
can still touch gated hardware and produce a synchronous external abort. That
already happened once and rebooted the unit. See
[debugfs.md](../incidents/debugfs.md).

Other device reads follow the same rule. If a node is not understood, do not
read a broad summary on the daily driver. Use a disposable unit or skip it.

## Network posture

Keep the device off-network during an exploit window unless the session
explicitly authorizes a wireless path. An OTA freeze was not in place for this
work. Airplane mode and Wi-Fi off reduce the chance of an update changing the
kernel or the SELinux policy in the middle of a run.

A wireless bootstrap is a separate change. It needs its own go, a backup of the
authorization file, and a restore step when the work is done.

## What is not claimed

- Stable root
- Cross-reboot persistence
- A safe universal exploit
- A bootloader unlock
- A modified boot image
- A root manager installation

The result is a per-boot credential patch and, optionally, a per-boot root
process. That is the whole claim.

## If you are reproducing this

Use a device you own or have written authorization to test. Read the whole
evidence policy first. Do not start with the daily driver. Do not reuse a P0
from another boot. Do not read debugfs summaries. Stop after the first panic.
