MODULE = kernel
include $(MODULE)/arch/$(CPU_ARCH)/local.mk
C_SRCS += $(MODULE)/main.c $(MODULE)/font.c $(MODULE)/screen.c $(MODULE)/printf.c

