LIMINE_FILES = limine-bios.sys limine-bios-cd.bin limine-uefi-cd.bin
LIMINE_DIR = $(OUT_DIR)/limine
LIMINE_EXECUTABLE = $(LIMINE_DIR)/limine
DEST_FILES = $(addprefix $(DISK_DIR)/limine/,$(LIMINE_FILES))

limine: $(DEST_FILES) $(LIMINE_EXECUTABLE) 

.PHONY: limine

$(LIMINE_DIR):
	git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1 $(LIMINE_DIR)

$(DEST_FILES): $(LIMINE_DIR)
	mkdir -p $(@D)
	-cp $^/$(@F) $@

$(LIMINE_EXECUTABLE): $(LIMINE_DIR)
	$(MAKE) -C $^

