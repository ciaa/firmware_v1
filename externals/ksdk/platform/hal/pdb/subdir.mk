PDB_HAL_DIR := $(SDK_ROOT)/platform/hal/pdb
SOURCES += $(wildcard $(PDB_HAL_DIR)/*.c)
INCLUDES += $(PDB_HAL_DIR)
