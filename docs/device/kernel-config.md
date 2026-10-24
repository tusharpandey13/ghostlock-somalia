# Kernel config

Selected build options that matter for memory disclosure, control-flow
integrity, SELinux, tracing, and crash behaviour. Values are from the device's
`/proc/config.gz` on a rooted boot. The full config is 7,755 lines and is not
reproduced here.

Tags are VALIDATED, INFERRED, UNVALIDATED, BLOCKED.

## Symbols and introspection

| Option | Value | Effect | Tag |
|---|---|---|---|
| `CONFIG_KALLSYMS` | `y` | symbol table built in | VALIDATED |
| `CONFIG_KALLSYMS_ALL` | `y` | data symbols included | VALIDATED |
| `CONFIG_KALLSYMS_BASE_RELATIVE` | `y` | relative encoding | VALIDATED |
| `CONFIG_DEBUG_INFO_BTF` | `y` | BTF present in `vmlinux` | VALIDATED |
| `CONFIG_DEBUG_INFO_BTF_MODULES` | `y` | module BTF allowed; the Mali module still has no `.BTF` section | VALIDATED |
| `CONFIG_DEBUG_FS` | `y` | debugfs built in | VALIDATED |
| `CONFIG_DEBUG_FS_ALLOW_ALL` | `y` | root can mount debugfs | VALIDATED |
| `CONFIG_DYNAMIC_DEBUG` | not set | no dyndbg control | VALIDATED |
| `CONFIG_PROC_KCORE` | not set | no `/proc/kcore` | VALIDATED |
| `CONFIG_KPROBES` | `y` | kprobe framework | VALIDATED |
| `CONFIG_KPROBE_EVENTS` | `y` | tracefs kprobe events | VALIDATED |
| `CONFIG_FTRACE` | `y` | tracing core | VALIDATED |
| `CONFIG_KASAN` | `y` | KASAN compiled | VALIDATED |
| `CONFIG_KASAN_HW_TAGS` | `y` | tag mode selected, but cannot arm without MTE | VALIDATED config, INFERRED runtime off |
| `CONFIG_KFENCE` | `y` | sampled allocator checks | VALIDATED |

The symbol table exists, but `kptr_restrict=2` masks it at runtime. The BTF is
useful offline; it is not readable through `/sys/kernel/btf/vmlinux` from
`shell`. [VALIDATED]

## KASLR and control-flow integrity

| Option | Value | Effect | Tag |
|---|---|---|---|
| `CONFIG_RANDOMIZE_BASE` | `y` | kernel virtual KASLR | VALIDATED |
| `CONFIG_RANDOMIZE_MODULE_REGION_FULL` | not set | module region only partly randomised | VALIDATED |
| `CONFIG_CFI_CLANG` | `y` | KCFI on indirect calls | VALIDATED |
| `CONFIG_CFI_PERMISSIVE` | not set | a CFI miss traps, it does not warn | VALIDATED |
| `CONFIG_ARM64_PTR_AUTH_KERNEL` | `y` | pointer auth compiled for the kernel | VALIDATED |
| `CONFIG_ARM64_BTI_KERNEL` | not set | kernel BTI off | VALIDATED |
| `CONFIG_SHADOW_CALL_STACK` | `y` | kernel shadow call stack | VALIDATED |
| `CONFIG_STRICT_KERNEL_RWX` | `y` | text is read-only and executable, data is NX | VALIDATED |
| `CONFIG_ARM64_MTE` | `y` | MTE configured | VALIDATED |
| `CONFIG_HARDENED_USERCOPY` | `y` | `copy_to_user` checks object bounds | VALIDATED |
| `CONFIG_FORTIFY_SOURCE` | `y` | fortified string and memory helpers | VALIDATED |

Compiled PAC is not the same as active PAC. `/proc/cpuinfo` does not advertise
`paca` or `pacg`, and the Mali module carries PAC instructions that retire as
hints. MTE is configured but the hardware does not advertise it, so KASAN
HW_TAGS stays inert. Both points are covered in [mali.md](mali.md). [VALIDATED
for the config and HWCAP absence, INFERRED for the runtime effect]

## Memory hardening

| Option | Value | Tag |
|---|---|---|
| `CONFIG_INIT_STACK_ALL_ZERO` | `y` | VALIDATED |
| `CONFIG_INIT_ON_ALLOC_DEFAULT_ON` | `y` | VALIDATED |
| `CONFIG_INIT_ON_FREE_DEFAULT_ON` | not set | VALIDATED |
| `CONFIG_STACKPROTECTOR` | `y` | VALIDATED |
| `CONFIG_STACKPROTECTOR_STRONG` | `y` | VALIDATED |
| `CONFIG_VMAP_STACK` | `y` | VALIDATED |
| `CONFIG_RANDOMIZE_KSTACK_OFFSET` | `y` | VALIDATED |
| `CONFIG_RANDOMIZE_KSTACK_OFFSET_DEFAULT` | not set | VALIDATED |
| `CONFIG_SLAB_FREELIST_RANDOM` | `y` | VALIDATED |
| `CONFIG_SLAB_FREELIST_HARDENED` | `y` | VALIDATED |
| `CONFIG_SHUFFLE_PAGE_ALLOCATOR` | `y` | VALIDATED |
| `CONFIG_RANDOM_KMALLOC_CACHES` | not set | VALIDATED |
| `CONFIG_LIST_HARDENED` | `y` | VALIDATED |
| `CONFIG_BUG_ON_DATA_CORRUPTION` | `y` | VALIDATED |

`RANDOM_KMALLOC_CACHES` being off keeps kmalloc cache geometry stable. That is a
grooming advantage, not a weakness in the exploit sense. `INIT_ON_FREE` being off
means freed objects are not immediately zeroed.

## SELinux and LSM

| Option | Value | Tag |
|---|---|---|
| `CONFIG_SECURITY_SELINUX` | `y` | VALIDATED |
| `CONFIG_SECURITY_SELINUX_DEVELOP` | `y` | VALIDATED |
| `CONFIG_SECURITY_SELINUX_BOOTPARAM` | not set | VALIDATED |
| `CONFIG_SECURITY_SELINUX_AVC_STATS` | `y` | VALIDATED |
| `CONFIG_SECURITYFS` | `y` | VALIDATED |
| `CONFIG_SECURITY_DMESG_RESTRICT` | not set | VALIDATED |
| `CONFIG_LSM` | `landlock,lockdown,yama,loadpin,safesetid,selinux,...` | VALIDATED |
| `CONFIG_LSM_MMAP_MIN_ADDR` | `32768` | VALIDATED |

The LSM list and the policy state are covered in [selinux.md](selinux.md).

## Namespaces and attack surface

| Option | Value | Note | Tag |
|---|---|---|---|
| `CONFIG_FUTEX_PI` | `y` | required by the GhostLock bug class | VALIDATED |
| `CONFIG_USER_NS` | not set | no unprivileged user namespaces | VALIDATED |
| `CONFIG_SYSVIPC` | not set | no `msg_msg` / SysV queues | VALIDATED |
| `CONFIG_POSIX_MQUEUE` | not set | no POSIX mqueue grooming | VALIDATED |
| `CONFIG_USERFAULTFD` | `y` | timing primitive available in the kernel | VALIDATED |
| `CONFIG_IO_URING` | `y` | allocator and timing surface | VALIDATED |
| `CONFIG_BPF_UNPRIV_DEFAULT_OFF` | not set | `unprivileged_bpf_disabled=0` at runtime | VALIDATED |
| `CONFIG_PERF_EVENTS` | `y` | perf present; shell scope is still subject to SELinux and `perf_harden` | VALIDATED |
| `CONFIG_BINFMT_MISC` | `y` | misc binary format enabled | VALIDATED |
| `CONFIG_DEVMEM` | not set | no `/dev/mem` | VALIDATED |

## Modules and panic behaviour

| Option | Value | Tag |
|---|---|---|
| `CONFIG_MODULES` | `y` | VALIDATED |
| `CONFIG_MODVERSIONS` | `y` | VALIDATED |
| `CONFIG_MODULE_SIG` | `y` | VALIDATED |
| `CONFIG_MODULE_SIG_FORCE` | not set | VALIDATED |
| `CONFIG_MODULE_SIG_PROTECT` | `y` | VALIDATED |
| `CONFIG_PANIC_ON_OOPS` | `y` | VALIDATED |
| `CONFIG_PANIC_ON_OOPS_VALUE` | `1` | VALIDATED |
| `CONFIG_PANIC_TIMEOUT` | `-1` | VALIDATED |

A kernel oops panics the unit. The PMIC/AP watchdog then resets the SoC, which
is how the two controlled panics recovered without a manual power cycle. The
`panic=-1` value alone would leave the kernel waiting; the watchdog is the
recovery path. [VALIDATED]

`MODULE_SIG_FORCE` is unset, so an unsigned module can load if it matches the
kernel's vermagic and symbol versions. That is not useful here without a write
primitive and does not change the locked-boot model.

## Related pages

- [Device overview](overview.md)
- [SELinux](selinux.md)
- [Mali](mali.md)
- [Memory map](memory-map.md)
- [Glossary](../../reference/glossary.md)
