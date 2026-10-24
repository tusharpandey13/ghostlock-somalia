# Device matrix

A small comparison with the public work this port builds on. Only the `somalia`
row was tested here. The other rows are public work and are not independently
reproduced in this repository.

| Device | SoC | Public work | Root shown | Per-boot P0 | Notes |
|---|---|---|---|---|---|
| Google Pixel (various) | Tensor | `JingMatrix/pixel-ksu-root` | yes | fixed physical load in the published target | upstream 6.6 chain and KernelSU handoff |
| OnePlus (various) | Qualcomm | `JoinChang/ghostlock-oneplus` | public port | per-device target headers | broader device matrix; useful comparison point |
| Redmi A7 Pro 5G `somalia` | Unisoc T8300 | this repo | yes, per boot | derived from the tracefs slide | TOS-randomized physical load; two incident records |

The interesting delta is the Unisoc loader. The Pixel and OnePlus ports provide
the chain and the device-matrix pattern; this port adds the TOS-byte/P0
derivation, a QEMU lab that boots the exact vendor Image, and the failure
analysis from the phone runs.

## What is not in the table

- No claim that the same P0 formula applies to another Unisoc device.
- No claim of a stable root channel.
- No claim of cross-reboot persistence.

See [boot-chain](boot-chain.md), [p0 derivation](../exploit/p0-derivation.md),
and [offsets](../../reference/offsets.md).
