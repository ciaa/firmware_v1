#ifndef USBD_STATES_H
#define USBD_STATES_H


typedef int (*usbd_state_fn)( usb_stack_t* pstack, uint8_t index );

int usbd_state_run( usb_stack_t* pstack, uint8_t index );


/*
 * State functions must follow the usb_dev_state_t enumeration order.
 *
 * Following functions do not assert input parameters, this is because they must
 * not be called directly, rather through usbd_state_run() so there's no need.
 */
int _usbd_state_waiting_ack( usb_stack_t* pstack, uint8_t index );
int _usbd_state_waiting_delay( usb_stack_t* pstack, uint8_t index );
int _usbd_state_disconnected( usb_stack_t* pstack, uint8_t index );
int _usbd_state_attached( usb_stack_t* pstack, uint8_t index );
int _usbd_state_powered( usb_stack_t* pstack, uint8_t index );
int _usbd_state_reset( usb_stack_t* pstack, uint8_t index );
int _usbd_state_default( usb_stack_t* pstack, uint8_t index );
int _usbd_state_address( usb_stack_t* pstack, uint8_t index );
int _usbd_state_configuring_pipes( usb_stack_t* pstack, uint8_t index );
int _usbd_state_dev_desc( usb_stack_t* pstack, uint8_t index );
int _usbd_state_dev_desc_len9( usb_stack_t* pstack, uint8_t index );
int _usbd_state_cfg_desc( usb_stack_t* pstack, uint8_t index );
int _usbd_state_unlocking( usb_stack_t* pstack, uint8_t index );
int _usbd_state_unlocking2( usb_stack_t* pstack, uint8_t index );
int _usbd_state_configured( usb_stack_t* pstack, uint8_t index );
int _usbd_state_suspended( usb_stack_t* pstack, uint8_t index );

#endif

