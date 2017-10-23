/* Copyright 2016, Gerardo Puga (UNLP)
 * Copyright 2014, Pablo Ridolfi (UTN-FRBA)
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief FreeOSEK Os Internal Arch Implementation File
 **
 ** \file sparcV8/grlib.c
 ** \arch sparcV8
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*==================[inclusions]=============================================*/


#include "sparcsyscalls.h"
#include "grlib.h"


/*==================[macros and definitions]=================================*/

/**
 * \brief AMBA AHB Master devices Plug&Play table base address.
 */
#define GAISLER_PNP_AHB_DEVICE_TABLE_ADDRESS_MASTERS 0xFFFFF000

/**
 * \brief AMBA AHB Slave devices Plug&Play table base address.
 */
#define GAISLER_PNP_AHB_DEVICE_TABLE_ADDRESS_SLAVES  0xFFFFF800


/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/


/**
 * \brief Shadow variable with the value of the system frequency.
 *
 * This is handled internally. It is set by the operating system after having autodetected
 * the system frequency during OS initialization, and is later accessed (through external module
 * services).
 */
int32_t  grSystemFrequencyValueInHz = -1;


/*==================[external data definition]===============================*/


/*==================[internal functions definition]==========================*/


/*==================[external functions definition]==========================*/


/**
 * \brief Read AMBA AHB Master information on the Plug&Play device information
 * table on the system.
 *
 * The user provides a set of filters, and the function returns the data of a device
 * that matches the provided data filters (vendor, device id, repetition order, etc.)
 *
 * @param requestedVendorId A GRLIB device vendor ID or GRLIB_PNP_VENDOR_ID_ANY to match with any vendor.
 * @param requestedDeviceId A GRLIB device ID or GRLIB_PNP_DEVICE_ID_ANY if ID to match with any device id.
 * @param ahbDeviceInfo Pointer to a AHB device information data block, where the device information will be returned (if a matching device was found).
 * @param ahbDeviceIndex Number of device repetition to return. If index=0, the first device matching the filters will be returned, if index=1 the second, if index=2 the third...
 * @return 0 if a matching device was found, -1 otherwise.
 */
int32_t  grWalkPlugAndPlayAHBMastersDeviceTable(uint32_t requestedVendorId, uint32_t requestedDeviceId, grPlugAndPlayAHBDeviceTableEntryType *ahbDeviceInfo, int32_t  ahbDeviceIndex)
{
   uint32_t *deviceTablePtr;

   uint32_t deviceTableIndex;
   uint32_t barEntryIndex;

   uint32_t *barTablePtr;

   uint32_t idRegisterValue;
   uint32_t barEntryValue;

   uint32_t idRegisterFieldVendorId;
   uint32_t idRegisterFieldDeviceId;
   uint32_t idRegisterFieldVersion;
   uint32_t idRegisterFieldIrq;

   uint32_t deviceFound;
   int32_t  returnValue;

   /* default to "device not found" */
   returnValue = -1;

   /* starting address of the PnP AHB device table */
   deviceTablePtr = (uint32_t *)GAISLER_PNP_AHB_DEVICE_TABLE_ADDRESS_MASTERS;

   deviceTableIndex = 0;
   deviceFound = 0;

   /* for each entry on the PNP AHB device table, decode the id register
    * and verify if the vendor/device pair matches the filter values
    * provided as arguments to the function. */
   for (deviceTableIndex = 0; deviceTableIndex < 64; deviceTableIndex++)
   {
      idRegisterValue = *(deviceTablePtr + 0);
      barTablePtr = deviceTablePtr + 4;

      /* extract the id register fields */
      idRegisterFieldVendorId = (idRegisterValue >> 24) & 0x000000ff;
      idRegisterFieldDeviceId = (idRegisterValue >> 12) & 0x00000fff;
      idRegisterFieldVersion  = (idRegisterValue >>  5) & 0x0000001f;
      idRegisterFieldIrq      = (idRegisterValue >>  0) & 0x0000001f;

      /* Have we found the end of the table? */
      if (idRegisterFieldVendorId == 0x00)
      {
         break;
      }

      /* Does it match the filter we were given? */
      if (((requestedVendorId == GRLIB_PNP_VENDOR_ID_ANY) || (requestedVendorId == idRegisterFieldVendorId))
            && ((requestedDeviceId == GRLIB_PNP_DEVICE_ID_ANY) || (requestedDeviceId == idRegisterFieldDeviceId)))
      {
         /* Is this the n-th match of the filter? */
         ahbDeviceIndex--;

         if (ahbDeviceIndex < 0)
         {
            deviceFound = 1;
            break;
         }
      }

      /* move the pointer to the next entry on the table */
      deviceTablePtr = deviceTablePtr + 8;
   }

   if (deviceFound != 0)
   {
      ahbDeviceInfo->vendorId = idRegisterFieldVendorId;
      ahbDeviceInfo->deviceId = idRegisterFieldDeviceId;
      ahbDeviceInfo->version  = idRegisterFieldVersion;
      ahbDeviceInfo->irq      = idRegisterFieldIrq;

      for (barEntryIndex = 0; barEntryIndex < 4; barEntryIndex++)
      {
         barEntryValue = barTablePtr[barEntryIndex];

         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address          = (barEntryValue >> 20) & 0x00000fff;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].prefetchableFlag = (barEntryValue >> 17) & 0x00000001;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].cacheableFlag    = (barEntryValue >> 16) & 0x00000001;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask             = (barEntryValue >>  4) & 0x00000fff;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].type             = (barEntryValue >>  0) & 0x0000000f;

         /* Only entries with MASK != 0 contain actual data */
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].validEntry       = (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask != 0)?1:0;

         /* the interpretation of the address and mask fields changes depending on the value of the type field */
         switch (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].type ) {
         case GRLIB_PNP_BAR_ENTRY_TYPE_AHBMEMORYSPACE :
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address = ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address << 20;
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask    = ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask << 20;
            break;
         case GRLIB_PNP_BAR_ENTRY_TYPE_AHBIOSPACE :
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address = (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address << 8) | 0xfff00000;
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask    = (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask << 8) | 0xfff00000;
            break;
         case GRLIB_PNP_BAR_ENTRY_TYPE_APBIOSPACE :
            /* this type of entry should not appear on the AHB table... */
            sparcAssert(0, "Impossible BAR table entry type!");
            break;
         }
      }

      returnValue = 0;
   }

   return returnValue;
}


/**
 * \brief Read AMBA AHB Slaves information on the Plug&Play device information
 * table on the system.
 *
 * The user provides a set of filters, and the function returns the data of a device
 * that matches the provided data filters (vendor, device id, repetition order, etc.)
 *
 * @param requestedVendorId A GRLIB device vendor ID or GRLIB_PNP_VENDOR_ID_ANY to match with any vendor.
 * @param requestedDeviceId A GRLIB device ID or GRLIB_PNP_DEVICE_ID_ANY if ID to match with any device id.
 * @param ahbDeviceInfo Pointer to a AHB device information data block, where the device information will be returned (if a matching device was found).
 * @param ahbDeviceIndex Number of device repetition to return. If index=0, the first device matching the filters will be returned, if index=1 the second, if index=2 the third...
 * @return 0 if a matching device was found, -1 otherwise.
 */
int32_t  grWalkPlugAndPlayAHBSlavesDeviceTable(uint32_t requestedVendorId, uint32_t requestedDeviceId, grPlugAndPlayAHBDeviceTableEntryType *ahbDeviceInfo, int32_t  ahbDeviceIndex)
{
   uint32_t *deviceTablePtr;

   uint32_t deviceTableIndex;
   uint32_t barEntryIndex;

   uint32_t *barTablePtr;

   uint32_t idRegisterValue;
   uint32_t barEntryValue;

   uint32_t idRegisterFieldVendorId;
   uint32_t idRegisterFieldDeviceId;
   uint32_t idRegisterFieldVersion;
   uint32_t idRegisterFieldIrq;

   uint32_t deviceFound;
   int32_t  returnValue;

   /* default to "device not found" */
   returnValue = -1;

   /* starting address of the PnP AHB device table */
   deviceTablePtr = (uint32_t *)GAISLER_PNP_AHB_DEVICE_TABLE_ADDRESS_SLAVES;

   deviceTableIndex = 0;
   deviceFound = 0;

   /* for each entry on the PNP AHB device table, decode the id register
    * and verify if the vendor/device pair matches the filter values
    * provided as arguments to the function. */
   for (deviceTableIndex = 0; deviceTableIndex < 63; deviceTableIndex++)
   {
      idRegisterValue = *(deviceTablePtr + 0);
      barTablePtr = deviceTablePtr + 4;

      /* extract the id register fields */
      idRegisterFieldVendorId = (idRegisterValue >> 24) & 0x000000ff;
      idRegisterFieldDeviceId = (idRegisterValue >> 12) & 0x00000fff;
      idRegisterFieldVersion  = (idRegisterValue >>  5) & 0x0000001f;
      idRegisterFieldIrq      = (idRegisterValue >>  0) & 0x0000001f;

      /* Have we found the end of the table? */
      if (idRegisterFieldVendorId == 0x00)
      {
         break;
      }

      /* Does it match the filter we were given? */
      if (((requestedVendorId == GRLIB_PNP_VENDOR_ID_ANY) || (requestedVendorId == idRegisterFieldVendorId))
            && ((requestedDeviceId == GRLIB_PNP_DEVICE_ID_ANY) || (requestedDeviceId == idRegisterFieldDeviceId)))
      {
         /* Is this the n-th match of the filter? */
         ahbDeviceIndex--;

         if (ahbDeviceIndex < 0)
         {
            deviceFound = 1;
            break;
         }
      }

      /* move the pointer to the next entry on the table */
      deviceTablePtr = deviceTablePtr + 8;
   }

   if (deviceFound != 0)
   {
      ahbDeviceInfo->vendorId = idRegisterFieldVendorId;
      ahbDeviceInfo->deviceId = idRegisterFieldDeviceId;
      ahbDeviceInfo->version  = idRegisterFieldVersion;
      ahbDeviceInfo->irq      = idRegisterFieldIrq;

      for (barEntryIndex = 0; barEntryIndex < 4; barEntryIndex++)
      {
         barEntryValue = barTablePtr[barEntryIndex];

         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address          = (barEntryValue >> 20) & 0x00000fff;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].prefetchableFlag = (barEntryValue >> 17) & 0x00000001;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].cacheableFlag    = (barEntryValue >> 16) & 0x00000001;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask             = (barEntryValue >>  4) & 0x00000fff;
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].type             = (barEntryValue >>  0) & 0x0000000f;

         /* Only entries with MASK != 0 contain actual data */
         ahbDeviceInfo->bankAddressRegisters[barEntryIndex].validEntry       = (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask != 0)?1:0;

         /* the interpretation of the address and mask fields changes depending on the value of the type field */
         switch (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].type ) {
         case GRLIB_PNP_BAR_ENTRY_TYPE_AHBMEMORYSPACE :
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address = ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address << 20;
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask    = ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask << 20;
            break;
         case GRLIB_PNP_BAR_ENTRY_TYPE_AHBIOSPACE :
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address = (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].address << 8) | 0xfff00000;
            ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask    = (ahbDeviceInfo->bankAddressRegisters[barEntryIndex].mask << 8) | 0xfff00000;
            break;
         case GRLIB_PNP_BAR_ENTRY_TYPE_APBIOSPACE :
            /* this type of entry should not appear on the AHB table... */
            sparcAssert(0, "Impossible BAR table entry type!");
            break;
         }
      }

      returnValue = 0;
   }

   return returnValue;
}

/**
 * \brief Read AMBA APB device information from any and all APB master devices on the system.
 *
 * The user provides a set of filters, and the function returns the data of a device
 * that matches the provided data filters (vendor, device id, repetition order, etc.)
 *
 * The routine sequentially searches through all of the APB device tables on the system, on the same
 * order in which their masters are found on the AHB Slave device information table on the main AHB bus.
 *
 * @param requestedVendorId A GRLIB device vendor ID or GRLIB_PNP_VENDOR_ID_ANY to match with any vendor.
 * @param requestedDeviceId A GRLIB device ID or GRLIB_PNP_DEVICE_ID_ANY if ID to match with any device id.
 * @param apbDeviceInfo Pointer to a AHB device information data block, where the device information will be returned (if a matching device was found).
 * @param apbDeviceIndex Number of device repetition to return. If index=0, the first device matching the filters will be returned, if index=1 the second, if index=2 the third...
 * @return 0 if a matching device was found, -1 otherwise.
 */
int32_t  grWalkPlugAndPlayAPBDeviceTable(uint32_t requestedVendorId, uint32_t requesteDeviceId, grPlugAndPlayAPBDeviceTableEntryType *apbDeviceInfo, int32_t  apbDeviceIndex)
{
   grPlugAndPlayAHBDeviceTableEntryType apbCtrlDeviceConfiguration;

   uint32_t *apbDeviceTablePtr;

   uint32_t ahbBridgeIndex;
   uint32_t barTableIndex;
   uint32_t deviceTableIndex;

   uint32_t idRegisterValue;
   uint32_t barRegisterValue;

   uint32_t ahbRangeAddress;
   uint32_t ahbRangeMask;

   uint32_t idRegisterFieldVendorId;
   uint32_t idRegisterFieldDeviceId;
   uint32_t idRegisterFieldCtFlag;
   uint32_t idRegisterFieldVersion;
   uint32_t idRegisterFieldIrq;

   uint32_t deviceFound;
   int32_t  returnValue;

   /* default to "device not found" */
   returnValue = -1;
   deviceFound = 0;

   ahbBridgeIndex = 0;

   deviceTableIndex = 0;

   while(grWalkPlugAndPlayAHBSlavesDeviceTable(
         GRLIB_PNP_VENDOR_ID_GAISLER_RESEARCH,
         GRLIB_PNP_DEVICE_ID_APBCTRL,
         &apbCtrlDeviceConfiguration,
         ahbBridgeIndex) >= 0)
   {
      ahbBridgeIndex++;

      /* the for loop is probably an overkill, since the APBCTRL core probably never has more
       * than a single BAR table entry, but since the documentation doesn't really say so,
       * it's better to fail on the safe side. */
      for (barTableIndex = 0; barTableIndex < 4; barTableIndex++)
      {
         /* if this entry is not valid or if it is not a memory space entry, skip it */
         if ((apbCtrlDeviceConfiguration.bankAddressRegisters[barTableIndex].validEntry == 0) ||
               (apbCtrlDeviceConfiguration.bankAddressRegisters[barTableIndex].type != GRLIB_PNP_BAR_ENTRY_TYPE_AHBMEMORYSPACE))
         {
            continue;
         }

         /* Save these. We'll need them if we eventually find the APB device that was asked for */
         ahbRangeAddress = apbCtrlDeviceConfiguration.bankAddressRegisters[barTableIndex].address;
         ahbRangeMask = apbCtrlDeviceConfiguration.bankAddressRegisters[barTableIndex].mask;

         /* construct the address of the APB PnP table located at the end of the
          * memory range */
         apbDeviceTablePtr = (uint32_t *)(ahbRangeAddress | 0x000ff000);

         /* for each entry on the AHB PnP device table, decode the id register
          * and check whether the vendor/device pair matches the filter values
          * provided as arguments to the function. */
         for (deviceTableIndex = 0; deviceTableIndex < 512; deviceTableIndex++)
         {
            idRegisterValue  = *apbDeviceTablePtr;
            barRegisterValue = *(apbDeviceTablePtr+1);

            /* extract the id register fields */
            idRegisterFieldVendorId = (idRegisterValue >> 24) & 0x000000ff;
            idRegisterFieldDeviceId = (idRegisterValue >> 12) & 0x00000fff;
            idRegisterFieldCtFlag   = (idRegisterValue >> 10) & 0x00000003;
            idRegisterFieldVersion  = (idRegisterValue >>  5) & 0x0000001f;
            idRegisterFieldIrq      = (idRegisterValue >>  0) & 0x0000001f;

            /* Have we found the end of the table? */
            if (idRegisterFieldVendorId == 0x00)
            {
               break;
            }

            /* Does it match the filter we were given? */
            if (((requestedVendorId == GRLIB_PNP_VENDOR_ID_ANY) || (requestedVendorId == idRegisterFieldVendorId))
                  && ((requesteDeviceId == GRLIB_PNP_DEVICE_ID_ANY) || (requesteDeviceId == idRegisterFieldDeviceId)))
            {
               /* Is this the n-th match of the filter? */
               apbDeviceIndex--;

               if (apbDeviceIndex < 0)
               {
                  deviceFound = 1;
                  break;
               }
            }

            /* move the pointer to the next entry on the table */
            apbDeviceTablePtr = apbDeviceTablePtr + 2;
         }

         /* If we exited the inner loop having found the requested APB
          * device, then get out of this loop too */
         if (deviceFound != 0)
         {
            break;
         }
      }

      /* If we exited the inner loop having found the requested APB
       * device, then get out of this loop too */
      if (deviceFound != 0)
      {
         break;
      }
   }

   if (deviceFound != 0)
   {
      apbDeviceInfo->vendorId = idRegisterFieldVendorId;
      apbDeviceInfo->deviceId = idRegisterFieldDeviceId;
      apbDeviceInfo->ctFlag   = idRegisterFieldCtFlag;
      apbDeviceInfo->version  = idRegisterFieldVersion;
      apbDeviceInfo->irq      = idRegisterFieldIrq;

      apbDeviceInfo->address  = (barRegisterValue >> 20) & 0x00000fff;
      apbDeviceInfo->mask     = (barRegisterValue >>  4) & 0x00000fff;
      apbDeviceInfo->type     = (barRegisterValue >>  0) & 0x0000000f;

      /* the interpretation of the address and mask fields changes depending on the value of the type field */
      switch (apbDeviceInfo->type ) {
      case GRLIB_PNP_BAR_ENTRY_TYPE_AHBMEMORYSPACE :
         /* this type of entry should not appear on the APB table... */
         sparcAssert(0, "Impossible APB device type!");
         break;
      case GRLIB_PNP_BAR_ENTRY_TYPE_AHBIOSPACE :
         /* this type of entry should not appear on the APB table... */
         sparcAssert(0, "Impossible APB device type!");
         break;
      case GRLIB_PNP_BAR_ENTRY_TYPE_APBIOSPACE :
         apbDeviceInfo->address = ahbRangeAddress | (apbDeviceInfo->address << 8);
         apbDeviceInfo->mask    = ahbRangeMask | (apbDeviceInfo->mask << 8);
         break;
      }

      returnValue = 0;
   }

   return returnValue;
}


/**
 * \brief Accesses a memory-mapped device register for writing.
 *
 * @param baseAddr Base address of the memory-mapped device.
 * @param offset   Relative offset of the register to access.
 * @param newValue Value that is to be written on the memory-mapped register.
 */
void grRegisterWrite(grDeviceAddress baseAddr, grDeviceAddress offset, grDeviceRegisterValue newValue)
{
   grDeviceRegisterValue *registerPtr;

   sparcAssert(((baseAddr + offset) & 0x03) == 0x00, "Register address not word aligned!");

   registerPtr = (grDeviceRegisterValue *)(baseAddr + offset);

   *registerPtr = newValue;
}

/**
 * \brief Accesses a memory-mapped device register for reading.
 *
 * @param baseAddr Base address of the memory-mapped device.
 * @param offset   Relative offset of the register to access.
 * return          The value read from the memory-mapped register.
 */
grDeviceRegisterValue grRegisterRead(grDeviceAddress baseAddr, grDeviceAddress offset)
{
   grDeviceRegisterValue *registerPtr;

   sparcAssert(((baseAddr + offset) & 0x03) == 0x00, "Register address not word aligned!");

   registerPtr = (grDeviceRegisterValue *)(baseAddr + offset);

   return (grDeviceRegisterValue)(*registerPtr);
}


/**
 * \brief Returns internally stored system frequency information.
 *
 * TODO this is an ugly hack to allow the drivers modules to determine the system frequency without having to
 * duplicate the autodetection code there that is already performed by the operating system, but this solution
 * is not really elegant and should probably be replaced with some other method of communication between
 * rtos and drivers.
 *
 * @return  The frequency value, in hertz.
 */
int32_t  grGetSystemClockFrequencyValue()
{
   return grSystemFrequencyValueInHz;
}

/**
 * \brief Sets the internally stored information about the system clock frequency.
 *
 * This must be called by the operating system during system initialization.
 *
 * TODO this is an ugly hack to allow the drivers modules to determine the system frequency without having to
 * duplicate the autodetection code there that is already performed by the operating system, but this solution
 * is not really elegant and should probably be replaced with some other method of communication between
 * rtos and drivers.
 *
 * @return  The frequency value, in hertz.
 */
void grSetSystemClockFrequencyValue(int32_t  systemFrequencyInHz)
{
   grSystemFrequencyValueInHz = systemFrequencyInHz;
}

/**
 * \brief Enables LEON3 data and instruction caches.
 *
 * This is something that needs to be done during system boot, because by default both caches are disabled
 * after power-on.
 */
void grEnableProcessorCaches()
{
   asm ( "flush\n\t"
         "set 0x81000f, %%g1\n\t"
         "sta %%g1, [%%g0] 2\n\t"
         : /* no output registers */ : /* no input registers */ : "%g1" /* clobbered registers */ );
}

/**
 * \brief Read the LEON3 CPU synthesis time configuration from a hidden system register.
 *
 * @param Pointer to the grCpuConfigType data struct where the configuration data will be returned.
 */
void grGetCPUConfig(grCpuConfigType *config)
{
   uint32_t cpuConfig;

   __asm__(
         "rd %%asr17,%0\n "
         : "=r" (cpuConfig) );

   config->clockSwitchingEnabled   = (cpuConfig >> 17) & 0x0001;
   config->cpuClockFrequency       = (cpuConfig >> 15) & 0x0003;
   config->loadDelay               = (cpuConfig >> 12) & 0x0001;
   config->fpuOption               = (cpuConfig >> 10) & 0x0003;
   config->macInstructionAvailable = (cpuConfig >>  9) & 0x0001;
   config->sparcV8MulDivAvailable  = (cpuConfig >>  8) & 0x0001;
   config->registersWindows        = (cpuConfig >>  0) & 0x001f;

   config->cpuClockFrequency       = config->cpuClockFrequency + 1;
   config->registersWindows        = config->registersWindows + 1;
}


/**
 * \brief Read the LEON3 instruction cache synthesis time configuration from a hidden system register.
 *
 * @param Pointer to the grCacheConfigType data struct where the configuration data will be returned.
 */
void grGetICacheConfig(grCacheConfigType *config)
{
   uint32_t instructionCacheConfig;

   __asm__(
         "mov 0x08,%%g1\n\t"
         "lda [%%g1]2,%0\n\t"
         : "=r" (instructionCacheConfig) : /* no input registers */ :"g1");

   config->lockingImplemented   = (instructionCacheConfig >> 31) & 0x0001;
   config->replacementPolicy    = (instructionCacheConfig >> 28) & 0x0003;
   config->snoopingImplemented  = (instructionCacheConfig >> 27) & 0x0001;
   config->associativity        = (instructionCacheConfig >> 24) & 0x0007;
   config->setSize              = (instructionCacheConfig >> 20) & 0x000f;
   config->localRamImplemented  = (instructionCacheConfig >> 19) & 0x0001;
   config->lineSize             = (instructionCacheConfig >> 16) & 0x0007;
   config->localRamsize         = (instructionCacheConfig >> 12) & 0x0007;
   config->localRamStartAddress = (instructionCacheConfig >>  4) & 0x00ff;
   config->mmuPresent           = (instructionCacheConfig >>  3) & 0x0001;

   config->setSize              = 1 << config->setSize;
   config->lineSize             = 1 << config->lineSize;
   config->localRamsize         = 1 << config->localRamsize;
   config->localRamStartAddress = config->localRamStartAddress << 24;

   if (config->localRamImplemented == 0)
   {
      config->localRamsize         = 0;
      config->localRamStartAddress = 0;
   }
}

/**
 * \brief Read the LEON3 data cache synthesis time configuration from a hidden system register.
 *
 * @param Pointer to the grCacheConfigType data struct where the configuration data will be returned.
 */
void grGetDCacheConfig(grCacheConfigType *config)
{
   uint32_t dataCacheConfig;

   __asm__(
         "mov 0x0c,%%g1\n\t"
         "lda [%%g1]2,%0\n\t"
         : "=r" (dataCacheConfig) : /* no input registers */ : "g1");

   config->lockingImplemented   = (dataCacheConfig >> 31) & 0x0001;
   config->replacementPolicy    = (dataCacheConfig >> 28) & 0x0003;
   config->snoopingImplemented  = (dataCacheConfig >> 27) & 0x0001;
   config->associativity        = (dataCacheConfig >> 24) & 0x0007;
   config->setSize              = (dataCacheConfig >> 20) & 0x000f;
   config->localRamImplemented  = (dataCacheConfig >> 19) & 0x0001;
   config->lineSize             = (dataCacheConfig >> 16) & 0x0007;
   config->localRamsize         = (dataCacheConfig >> 12) & 0x0007;
   config->localRamStartAddress = (dataCacheConfig >>  4) & 0x00ff;
   config->mmuPresent           = (dataCacheConfig >>  3) & 0x0001;

   config->setSize              = 1 << config->setSize;
   config->lineSize             = 1 << config->lineSize;
   config->localRamsize         = 1 << config->localRamsize;
   config->localRamStartAddress = config->localRamStartAddress << 24;

   if (config->localRamImplemented == 0)
   {
      config->localRamsize         = 0;
      config->localRamStartAddress = 0;
   }
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

