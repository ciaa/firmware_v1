#ifndef __SLAVE_IPC_MSG_H 
#define __SLAVE_IPC_MSG_H

/* commands the slave shall get */
enum ipcSlaveMsg_tag {

	CMD_SLAVE_NONE = 0,
	PRINT_WELCOME_MESSAGE,
	DATA_RESULT,
	PRINT_NUM_MESSAGES
		
};

#endif
