PDB_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/pdb
SOURCES += $(PDB_DRIVER_DIR)/src/fsl_pdb_driver.c \
           $(PDB_DRIVER_DIR)/common/fsl_pdb_common.c
INCLUDES += $(PDB_DRIVER_DIR) \
            $(PDB_DRIVER_DIR)/common
