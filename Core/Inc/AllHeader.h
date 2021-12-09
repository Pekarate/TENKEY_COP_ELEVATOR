
//#include	<p18f4580.h>
#include	"Dict.h"
#include	"CANOPEN.H"
#include 	"Bootloader.h"
#include	"Pic_Main.h"
#include	"Init.h"
#include	"CanBus.h"

#include	"EEPROM.h"
#include	"Subprog.h"
#include	"Uart.h"
#include 	"Version.h"
#include    "main.h"
#include 	"string.h"
#include 	"flash.h"

enum {
	CHAR_G =0,
	CHAR_B,
	CHAR_C,
	CHAR_M
};
