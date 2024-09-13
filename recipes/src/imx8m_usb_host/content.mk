#
# Specific driver portions
#

MIRROR_FROM_REP_DIR := src/include/spec/arm_v8a/lx_emul \
                       src/driver/usb_host/imx8m \
                       src/lib/imx/lx_emul

#
# Generic driver poritions from dde_linux and os
#
MIRROR_FROM_OS_DIR  := src/lib/genode_c_api/usb.cc

DDE_LINUX_REP_DIR  := $(GENODE_DIR)/repos/dde_linux

content: $(MIRROR_FROM_REP_DIR) $(MIRROR_FROM_OS_DIR)

$(MIRROR_FROM_REP_DIR):
	$(mirror_from_rep_dir)

$(MIRROR_FROM_OS_DIR):
	mkdir -p $(dir $@)
	cp -r $(GENODE_DIR)/repos/os/$@ $@

content: LICENSE
LICENSE:
	( echo "Linux is subject to GNU General Public License version 2, see:"; \
	  echo "https://www.kernel.org/pub/linux/kernel/COPYING"; ) > $@
