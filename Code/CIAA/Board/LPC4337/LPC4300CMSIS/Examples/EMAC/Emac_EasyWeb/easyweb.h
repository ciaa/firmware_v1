/******************************************************************
 *****                                                        *****
 *****  Name: easyweb.h                                       *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 07/05/2001                                      *****
 *****  Auth: Andreas Dannenberg                              *****
 *****        HTWK Leipzig                                    *****
 *****        university of applied sciences                  *****
 *****        Germany                                         *****
 *****  Func: header-file for easyweb.c                       *****
 *****                                                        *****
 ******************************************************************/

#ifndef __EASYWEB_H
#define __EASYWEB_H

const unsigned char GetResponse[] =              // 1st thing our server sends to a client
{
  "HTTP/1.0 200 OK\r\n"                          // protocol ver 1.0, code 200, reason OK
  "Content-Type: text/html\r\n"                  // type of data we want to send
  "\r\n"                                         // indicate end of HTTP-header
};

void InitOsc(void);                              // prototypes
void InitPorts(void);
void HTTPServer(void);
void InsertDynamicValues(void);
unsigned int GetAD7Val(void);
unsigned int GetTempVal(void);

unsigned char *PWebSide;                         // pointer to webside
unsigned int HTTPBytesToSend;                    // bytes left to send

unsigned char HTTPStatus;                        // status byte 
#define HTTP_SEND_PAGE               0x01        // help flag

#endif

