
void StartOs_Arch_SysTick(void)
{
   /* TODO see https://github.com/ciaa/Firmware/issues/66 */
/*	//Activate MemFault, UsageFault and BusFault exceptions
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk;

	//Activate SysTick
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000);*/
}
