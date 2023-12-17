ISO_NAME = noted.iso

.PHONY: disk

disk: $(OUT_DIR)/$(ISO_NAME)

$(DISK_DIR)/limine/limine.cfg: limine.cfg
	cp $^ $@

$(DISK_DIR)/efi/boot/BOOTX64.EFI: $(LIMINE_DIR)/BOOTX64.EFI
	mkdir -p $(@D)
	cp $^ $@

$(OUT_DIR)/$(ISO_NAME): $(TARGET) $(DISK_DIR)/limine/limine.cfg $(DISK_DIR)/efi/boot/BOOTX64.EFI
	cp $< $(DISK_DIR)/$(nodir $<)
	xorriso -as mkisofs -b limine/limine-bios-cd.bin -no-emul-boot \
    -boot-load-size 4 -boot-info-table --efi-boot \
    limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image \
    --protective-msdos-label $(DISK_DIR) -o $@
	$(LIMINE_EXECUTABLE) bios-install $@
