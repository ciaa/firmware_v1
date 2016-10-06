#!/bin/bash

if [ "$1" = "" ] ; then
    echo "use ${0}  root_dir"
    exit
fi


ROOT_DIR=$1
# TODO maybe validate ROOT_DIR
OLD_DIR=$PWD
DOWNLOAD_DIR="out/download"
LPCOPEN_ZIP="lpcopen_2_16_lpcxpresso_nxp_lpcxpresso_4337.zip"
LPCOPEN_URL="http://cache.nxp.com/files/microcontrollers/software/LPCWare/${LPCOPEN_ZIP}"
UNZIPPED_DIR="${DOWNLOAD_DIR}/lpcopen_2_16"
ROOT_EXAMPLE="${UNZIPPED_DIR}/lwip_tcpecho"
LWIPDIR="externals/lwip" 

# Going to RootDir
cd $ROOT_DIR # voy al raiz

# create download dir
mkdir -p $DOWNLOAD_DIR/lpcopen_2_16

# git ignoring download dir
cat > $DOWNLOAD_DIR/.gitignore << EOF
# Ignore everything in this directory
*
EOF

# Download LPCOpen
wget -c -O $DOWNLOAD_DIR/$LPCOPEN_ZIP $LPCOPEN_URL

# Unzip LPCOpen
unzip -q $DOWNLOAD_DIR/$LPCOPEN_ZIP  -d $UNZIPPED_DIR/


# Arch Files
cp -rf $ROOT_EXAMPLE/lwip/inc/arch $LWIPDIR/cortexM4/lpc43xx/lpc4337/inc/
cp -rf $ROOT_EXAMPLE/../lpc_board_nxp_lpcxpresso_4337/inc/lpc_phy.h \
	$LWIPDIR/cortexM4/lpc43xx/lpc4337/inc/

cp -rf $ROOT_EXAMPLE/lwip/src/arch $LWIPDIR/cortexM4/lpc43xx/lpc4337/src/
cp -rf $ROOT_EXAMPLE/../lpc_board_nxp_lpcxpresso_4337/src/lpc_phy_smsc87x0.c \
	$LWIPDIR/cortexM4/lpc43xx/lpc4337/src/arch/

# Needed files for blinking_lwip example
mkdir -p examples/blinking_lwip/inc/
cp -rf $ROOT_EXAMPLE/example/inc/*.h examples/blinking_lwip/inc/
cp -rf $ROOT_EXAMPLE/example/src/*.h examples/blinking_lwip/inc/

# arch patches
patch -p1 < $LWIPDIR/patches/0001_remove-board-h.patch
patch -p1 < $LWIPDIR/patches/0003_lpc18xx_43xx_emac.patch
patch -p1 < $LWIPDIR/patches/0004_lpc_phy.patch
patch -p1 < $LWIPDIR/patches/0005_lpc18xx_systick_arch.patch
# example patches
patch -p1 < $LWIPDIR/patches/0006_lpc_18xx43xx_emac_config.patch
patch -p1 < $LWIPDIR/patches/0007_lwipopts_sample.patch
patch -p1 < $LWIPDIR/patches/0008_lwipopts_comments.patch


# Returning to called directory
cd $OLD_DIR
