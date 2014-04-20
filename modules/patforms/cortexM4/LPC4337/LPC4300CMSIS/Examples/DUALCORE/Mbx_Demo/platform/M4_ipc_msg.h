#ifndef __M4_IPC_MSG_H 
#define __M4_IPC_MSG_H

/* messages the M4 shall get */
typedef enum ipcM4Msg_tag {

	CMD_M4_NONE = 0,
	NOTIFY_M0_STARTED,
	REQUEST_PROCESS_DATA
		
}  ipcM4Msg_t;


#endif
