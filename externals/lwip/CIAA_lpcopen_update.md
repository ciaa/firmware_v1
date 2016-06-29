# LWIP for Computadora Industrial Abierta Argentina

This LWIP hace been downloade from [LPCOpen v2.16 LPCXpresso](http://cache.nxp.com/files/microcontrollers/software/LPCWare/lpcopen_2_16_lpcxpresso_nxp_lpcxpresso_4337.zip) an then patched.

The LPCOpen v2.16 LPCXpresso can be download from [LPCOPEN-SOFTWARE-FOR-LPC43XX: LPCOpen Software Development Platform LPC43XX](http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/lpc-cortex-m-mcus/lpc-cortex-m3/lpc1800-cortex-m3/lpcopen-software-development-platform-lpc43xx:LPCOPEN-SOFTWARE-FOR-LPC43XX?code=LPCOPEN-SOFTWARE-FOR-LPC43XX&nodeId=0150522FC850404BF04A3A&fpsp=1&tab=Design_Tools_Tab)

The copyright for each file taken is wrote in the sourcecode file.

There is a script whichs downloads lpcopen, copy files and apply patches attached in patches directory.

The script may be called typing `bash download_and_patch.sh root_firmware_dir` and could be called via some Makefile taget.


## Copied files

The files `lwip_tcpecho/example/inc/lpc_18xx43xx_emac_config.h` and `lwip_tcpecho/example/inc/lwipopts.h` has been copied to `examples/blinking_lwip/inc` directory.

The files `lpc_board_nxp_lpcxpresso_4337/inc/lpc_phy.h` and `lpc_board_nxp_lpcxpresso_4337/src/lpc_phy_smsc87x0.c` has been copied to `externals/lwip/inc/arch/`

## Patchs

see `patches/`

## Actualizaciones

After update lpcopen pay attention to the patches and apply them. Test before mmaking a merge request.

Keep mak dir as cames inside `externals/lwip`.
