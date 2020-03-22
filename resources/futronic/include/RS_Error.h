/**
 *  Error Codes
 *
 *  @author jhmoon@suprema.co.kr
 *  @see    
 */

 /*  
 *  Copyright (c) 2012 Suprema Inc. All Rights Reserved.
 * 
 *  This software is the confidential and proprietary information of 
 *  Suprema Co., Ltd. ("Confidential Information").  You shall not
 *  disclose such Confidential Information and shall use it only in
 *  accordance with the terms of the license agreement you entered into
 *  with Suprema.
 */

#ifndef __RS_ERROR_H__
#define __RS_ERROR_H__

typedef enum {
	RS_SUCCESS	= 0,
	
	//
	// Generic errors
	//
	RS_ERR_SDK_UNINITIALIZED			= -10,
	RS_ERR_SDK_ALREADY_INITIALIZED		= -11,
	RS_ERR_INSUFFICIENT_HARDWARE		= -12,
	RS_ERR_INVALID_PARAM				= -13,
	RS_ERR_MEM_FULL						= -14,
	RS_ERR_NOT_YET_SUPPORTED			= -15,
	RS_ERR_CANNOT_OPEN_FILE				= -16,
	RS_ERR_CANNOT_READ_FILE				= -17,
	RS_ERR_CANNOT_WRITE_FILE			= -18,

	//
	// Device related errors
	//
	RS_ERR_NO_DEVICE					= -100,
	RS_ERR_INVALID_DEVICE_INDEX			= -101,
	RS_ERR_INVALID_HANDLE				= -102,
	RS_ERR_CANNOT_INIT_DEVICE			= -103,
	RS_ERR_UNSUPPORTED_DEVICE			= -104,
	RS_ERR_CANNOT_GET_USB_DEVICE		= -105,
	RS_ERR_DEVICE_ALREADY_INITIALIZED 	= -106,
	RS_ERR_CANNOT_OPEN_DEVICE			= -107,
	RS_ERR_CANNOT_WRITE_USB				= -108,
	RS_ERR_WRITE_USB_TIMEOUT			= -109,
	RS_ERR_CANNOT_READ_USB				= -110,
	RS_ERR_READ_USB_TIMEOUT				= -111,
	RS_ERR_INVALID_CAMERA_MODE			= -112,
	RS_ERR_UNSUPPORTED_WAV_FORMAT		= -113,
	RS_ERR_UNSUPPORTED_COMMAND			= -114,
	RS_ERR_SENSOR_DIRTY					= -115,
	RS_ERR_FINGER_EXIST					= -116,
	RS_ERR_TOO_STRONG_LIGHT				= -117,
	RS_ERR_INVALID_DEVICE_CONNECTION	= -124,

	//
	// Capture related errors
	//
	RS_ERR_INVALID_CAPTURE_MODE			= -200,
	RS_ERR_CAPTURE_DISABLED				= -201,
	RS_ERR_CAPTURE_TIMEOUT				= -202,
	RS_ERR_CAPTURE_ABORTED				= -203,
	RS_ERR_CAPTURE_CAMERA				= -204,
	RS_ERR_CAPTURE_NO_PREVIEW			= -205,
	RS_ERR_ROLL_PART_LIFT				= -206,
	RS_ERR_ROLL_DIRTY					= -207,
	RS_ERR_ROLL_TOO_FAST				= -208,
	RS_ERR_ROLL_SHIFTED					= -209,
	RS_ERR_ROLL_DRY						= -210,
	RS_ERR_ROLL_WRONG_DIR				= -211,
	RS_ERR_CAPTURE_IS_RUNNING			= -212,
	RS_ERR_CAPTURE_IS_NOT_RUNNING		= -213,
	RS_ERR_ROLL_TOO_SHORT				= -214,
	RS_ERR_CANNOT_SEGMENT				= -215,
	RS_ERR_CANNOT_GET_QUALITY			= -216,
	RS_ERR_SEGMENT_FEWER_FINGER			= -217,
	RS_ERR_SEGMENT_WRONG_HAND			= -218,
	RS_ERR_CANNOT_EXTRACT_TEMPLATE		= -219,
	RS_ERR_NO_MATCH						= -220,
	RS_ERR_ROLL_NO_FINGER				= -221,

	//
	// Viewer related errors
	//
	RS_ERR_CANNOT_SET_VIEW_WINDOW		= -300,
	RS_ERR_NO_MORE_OVERLAY				= -301,
	RS_ERR_INVALID_OVERLAY_HANDLE		= -302,
	RS_ERR_CANNOT_SHOW_OVERLAY			= -303,
	RS_ERR_CANNOT_SHOW_PREVIEW			= -304,

	//
	// IO related errors
	//
	RS_ERR_CANNOT_SET_KEYPAD_CALLBACK	= -400,
	RS_ERR_CANNOT_MAKE_LCD_DATA			= -401,
	RS_ERR_INVALID_LCD_DATA				= -402,

	//
	// Warning
	//
	RS_WRN_FAKE_FINGER				= 133,

} RS_ERROR_CODE;


#endif
