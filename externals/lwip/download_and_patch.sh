#!/bin/bash

if [ "$1" = "" ] ; then
    echo "use ${0}  dir_de_raiz"
    exit
fi


ROOT_DIR=$1
OLD_DIR=$PWD
ROOT_EXAMPLE="out/download/lpcopen_2_16/lwip_tcpecho"
LWIPDIR="externals/lwip" 

cd $ROOT_DIR # voy al raiz

mkdir -p out/download/lpcopen_2_16

cat > out/download/.gitignore << EOF
# Ignore everything in this directory
*
EOF

wget -c -Oout/download/lpcopen_2_16_lpcxpresso_nxp_lpcxpresso_4337.zip \
	http://cache.nxp.com/files/microcontrollers/software/LPCWare/lpcopen_2_16_lpcxpresso_nxp_lpcxpresso_4337.zip

unzip -q out/download/lpcopen_2_16_lpcxpresso_nxp_lpcxpresso_4337.zip \
	-d out/download/lpcopen_2_16/

mkdir -p $LWIPDIR/src/ $LWIPDIR/inc/

# FIXME tal vez la técnica del tar.
cp -rf $ROOT_EXAMPLE/lwip/src/* $LWIPDIR/src/
cp -rf $ROOT_EXAMPLE/lwip/inc/* $LWIPDIR/inc/

cp -rf $ROOT_EXAMPLE/../lpc_board_nxp_lpcxpresso_4337/inc/lpc_phy.h \
	$LWIPDIR/inc/arch/
cp -rf $ROOT_EXAMPLE/../lpc_board_nxp_lpcxpresso_4337/src/lpc_phy_smsc87x0.c \
	$LWIPDIR/src/arch/

mkdir -p examples/blinking_lwip/inc/
cp -rf $ROOT_EXAMPLE/example/inc/*.h examples/blinking_lwip/inc/
cp -rf $ROOT_EXAMPLE/example/src/*.h examples/blinking_lwip/inc/


patch -p1 < $LWIPDIR/patches/0001-remove-board-h.patch
patch -p1 < $LWIPDIR/patches/0002-lpc-head.patch
patch -p1 < $LWIPDIR/patches/0003-lpc18xx_43xx_emac.patch
patch -p1 < $LWIPDIR/patches/0004-lpc_phy.patch
patch -p1 < $LWIPDIR/patches/0005_lpc18xx_systick_arch.patch
patch -p1 < $LWIPDIR/patches/0006_lpc_18xx43xx_emac_config.patch
patch -p1 < $LWIPDIR/patches/0007_lwipopts_sample.patch


cd $OLD_DIR
