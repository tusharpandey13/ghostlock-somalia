# Incident: debugfs clock summary panic

A read-only debugfs recon session panicked the device and caused a self-reboot.
The mount was authorized. The unmount never ran. The unit booted Android again
on the same slot, with no brick, but the root channel and the boot's P0 were
lost.

Tags: VALIDATED, INFERRED, UNVALIDATED, BLOCKED. See
[evidence](../methodology/evidence.md).

## Sequence

1. debugfs was not mounted. The root socket was live, SELinux was Permissive,
   and the device was on a known boot.
2. The operator mounted debugfs read-write at `/sys/kernel/debug`.
3. Top-level and depth-2 listings were captured, along with Mali, DRM, devfreq,
   regulator, and partial clock listings.
4. Reading `clk_summary` and `regulator_summary` correlated with a kernel panic:
   `synchronous external abort: fatal exception`.
5. adb dropped. USB stayed enumerated.
6. The unit self-rebooted. The boot reason was
   `kernel_panic,synchroomus_external_abort:_fatal_exception` (sic). The slot
   stayed `_b`, verified boot stayed green, and SELinux returned to Enforcing.

The exact offending register access was not captured. The timing is tightly
correlated with the clock and regulator summary reads. Treat the attribution as
INFERRED, not proven.

## Why the reboot happened

A synchronous external abort is a fatal exception from a bad device access, not
an ordinary oops. The session had `panic_on_oops=1` and a panic timeout
configured, but those settings did not keep the unit up. The abort path rebooted
the device.

The likely cause is a summary walk over a clock or power domain that was gated
or powered down. Reading such a register can produce an external abort. That is
INFERRED. The lesson does not depend on the exact register: broad clock and
regulator summaries are unsafe on this unit.

## Pstore

No new pstore entry was visible to shell after the reboot. The visible pstore
files were from an earlier, unrelated Mali panic. The post-reboot session had no
root, so any root-only pstore path stayed unreadable.

The incident record therefore rests on the live boot reason string and the
timing, not on a captured ramoops stack.

## What survived

| Item | State | Tag |
|---|---|---|
| Android boot | completed on slot `_b` | VALIDATED |
| Verified boot | green | VALIDATED |
| Brick | none | VALIDATED |
| Data | intact | VALIDATED |
| SELinux | back to Enforcing | VALIDATED |
| debugfs mount | gone after reboot | VALIDATED |
| Root socket | gone | VALIDATED |
| Old P0 | invalid on the new boot | VALIDATED |

The reboot was a recovery by accident, not a designed cleanup. The mount
disappeared because the kernel restarted, not because the session unmounted it.

## Lesson

- Do not read `clk_summary`, `regulator_summary`, or broad clock and power
  summaries on this device without a disposable unit and a crash plan.
- A read-only debugfs node can still touch hardware. Read-only at the VFS layer
  does not mean side-effect free at the device layer.
- `panic_on_oops=1` is not a guard against every fatal exception. Plan for a
  reboot, not just a stuck process.
- Keep the root channel and the leak/P0 work on the same boot. A reboot
  invalidates both.
- After a panic, stop the session. Re-rooting needs a new authorization, a fresh
  leak, a new P0, and a new run.

## Current rule

debugfs clock and regulator summaries remain off-limits. Other debugfs nodes
are not automatically safe either. Any future mount is a state change that
needs an explicit session lift and an unmount plan.

## Evidence

The selected incident lines are in
[incidents.md](../../evidence/incidents.md). The general safety gate is in
[safety.md](../methodology/safety.md).
