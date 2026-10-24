# Incident: module_put panic after a dual-socket su test

A dual-socket root build passed its smoke tests, then the device panicked in
`module_put` through `__fput` and rebooted. The panic happened on a binder
thread, not on the root server. Root was lost on the new boot. The device still
booted Android on slot `_b` and did not brick.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence](../methodology/evidence.md).

## What was being tested

The build offered two root paths:

- an abstract Unix socket, peer UID 0 or 2000
- a private Termux socket, mode 0600, owned by the Termux UID

Before the panic, the tests passed:

- `su -c id` through the private socket returned uid 0
- the Termux app UID was denied on the abstract socket
- a host `adb forward` to the abstract socket returned uid 0
- a cold start recreated the server and the private socket

The device then dropped off USB during cleanup. adb returned `error: closed`.
The unit re-enumerated on a new boot with SELinux Enforcing and no root server.

## Panic signature

Selected pstore lines:

```
Unable to handle kernel paging request at virtual address [redacted]
Internal error: Oops: 0000000096000004 [#1] PREEMPT SMP
CPU: 0  Comm: binder:1234_7
pc : module_put+0x18/0x12c
lr : __fput+0xc4/0x2b8
Kernel panic - not syncing: Oops: Fatal exception
```

The call path is `__fput` calling `module_put`. The file being closed had an
`f_op->owner` value that was corrupted or not a kernel pointer. The faulting
task is a binder thread. It is not the root server. VALIDATED from pstore.

## Working hypothesis

The dual server added a `close_inherited_fds()` step. It closed the exploit
process's inherited descriptors, including pipe and memfd descriptors, when the
server started.

One of those descriptors could still have referenced a file whose `f_op` was the
forged GhostLock table. Closing such a file would call `fput` on it, then
`module_put` on the forged `owner`. A fake table does not have a valid module
owner. That is a plausible path to this exact stack.

The link is not proven. Three facts keep the attribution open:

- The fault is on a binder thread, not on the server that did the closing.
- The panic is delayed relative to the dual-server start.
- GhostLock itself creates forged fops and files, so a latent collateral fault
  from the root stage cannot be excluded.

Tag: INFERRED, not VALIDATED.

## Selective-fd fix

The server lifecycle was changed after the panic. The new rule is to close only
pipe and regular-file descriptors. Character devices, ashmem descriptors, and
sockets are left alone. That avoids closing a descriptor that might still carry
a forged `f_op`, while still releasing the pipe descriptors that caused an
earlier `F_SETPIPE_SZ` failure.

The fixed build was re-rooted on the new boot:

- derived P0 `0x801f0000`
- capture succeeded
- `uid 2000 -> 0`
- `pipe physrw root=1`
- both the abstract socket and the private Termux socket answered
- no new panic during the short watch window

This is a reliability fix, not a capability change. It does not survive a
reboot, and it does not make the channel stable by itself. The build remains
under observation.

## What this does not prove

- It does not prove the inherited-fd close was the sole cause.
- It does not prove the selective-fd rule covers every forged descriptor.
- It does not make the dual-socket build safe to reuse.
- It does not make root persistent.

A clean short watch is not a stability result. The earlier build also passed its
smoke tests before it panicked.

## Current rule

Do not use the dual-socket build, `gl-root`, or the private `su` path again
until the teardown is fixed and tested under a fresh session. The safer server
design is to close only known pipe and memfd descriptors, or to exec a clean
server process that never inherits the exploit's files.

debugfs clock and regulator summaries remain off-limits. See
[debugfs.md](debugfs.md).

## Recovery

The reboot restored Enforcing and changed P0. Re-rooting needs a new
authorization, a fresh tracefs leak, a new P0, and a new run. The old P0 is
invalid. The device booted slot `_b` and did not brick.

## Evidence

The selected stack lines are in
[incidents.md](../../evidence/incidents.md). The server and per-boot boundary
are described in [cred-patch.md](../exploit/cred-patch.md).
