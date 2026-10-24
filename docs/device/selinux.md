# SELinux

The policy is Android's standard modular policy with a Unisoc vendor extension.
The baseline is Enforcing. A rooted boot becomes Permissive because the kernel
payload writes `selinux_state.enforcing = 0`; that change is in memory and dies
with the boot.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Policy facts

| Fact | Value | Tag |
|---|---|---|
| Policy version | 33 | VALIDATED |
| MLS | enabled (`1`) | VALIDATED |
| `checkreqprot` | 0 | VALIDATED |
| Object classes | 107 | VALIDATED |
| Booleans | none (`count=0`) | VALIDATED |
| Policy blob mode | `0444` | VALIDATED |
| Policy size | 1,225,213 bytes | VALIDATED |
| Policy SHA-256 | `8f16da7b21a25245e65449844a8154dd694f7b1046b0cb4e4e21c3441de8caa1` | VALIDATED |
| `CONFIG_SECURITY_SELINUX_DEVELOP` | `y` | VALIDATED |
| `CONFIG_SECURITY_SELINUX_BOOTPARAM` | not set | VALIDATED |

The policy text and CIL were used for analysis. The binary policy is not
redistributed here.

## Enforcing and Permissive

| Boot state | `getenforce` | `enforce` | Tag |
|---|---|---|---|
| Cold boot | `Enforcing` | `1` | VALIDATED |
| After the GhostLock payload | `Permissive` | `0` | VALIDATED |
| After reboot | `Enforcing` again | `1` | VALIDATED |

Permissive is a runtime side effect of the credential patch, not a policy
change. No permissive domain is defined in the policy; the global flag is what
changes. A reboot restores the default. [VALIDATED]

The root process runs in `u:r:kernel:s0`, not in a userspace domain. A fresh
`adb shell` still runs as `u:r:shell:s0` with uid 2000; the root credential
patch applies to the exploit's child and to the socket server it starts.
[VALIDATED]

## Contexts observed

Selected file contexts from the rooted baseline. [VALIDATED]

| Path | Context |
|---|---|
| `/data` | `u:object_r:system_data_root_file:s0` |
| `/data/local/tmp` | `u:object_r:shell_data_file:s0` |
| `/metadata` | `u:object_r:metadata_file:s0` |
| `/system` | `u:object_r:system_file:s0` |
| `/vendor` | `u:object_r:vendor_file:s0` |
| `/dev/block/by-name` | `u:object_r:block_device:s0` |
| `/dev/mali0` | `u:object_r:gpu_device:s0` |
| `/dev/modem` | `u:object_r:loader_device:s0` |
| `/dev/wcn` | `u:object_r:device:s0`, root-only |
| `/proc/iomem` | `u:object_r:proc_iomem:s0`, `0444` |
| `/proc/kallsyms` | `u:object_r:proc_kallsyms:s0`, `0444` |
| `/sys/fs/selinux/policy` | `u:object_r:selinuxfs:s0` |
| `/sys/kernel/tracing/kprobe_events` | `u:object_r:debugfs_tracing:s0`, `0777` |

Process contexts seen in a 620-process snapshot include `platform_app`,
`system_app`, `priv_app`, `tee`, `netd`, `rild`, `radio`, `zygote`,
`vndservicemanager`, `vendor_init`, and `untrusted_app` instances. [VALIDATED]

## kptr_restrict and kernel pointers

`kptr_restrict=2` is the default. At that value: [VALIDATED]

- `/proc/kallsyms` lists symbol names with every address
  `0000000000000000`.
- `/proc/modules` lists module names with every base
  `0x0000000000000000`.
- `/sys/module/*/sections/*` reads zero.
- `/proc/vmallocinfo` virtual columns stay hashed even when
  `kptr_restrict` is temporarily lowered to 0. Only the `phys=` column is real.
- `/proc/mali/memory_usage` still prints live `kctx` pointers. That is a
  separate interface and is not covered by `kptr_restrict`.

A temporary 2 to 0 to 2 drop was used once, under a separate authorization, to
capture the symbol table; the value was restored and verified. The default stays
2. [VALIDATED]

## Trace and GPU surface

| Interface | Mode | Label | Concern | Tag |
|---|---|---|---|---|
| `/sys/kernel/tracing/kprobe_events` | `0777` | `debugfs_tracing` | any local process can create kprobe events | VALIDATED |
| `events/kprobes/*/enable` | `0640` root:readtracefs | `debugfs_tracing` | creation works, enabling is blocked for `shell` | VALIDATED |
| `trace_marker` | `0222` | `debugfs_tracing` | writable by `shell`; the `_THIS_IP_` record leaks the current kernel slide through `trace_pipe_raw` | VALIDATED |
| `trace_pipe_raw` | `0640` group `readtracefs` | `debugfs_tracing` | `shell` is in group 3012, so it can read | VALIDATED |
| `/dev/mali0` | `0666` | `gpu_device` | open, mmap, and ioctl are allowed to ordinary app domains with no ioctl allowlist | VALIDATED |
| `/proc/mali/*` | mixed | `proc_mali` | `memory_usage` leaks `kctx` pointers; `jit_can_reclaim` is `0664` | VALIDATED |
| `/sys/kernel/debug` | directory | `debugfs` | present but unmounted; debugfs `clk_summary`/`regulator_summary` reads panicked the unit | VALIDATED |

The tracefs path is the working shell-context KASLR leak. It is not an
accidental write primitive: `kprobe_events` creation is allowed, but the
`enable` files are not writable by `shell`. The leak uses `trace_marker` and the
raw trace pipe, not a kprobe. [VALIDATED]

## Potential hardening issues

These are observations about the stock configuration, not exploit claims.

1. `kprobe_events` is world-writable. A local process can create probe
   definitions even if it cannot enable them. That is unnecessary exposure and
   creates a persistent tracefs object if cleanup is missed. [VALIDATED]
2. `/dev/mali0` is `0666` and the GPU ioctl is not filtered by SELinux
   `allowxperm`. The driver's own ioctl switch is the only gate. The stock policy
   grants `ioctl`, `read`, `write`, `map`, and `open` on the node to
   `appdomain`. [VALIDATED]
3. `/proc/mali/memory_usage` prints live kernel `kctx` pointers to a world
   readable interface. That is a kernel-address disclosure independent of
   `kptr_restrict`. [VALIDATED]
4. `CONFIG_DEBUG_FS_ALLOW_ALL=y` means a root process can mount debugfs. On
   this unit a debugfs read of clock and regulator summaries caused a
   synchronous external abort and a panic, so debugfs is off-limits in practice.
   [VALIDATED]
5. `CONFIG_SECURITY_DMESG_RESTRICT` is not set, but `dmesg` from `shell` is
   denied by SELinux and the sysctl is not readable from `shell`. The effective
   exposure is lower than the config alone suggests. [VALIDATED for the denial,
   INFERRED for the config effect]
6. `CONFIG_SECURITY_SELINUX_DEVELOP=y` lets a root process change the policy
   state. That is expected on an Android user build but widens the effect of any
   kernel write primitive. [VALIDATED]

## Related pages

- [Device overview](overview.md)
- [Kernel config](kernel-config.md)
- [Mali](mali.md)
- [Glossary](../../reference/glossary.md)
