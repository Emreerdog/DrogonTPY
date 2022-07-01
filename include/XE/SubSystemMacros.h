#ifndef XE_ENV_SUBSYSTEMMACROS_H
#define XE_ENV_SUBSYSTEMMACROS_H

#define XE_SUBSYSTEM_FILESYSTEM 0x0001
#define XE_SUBSYSTEM_THREADING 0x0002
#define XE_SUBSYSTEM_MEMORY 0x0003
#define XE_SUBSYSTEM_PATH_TRAITS 0x0004
#define XE_SUBSYSTEM_THREAD_SYNC 0x0005
#define XE_SUBSYSTEM_INTERNAL_ARRAY 0x0006
#define XE_SUBSYSTEM_XEPYTHON 0x0007
#define XE_SUBSYSTEM_CLI 0x0008
#define XE_SUBSYSTEM_LIBLOADER 0x0009

#define XE_SUBSYSTEM_XEPYTHON_STEP_SIZE 128
#define XE_SUBSYSTEM_THREADING_STEP_SIZE 32
#define XE_SUBSYSTEM_FILESYSTEM_STEP_SIZE 64
#define XE_SUBSYSTEM_MEMORY_STEP_SIZE 64
#define XE_SUBSYSTEM_LIBLOADER_STEP_SIZE 16

#endif // XE_ENV_SUBSYSTEMMACROS_H