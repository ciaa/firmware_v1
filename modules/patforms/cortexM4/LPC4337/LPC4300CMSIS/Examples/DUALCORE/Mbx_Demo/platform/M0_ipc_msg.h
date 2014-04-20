#ifndef __M0_IPC_MSG_H 
#define __M0_IPC_MSG_H

/* messages the M0 shall get */
typedef enum ipcMsg_tag {

	CMD_M0_NONE = 0,
	PRINT_WELCOME_MESSAGE,
	DATA_RESULT,
	PRINT_NUM_MESSAGES
		
}  ipcM0Msg_t;

#endif
