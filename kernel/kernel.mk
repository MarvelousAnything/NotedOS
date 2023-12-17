MODULE = kernel
include $(MODULE)/arch/$(CPU_ARCH)/local.mk
C_SRCS += $(MODULE)/main.c

