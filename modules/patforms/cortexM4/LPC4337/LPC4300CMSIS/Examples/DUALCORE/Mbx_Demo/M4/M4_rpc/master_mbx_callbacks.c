#include <stdint.h>
#include "lpc_types.h"
#include "ipc_mbx.h"
#include "master_mbx_callbacks.h"

/* ordering of function table items is not important  */
CbackItem Master_CbackTable[NUM_MASTER_MBX] __attribute__((section("Master_callbackTable"))) = {

		{ MASTER_MBX_TASKA, (mbxCallback_t) &masterCbackA},
		{ MASTER_MBX_TASKB, (mbxCallback_t) &masterCbackB},
		{ MASTER_MBX_TASKC, (mbxCallback_t) &masterCbackC},
		{ MASTER_MBX_CMD, (mbxCallback_t) &masterCbackCmd}
};


// command function skeleton 
void masterCbackCmd(msg_t msg, msgId_t idNum, mbxParam_t parameter) { }

// processing function skeletons 
void masterCbackA(msg_t msg, msgId_t idNum, mbxParam_t parameter) { }
void masterCbackB(msg_t msg, msgId_t idNum, mbxParam_t parameter)	{ }
void masterCbackC(msg_t msg, msgId_t idNum, mbxParam_t parameter) { }

