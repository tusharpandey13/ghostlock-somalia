# Incident evidence excerpt

Selected sanitized lines from the two device incidents. Full pstore captures,
module lists, and long addresses stay in the private audit store. The function
names and offsets needed to identify the failure are preserved.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence policy](../docs/methodology/evidence.md).

## debugfs clock read

Boot reason after the self-reboot:

```
kernel_panic,synchroomus_external_abort:_fatal_exception
```

The spelling is preserved from the device property. It means
`synchronous external abort: fatal exception`.

Observed after the reboot:

- Android booted on slot `_b`.
- Verified boot was green.
- SELinux was Enforcing.
- The root channel was gone.
- No new pstore entry was visible to shell.

No stack was captured for this incident. The attribution to the
`clk_summary`/`regulator_summary` reads is INFERRED from the timing. The
postmortem is in [debugfs.md](../docs/incidents/debugfs.md).

## binder module_put

Selected pstore stack lines:

```
Unable to handle kernel paging request at virtual address [redacted]
Internal error: Oops: 0000000096000004 [#1] PREEMPT SMP
CPU: 0  Comm: binder:1234_7
pc : module_put+0x18/0x12c
lr : __fput+0xc4/0x2b8
Kernel panic - not syncing: Oops: Fatal exception
```

The signature is `__fput` calling `module_put` on a file whose `f_op->owner` was
not a valid kernel pointer. The task is a binder thread, not the root server.

The faulting virtual address is redacted. The module list and any unrelated
addresses are omitted. The `module_put` and `__fput` frames are kept because the
claim depends on them.

Boot state after the panic:

- SELinux Enforcing
- no root server
- slot `_b`
- verified boot green
- data intact
- no brick

The inherited-fd and forged-fops link is INFERRED, not proven. The reasoning and
the selective-fd change are in
[module-put.md](../docs/incidents/module-put.md).

## What is redacted

- the long fault address
- kernel module list and unrelated symbol addresses
- boot identifiers and process identifiers
- device identifiers, network identifiers, and authorization material
- full pstore files and raw console captures

The redactions remove data that is not needed for the technical claim. They do
not change the function, offset, or call path.

## Links

- [debugfs incident](../docs/incidents/debugfs.md)
- [module_put incident](../docs/incidents/module-put.md)
- [Root success excerpt](root-success.md)
