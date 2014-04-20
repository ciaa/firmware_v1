#include <stdint.h>
//#include "type.h"
#include "ipc_mbx.h"


/* ordering of function table items is not important  */
CbackItem Slave_CbackTable[NUM_SLAVE_MBX] __attribute__((section("Slave_callbackTable"))) = {

		{ SLAVE_MBX_TASKD, (mbxCallback_t) &slaveCbackD},
		{ SLAVE_MBX_TASKE, (mbxCallback_t) &slaveCbackE},
		{ SLAVE_MBX_TASKF, (mbxCallback_t) &slaveCbackF},
		{ SLAVE_MBX_CMD,  (mbxCallback_t) &slaveCbackCmd}
};



// command function skeleton with parameter
void slaveCbackCmd(msg_t msg, msgId_t idNum, mbxParam_t parameter) { }

// processing function skeletons with parameter
void slaveCbackD (msg_t msg, msgId_t idNum, mbxParam_t parameter) { }
void slaveCbackE (msg_t msg, msgId_t idNum, mbxParam_t parameter) { }
void slaveCbackF (msg_t msg, msgId_t idNum, mbxParam_t parameter) { }






