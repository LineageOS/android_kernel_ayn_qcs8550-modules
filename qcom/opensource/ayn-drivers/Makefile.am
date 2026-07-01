
AYN_ROOT=$(ROOTDIR)vendor/qcom/opensource/ayn-drivers
KBUILD_OPTIONS := AYN_ROOT=$(AYN_ROOT) CONFIG_MSM_AYN=m

ifeq ($(TARGET_SUPPORT),genericarmv8)
	KBUILD_OPTIONS += CONFIG_ARCH_KALAMA=y
endif

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) modules $(KBUILD_OPTIONS)

modules_install:
	$(MAKE) INSTALL_MOD_STRIP=1 -C $(KERNEL_SRC) M=$(M) modules_install

%:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $@ $(KBUILD_OPTIONS)

clean:
	rm -f *.o *.ko *.mod.c *.mod.o *~ .*.cmd Module.symvers
	rm -rf .tmp_versions
