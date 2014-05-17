/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/** \brief FreeOSEK Os Generated Internal Achitecture Configuration Implementation File
 **
 ** \file Os_Internal_Arch_Cfg.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.2 MaCe rename file to Os_
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
<?php
foreach ($tasks as $task)
{
	print "/** \brief $task context */\n";
	print "TaskContextType TaskContext" . $task . ";\n";
}
print "\n";

?>

InterruptType InterruptTable[INTERUPTS_COUNT] =
{
<?php
$intnames = $config->getList("/OSEK","ISR");
for ($loopi = 0; $loopi < 32; $loopi++)
{
	if ($loopi<8)
	{
		switch($loopi)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				print "	OSEK_ISR_NoHandler, /* no interrupt handler for interrupt $loopi */\n";
				break;
			case 4:
				print "	OSEK_ISR_HWTimer0, /* HW Timer 0 Interrupt handler */\n";
				break;
			case 5:
				print "	OSEK_ISR_HWTimer1, /* HW Timer 1 Interrupt handler */\n";
				break;
			case 6:
			case 7:
				print "	OSEK_ISR_NoHandler, /* no interrupt handler for interrupt $loopi */\n";
				break;
		}
	}
	else
	{
		$flag = false;
		foreach ($intnames as $int)
		{
			$inttype = $config->getValue("/OSEK/" . $int,"INTERRUPT");
			$intcat = $config->getValue("/OSEK/" . $int,"CATEGORY");
			switch($inttype)
			{
				case "INT_CANRX":
					if($loopi == 8)
					{
						if($intcat == "1")
						{
							print "	OSEK_ISR_$int, /* interrupt handler $loopi */\n";
							$flag = true;
						}
						elseif($intcat == "2")
						{
							print "	OSEK_ISR2_$int, /* interrupt handler $loopi */\n";
							$flag = true;
						}
						else
						{
							error("Interrupt $int type $inttype has an invalid category $intcat");
						}
					}
					break;
				case "INT_CANTX":
					if($loopi == 9)
					{
						if($intcat == "1")
						{
							print "	OSEK_ISR_$int, /* interrupt handler $loopi */\n";
							$flag = true;
						}
						elseif($intcat == "2")
						{
							print "	OSEK_ISR2_$int, /* interrupt handler $loopi */\n";
							$flag = true;
						}
						else
						{
							error("Interrupt $int type $inttype has an invalid category $intcat");
						}
					}
					break;
			}
		}
		if ($flag == false)
		{
			print "	OSEK_ISR_NoHandler, /* no interrupt handler for interrupt $loopi */\n";
		}
	}
}
?>
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

