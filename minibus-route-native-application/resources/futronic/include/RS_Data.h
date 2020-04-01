/**
 *  Data Structure Definitions
 *
 *  @author jhmoon@suprema.co.kr
 *  @see    
 */

 /*  
 *  Copyright (c) 2011 Suprema Inc. All Rights Reserved.
 * 
 *  This software is the confidential and proprietary information of 
 *  Suprema Co., Ltd. ("Confidential Information").  You shall not
 *  disclose such Confidential Information and shall use it only in
 *  accordance with the terms of the license agreement you entered into
 *  with Suprema.
 */

#ifndef __RS_DATA_H__
#define __RS_DATA_H__

//#include <afx.h>

//
// Device information
//
struct RSDeviceInfo {
	int deviceType;
	char productName[16];
	char deviceID[16];
	char firmwareVersion[16];
	char hardwareVersion[16];
	int reserved[32];
};

//
// SDK information
//
struct RSSDKInfo {
	char product[16];
	char version[16];
	char buildDate[16];
	int reserved[16];
};

//
// Overlay data structures
//

struct RSOverlayText {
	POINT		pos;
	unsigned	alignment;
	char		text[128];
	int			fontSize;
	char		fontName[32];
	COLORREF	color;
	int			reserved[16];
};


struct RSOverlayCross {
	POINT		centerPos;
	int			rangeX;
	int			rangeY;
	COLORREF	color;
	int			width;
	int			reserved[16];
};


struct RSOverlayLine {
	POINT		startPos;
	POINT		endPos;
	COLORREF	color;
	int			width;
	int			reserved[16];
};


struct RSOverlayQuadrangle {
	POINT		pos[4];
	COLORREF	color;
	int			width;
	int			reserved[16];
};


struct RSPoint {
	int x;
	int y;
};

//
// Slap information
//
struct RSSlapInfo {
	int fingerType; 
	RSPoint fingerPosition[4];
	int imageQuality;
	int rotation;
	int reserved[3];
};


#endif