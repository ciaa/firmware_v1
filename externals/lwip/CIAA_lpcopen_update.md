# LWIP for Computadora Industrial Abierta Argentina

LWIP drivers has been downloaded from [LPCOpen v2.16 LPCXpresso](http://cache.nxp.com/files/microcontrollers/software/LPCWare/lpcopen_2_16_lpcxpresso_nxp_lpcxpresso_4337.zip) an then patched.

The LPCOpen v2.16 LPCXpresso can be downloaded from [LPCOPEN-SOFTWARE-FOR-LPC43XX: LPCOpen Software Development Platform LPC43XX](http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/lpc-cortex-m-mcus/lpc-cortex-m3/lpc1800-cortex-m3/lpcopen-software-development-platform-lpc43xx:LPCOPEN-SOFTWARE-FOR-LPC43XX?code=LPCOPEN-SOFTWARE-FOR-LPC43XX&nodeId=0150522FC850404BF04A3A&fpsp=1&tab=Design_Tools_Tab)

LWIP is a submodule from official repository.

At the moment IPv6 and ppp files has not been tested. Only LWIP version 1.4.1 runs without problems.

The copyright for each file taken is wrote in the sourcecode file.

There is a script whichs downloads lpcopen, copy files and apply patches attached in patches directory.

The script may be called typing `bash download_and_patch.sh root_firmware_dir` and could be called via some Makefile taget.


## Copied files

See the script sourcecode and its comments.

## Patchs

see `patches/`

## Updates

After update lpcopen pay attention to the patches and apply them. Test before commiting. The script has its own comments for better understanding.

Keep mak dir as cames inside `externals/lwip`.
