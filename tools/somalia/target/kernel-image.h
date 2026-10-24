// somalia-OS3.0.13.0 kernel-image layer — symbol and structure offsets derived
// offline from fw/extracted/kernel/Image + System.map by
// tools/thirdparty/pixel-ksu-root/tools/pixel-image/derive_offsets.py.
//
// This file is the "kernel image" layer of the target description: facts about
// ONE image, as its banner names it:
//
//   Linux version 6.6.118-android15-8-ge56cf6b09cca-ab15511674-4k
//
// It deliberately carries no interface facts. Stack-map constants
// (SIGRETURN_WAITER_OFF, CLONE3_WAITER_OFF, PROCESS_VM_WAITER_OFF,
// BPF_WAITER_OFF, MCAST_WAITER_OFF, SOCKOPT_WAITER_OFF) and the slab/geometry
// defaults live in cves/targets/kmi/android15-6.6.h, which the composed
// target.h includes AFTER this file. The 6.1-interface discriminator
// PSELECT_WAITER_WORD_SHIFT is absent here on purpose: the android15-6.6 entry
// stage forbids it.
//
// Two symbols were not derived: PWQ_DELAYED_WORKS_OFF (pool_workqueue has no
// 'delayed_works' field in this image's BTF) and MCAST_WAITER_OFF (no
// 0x108-byte group_source_req copy in do_ip_setsockopt). Neither is a build
// requirement; both are covered by fallbacks elsewhere (see the port plan).

#define KIMAGE_TEXT_BASE 0xffffffc080000000ULL

// Derived: profile=android15-6.6, VA_BITS=39, PAGE_SHIFT=12, sizeof(struct page)=64 (Image BTF), VMEMMAP_SIZE=0x100000000
#define VMEMMAP_START                              0xfffffffe00000000ULL
#define ASHMEM_IOCTL_OFF                           0x00c8dc94ULL
#define ASHMEM_MMAP_OFF                            0x00c8e3a4ULL
#define ASHMEM_OPEN_OFF                            0x00c8e5c4ULL
#define ASHMEM_RELEASE_OFF                         0x00c8e64cULL
#define ASHMEM_SHOW_FDINFO_OFF                     0x00c8e6d8ULL
#define ASHMEM_FOPS_OFF                            0x012f78c0ULL
#define ASHMEM_MISC_FOPS_OFF                       0x0228c568ULL
#define CONFIGFS_READ_ITER_OFF                     0x00492250ULL
#define CONFIGFS_BIN_WRITE_ITER_OFF                0x0049277cULL
#define COPY_SPLICE_READ_OFF                       0x00415e78ULL
#define NOOP_LLSEEK_OFF                            0x003c8c18ULL
#define INIT_TASK_OFF                              0x0212e280ULL
#define ROOT_TASK_GROUP_OFF                        0x02328980ULL
/* BTF (fw/extracted/kernel/vmlinux.btf): selinux_state.enforcing is field 0,
 * initialized is field 1. The upstream offset-maps.txt rule encodes the
 * pre-6.6 layout (+1) and produced 0x0236a2e1, which points at `initialized`.
 * Corrected 2026-10-23 from the BTF check; write this byte, not +1. */
#define SELINUX_ENFORCING_OFF                      0x0236a2e0ULL
#define SELINUX_BLOB_SIZES_OFF                     0x01685430ULL
#define SECURITY_HOOK_HEADS_OFF                    0x01684cf8ULL
#define KMALLOC_CACHES_OFF                         0x01684838ULL
#define ANON_PIPE_BUF_OPS_OFF                      0x01176b48ULL
#define EP_POLL_CALLBACK_OFF                       0x00432e70ULL
#define LINUX_BANNER_OFF                           0x0134c4c0ULL
#define INPUT_DEV_LIST_OFF                         0x0226db78ULL
#define CALL_USERMODEHELPER_EXEC_WORK_OFF          0x000d1034ULL
#define SYSTEM_UNBOUND_WQ_OFF                      0x0211ae60ULL
#define SLIDE_NFULNL_LOGGER_OFF                    0x02122260ULL
#define SLIDE_SYSCTL_BOOTID_OFF                    0x0238b2d8ULL
#define SLIDE_LOGGERS_0_1_OFF                      0x021221b0ULL
#define SLIDE_RANDOM_BOOT_ID_DATA_OFF              0x02249468ULL
#define ASHMEM_COMPAT_IOCTL_OFF                    0x00c8e350ULL
#define ASHMEM_IOCTL_CALLABLE_OFF                  0x00c8dc94ULL
#define ASHMEM_COMPAT_IOCTL_CALLABLE_OFF           0x00c8e350ULL
#define ASHMEM_MMAP_CALLABLE_OFF                   0x00c8e3a4ULL
#define ASHMEM_OPEN_CALLABLE_OFF                   0x00c8e5c4ULL
#define ASHMEM_RELEASE_CALLABLE_OFF                0x00c8e64cULL
#define ASHMEM_SHOW_FDINFO_CALLABLE_OFF            0x00c8e6d8ULL
#define COPY_SPLICE_READ_CALLABLE_OFF              0x00415e78ULL
#define CONFIGFS_READ_ITER_CALLABLE_OFF            0x00492250ULL
#define CONFIGFS_BIN_WRITE_ITER_CALLABLE_OFF       0x0049277cULL
#define CALL_USERMODEHELPER_EXEC_WORK_CALLABLE_OFF 0x000d1034ULL
#define POSIX_CPU_TIMER_DEL_RACE_OFF               0x00000134ULL
#define SLIDE_TRACE_MARK_IP_OFF                    0x001f4cccULL
#define TASK_PID_OFF                               0x00000618ULL
#define TASK_TGID_OFF                              0x0000061cULL
#define TASK_REAL_PARENT_OFF                       0x00000628ULL
#define TASK_REAL_CRED_OFF                         0x00000818ULL
#define TASK_CRED_OFF                              0x00000820ULL
#define TASK_COMM_OFF                              0x00000830ULL
#define TASK_TASKS_OFF                             0x00000550ULL
#define TASK_SECCOMP_OFF                           0x000008e8ULL
#define TASK_ATOMIC_FLAGS_OFF                      0x000005d8ULL
#define FAKE_TASK_USAGE_OFF                        0x00000040ULL
#define FAKE_TASK_PRIO_OFF                         0x00000084ULL
#define FAKE_TASK_NORMAL_PRIO_OFF                  0x0000008cULL
#define FAKE_TASK_TASK_GROUP_OFF                   0x00000348ULL
#define FAKE_TASK_PI_LOCK_OFF                      0x0000090cULL
#define FAKE_TASK_PI_WAITERS_OFF                   0x00000920ULL
#define FAKE_TASK_PI_TOP_TASK_OFF                  0x00000930ULL
#define FAKE_TASK_PI_BLOCKED_ON_OFF                0x00000938ULL
#define CRED_UID_OFF                               0x00000008ULL
#define CRED_SECUREBITS_OFF                        0x00000028ULL
#define CRED_CAPS_OFF                              0x00000030ULL
#define CRED_SECURITY_OFF                          0x00000080ULL
#define MM_OWNER_OFF                               0x00000408ULL
#define WAITER_TREE_ENTRY_OFF                      0x00000000ULL
#define WAITER_PI_TREE_ENTRY_OFF                   0x00000028ULL
#define WAITER_TASK_OFF                            0x00000050ULL
#define WAITER_LOCK_OFF                            0x00000058ULL
#define WAITER_WAKE_STATE_OFF                      0x00000060ULL
#define WAITER_WW_CTX_OFF                          0x00000068ULL
#define FOPS_OWNER_OFF                             0x00000000ULL
#define FOPS_LLSEEK_OFF                            0x00000008ULL
#define FOPS_READ_OFF                              0x00000010ULL
#define FOPS_WRITE_OFF                             0x00000018ULL
#define FOPS_READ_ITER_OFF                         0x00000020ULL
#define FOPS_WRITE_ITER_OFF                        0x00000028ULL
#define FOPS_IOCTL_OFF                             0x00000048ULL
#define FOPS_COMPAT_IOCTL_OFF                      0x00000050ULL
#define TASK_GROUP_LEADER_OFF                      0x00000658ULL
#define CRED_USAGE_OFF                             0x00000000ULL
#define CRED_USER_OFF                              0x00000088ULL
#define CRED_USER_NS_OFF                           0x00000090ULL
#define CRED_UCOUNTS_OFF                           0x00000098ULL
#define CRED_GROUP_INFO_OFF                        0x000000a0ULL
#define MISCDEVICE_FOPS_OFF                        0x00000010ULL
#define STRUCT_SLAB_CACHE_OFF                      0x00000008ULL
#define STRUCT_PAGE_COMPOUND_HEAD_OFF              0x00000008ULL
#define EP_OFF_WQ                                  0x00000030ULL
#define EP_OFF_POLL_WAIT                           0x00000048ULL
#define EP_OFF_RDLLIST                             0x00000060ULL
#define EP_OFF_LOCK                                0x00000070ULL
#define EP_OFF_RBR                                 0x00000078ULL
#define EP_OFF_OVFLIST                             0x00000088ULL
#define EP_OFF_WS                                  0x00000090ULL
#define EP_OFF_REFS                                0x000000b0ULL
#define MM_PGD_OFF                                 0x00000070ULL
#define TASK_FILES_OFF                             0x00000860ULL
#define FILES_FDT_OFF                              0x00000020ULL
#define FDT_FD_OFF                                 0x00000008ULL
#define FILE_PRIVATE_DATA_OFF                      0x000000d8ULL
#define PIPE_INODE_OFF_RING_SIZE                   0x0000006cULL
#define WAIT_QUEUE_HEAD_OFF                        0x00000008ULL
#define WAIT_QUEUE_ENTRY_OFF_ENTRY                 0x00000018ULL
#define INPUT_DEV_NAME_OFF                         0x00000000ULL
#define INPUT_DEV_USERS_OFF                        0x00000238ULL
#define INPUT_DEV_NODE_OFF                         0x000005f8ULL
#define EPITEM_OFF_RDLLINK                         0x00000018ULL
#define EPITEM_OFF_FFD                             0x00000030ULL
#define EPITEM_OFF_PWQLIST                         0x00000040ULL
#define EPITEM_OFF_EP                              0x00000048ULL
#define EPITEM_OFF_FLLINK                          0x00000050ULL
#define EPITEM_OFF_WS                              0x00000060ULL
#define EPITEM_OFF_EVENT                           0x00000068ULL
#define EPPOLL_OFF_BASE                            0x00000008ULL
#define EPPOLL_OFF_WAIT                            0x00000010ULL
#define FILE_OFF_F_INODE                           0x000000b8ULL
#define FILE_OFF_F_OP                              0x000000c0ULL
#define FILE_OFF_F_LOCK                            0x00000010ULL
#define FILE_OFF_F_COUNT                           0x00000018ULL
#define FILE_OFF_F_POS                             0x00000050ULL
#define FILE_OFF_PRIVATE_DATA                      0x000000d8ULL
#define FILE_OFF_F_EP                              0x000000e0ULL
#define INODE_OFF_I_SB                             0x00000028ULL
#define INODE_OFF_I_INO                            0x00000040ULL
#define SUPER_BLOCK_OFF_S_DEV                      0x00000010ULL
#define PIPE_INODE_OFF_HEAD                        0x00000060ULL
#define PIPE_INODE_OFF_TAIL                        0x00000064ULL
#define PIPE_INODE_OFF_BUFS                        0x000000a8ULL
#define PIPE_BUFFER_OFF_PAGE                       0x00000000ULL
#define PIPE_BUFFER_OFF_OFFSET                     0x00000008ULL
#define PIPE_BUFFER_OFF_LEN                        0x0000000cULL
#define PIPE_BUFFER_OFF_OPS                        0x00000010ULL
#define PIPE_BUFFER_OFF_FLAGS                      0x00000018ULL
#define PIPE_BUFFER_OFF_PRIVATE                    0x00000020ULL
#define CFG_PAGE_OFF                               0x00000010ULL
#define CFG_NEEDS_READ_FILL_OFF                    0x00000050ULL
#define CFG_BIN_BUFFER_OFF                         0x00000058ULL
#define CFG_BIN_BUFFER_SIZE_OFF                    0x00000060ULL
#define CFG_CB_MAX_SIZE_OFF                        0x00000064ULL
#define FOPS_MMAP_OFF                              0x00000058ULL
#define FOPS_OPEN_OFF                              0x00000068ULL
#define FOPS_RELEASE_OFF                           0x00000078ULL
#define FOPS_SPLICE_READ_OFF                       0x000000b8ULL
#define FOPS_SHOW_FDINFO_OFF                       0x000000d8ULL
#define STRUCT_PAGE_SIZE                           0x00000040ULL
#define WQ_DFL_PWQ_OFF                             0x000000b0ULL
#define PWQ_POOL_OFF                               0x00000000ULL
#define PWQ_WQ_OFF                                 0x00000008ULL
#define PWQ_WORK_COLOR_OFF                         0x00000010ULL
#define PWQ_REFCNT_OFF                             0x00000018ULL
#define PWQ_NR_IN_FLIGHT_OFF                       0x0000001cULL
#define PWQ_NR_ACTIVE_OFF                          0x0000005cULL
#define PWQ_MAX_ACTIVE_OFF                         0x00000060ULL
// UNRESOLVED PWQ_DELAYED_WORKS_OFF — pool_workqueue.delayed_works: no field 'delayed_works' (has: pool, wq, work_color, flush_color, refcnt, nr_in_flight, nr_active, max_active...)
#define POOL_WORKLIST_OFF                          0x00000028ULL
#define POOL_NR_IDLE_OFF                           0x0000003cULL
#define WORK_DATA_OFF                              0x00000000ULL
#define WORK_ENTRY_OFF                             0x00000008ULL
#define WORK_FUNC_OFF                              0x00000018ULL
#define SECCOMP_MODE_OFF                           0x00000000ULL
#define SECCOMP_FILTER_OFF                         0x00000008ULL
