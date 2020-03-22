/**
 *  RealScan SDK Main API V1.6
 *
 *  @author jhmoon@suprema.co.kr
 *  @see    
 */

/*  
 *  Copyright (c) 2012 Suprema Inc. All Rights Reserved.
 * 
 *  This software is the confidential and proprietary information of 
 *  Suprema Inc. ("Confidential Information").  You shall not
 *  disclose such Confidential Information and shall use it only in
 *  accordance with the terms of the license agreement you entered into
 *  with Suprema.
 */

#ifndef __RS_API_H__
#define __RS_API_H__

//#include <atlstr.h>
//#include <atlimage.h>
#include <windows.h>
#include "RS_Error.h"
#include "RS_Data.h"

#ifdef REALSCANSDK_EXPORTS
#define REALSCANSDK_API __declspec(dllexport)
#else
#define REALSCANSDK_API __declspec(dllimport)
#endif

//
// Device type
//
#define RS_DEVICE_REALSCAN_10		0x00
#define RS_DEVICE_REALSCAN_10F		0x01
#define RS_DEVICE_REALSCAN_D		0x10
#define RS_DEVICE_REALSCAN_DF		0x11
#define RS_DEVICE_REALSCAN_F		0x20 
#define RS_DEVICE_REALSCAN_G10		0x30 
#define RS_DEVICE_REALSCAN_G10F		0x31 
#define RS_DEVICE_REALSCAN_G1		0x32
#define RS_DEVICE_UNKNOWN		0xFF

//
// Initialization Mode
//
#define RS_INIT_HIDE_INIDLG		0x01
#define RS_INIT_SHOW_INIDLG		0x02
#define RS_INIT_FULL			0x04   

//
// Capture mode
//
#define RS_CAPTURE_DISABLED					0x00
#define RS_CAPTURE_ROLL_FINGER				0x01
#define RS_CAPTURE_FLAT_SINGLE_FINGER		0x02
#define RS_CAPTURE_FLAT_TWO_FINGERS			0x03
#define RS_CAPTURE_FLAT_LEFT_FOUR_FINGERS	0x04
#define RS_CAPTURE_FLAT_RIGHT_FOUR_FINGERS	0x05
#define RS_CAPTURE_FLAT_LEFT_PALM			0x06 
#define RS_CAPTURE_FLAT_RIGHT_PALM			0x07 
#define RS_CAPTURE_FLAT_SINGLE_FINGER_EX	0x12
#define RS_CAPTURE_FLAT_TWO_FINGERS_EX		0x13 
	
//
// Options for capturing flat fingers
//
#define RS_AUTO_SENSITIVITY_NORMAL		0x00
#define RS_AUTO_SENSITIVITY_HIGH		0x01
#define RS_AUTO_SENSITIVITY_HIGHER		0x02
#define RS_AUTO_SENSITIVITY_DISABLED	0x03 

//
// Roll direction
//
#define	RS_ROLL_DIR_L2R		0x00
#define RS_ROLL_DIR_R2L		0x01
#define RS_ROLL_DIR_AUTO	0x02
#define RS_ROLL_DIR_AUTO_M	0x03 

//
// Roll profile
//
#define RS_ROLL_PROFILE_LOW		0x01
#define RS_ROLL_PROFILE_NORMAL	0x02
#define RS_ROLL_PROFILE_HIGH	0x03

//
// Text alignment
//
#define RS_TEXT_ALIGN_LEFT		0x00
#define RS_TEXT_ALIGN_CENTER	0x01
#define RS_TEXT_ALIGN_RIGHT		0x02

//
// Beeper pattern
//
#define RS_BEEP_PATTERN_NONE	0
#define RS_BEEP_PATTERN_1		1	// 1 short beep
#define RS_BEEP_PATTERN_2		2	// 2 short beeps

//
// Keypad code
//
#define RS_REALSCAN10_NO_KEY	0x00
#define RS_REALSCAN10_PLAY_KEY	0x20
#define RS_REALSCAN10_STOP_KEY	0x40
#define RS_REALSCAN10_ALL_KEYS  0x7F

#define RS_REALSCAND_NO_KEY		0x00
#define RS_REALSCAND_KEY_0		0x20
#define RS_REALSCAND_ALL_KEYS	0x7F

#define RS_REALSCANF_NO_KEY		0x00 
#define RS_REALSCANF_UP_KEY		0x01 
#define RS_REALSCANF_DOWN_KEY	0x02 
#define RS_REALSCANF_LEFT_KEY	0x04 
#define RS_REALSCANF_RIGHT_KEY	0x08 
#define RS_REALSCANF_PLAY_KEY	0x20 
#define RS_REALSCANF_STOP_KEY	0x40 
#define RS_REALSCANF_FOOTSWITCH	0x80 
#define RS_REALSCANF_ALL_KEYS	0xFF 
#define RS_REALSCANG10_NO_KEY	0x00 
#define RS_REALSCANG10_PLAY_KEY	0x20 
#define RS_REALSCANG10_STOP_KEY	0x40 
#define RS_REALSCANG10_ALL_KEYS 0x7F 
//
// Finger Index
//
#define RS_FINGER_ALL			0x00
#define RS_FINGER_LEFT_LITTLE	0x01
#define RS_FINGER_LEFT_RING		0x02
#define RS_FINGER_LEFT_MIDDLE	0x03
#define RS_FINGER_LEFT_INDEX	0x04
#define RS_FINGER_LEFT_THUMB	0x05
#define RS_FINGER_RIGHT_THUMB	0x06
#define RS_FINGER_RIGHT_INDEX	0x07
#define RS_FINGER_RIGHT_MIDDLE	0x08
#define RS_FINGER_RIGHT_RING	0x09
#define RS_FINGER_RIGHT_LITTLE	0x0A
#define RS_FINGER_TWO_THUMB		0x0B
#define RS_FINGER_LEFT_FOUR		0x0C
#define RS_FINGER_RIGHT_FOUR	0x0D
#define RS_FINGER_TWO_LEFT1		0x0E 
#define RS_FINGER_TWO_LEFT2		0x0F 
#define RS_FINGER_TWO_RIGHT2	0x10 
#define RS_FINGER_TWO_RIGHT1	0x11 
#define RS_PALM_LEFT			0x12 
#define RS_PALM_RIGHT			0x13 

//
// Finger position for segmentation
//
#define RS_FGP_UNKNOWN				0
#define RS_FGP_RIGHT_THUMB			1
#define RS_FGP_RIGHT_INDEX			2
#define RS_FGP_RIGHT_MIDDLE			3
#define RS_FGP_RIGHT_RING			4
#define RS_FGP_RIGHT_LITTLE			5
#define RS_FGP_LEFT_THUMB			6
#define RS_FGP_LEFT_INDEX			7
#define RS_FGP_LEFT_MIDDLE			8
#define RS_FGP_LEFT_RING			9
#define RS_FGP_LEFT_LITTLE			10
#define RS_FGP_PLAIN_RIGHT_THUMB	11
#define RS_FGP_PLAIN_LEFT_THUMB		12
#define RS_FGP_PLAIN_RIGHT_FOUR		13
#define RS_FGP_PLAIN_LEFT_FOUR		14
#define RS_FGP_PLAIN_TWO_THUMBS		15
#define RS_FGP_EJI_OR_TIP			16

//
// LED for RealScan-10
//
#define RS_LED_MODE_ALL				0x00
#define RS_LED_MODE_LEFT_FINGER4	0x01
#define RS_LED_MODE_RIGHT_FINGER4	0x02
#define RS_LED_MODE_TWO_THUMB		0x03
#define RS_LED_MODE_ROLL			0x04
#define RS_LED_POWER				0x05

//
// LED Colors
//
#define RS_LED_OFF				0x00
#define RS_LED_GREEN			0x01
#define RS_LED_RED				0x02
#define RS_LED_YELLOW			0x03

//
// LED Status for G1 
//
#define RS_LED_STATUS_OFF		0x00
#define RS_LED_STATUS_ON		0x01
#define RS_LED_STATUS_BLINK		0x02

//
// Slap type
//
#define RS_SLAP_LEFT_FOUR			1
#define RS_SLAP_RIGHT_FOUR			2
#define RS_SLAP_FOUR_FINGER			3
#define RS_SLAP_TWO_THUMB			4
#define RS_SLAP_TWO_FINGER			5
#define RS_SLAP_ONE_FINGER			6
#define RS_SLAP_ONE_FINGER_ROLL		7 

//
// Security level
//
#define RS_SECURITY_1_TO_100		0x01 
#define RS_SECURITY_1_TO_1000		0x02 
#define RS_SECURITY_1_TO_10000		0x03
#define RS_SECURITY_1_TO_100000		0x04
#define RS_SECURITY_1_TO_1000000	0x05
#define RS_SECURITY_1_TO_10000000	0x06
#define RS_SECURITY_1_TO_100000000	0x07

//
// Contrast enhancement
//
#define RS_CONTRAST_ENHANCEMENT_DEFVALUE 0
#define RS_CONTRAST_ENHANCEMENT_MAXVALUE 40

//
// LCD Display
//
#define RS_LCD_WIDTH_MAX			320 
#define RS_LCD_HEIGHT_MAX			240 
#define RS_LCD_DATA_SIZE_MAX		153600 

//
// LFD Level for G1 
//
#define RS_LFD_OFF				0
#define RS_LFD_LEVEL_1				1
#define RS_LFD_LEVEL_2				2
#define RS_LFD_LEVEL_3				3
#define RS_LFD_LEVEL_4				4
#define RS_LFD_LEVEL_5				5
#define RS_LFD_LEVEL_6				6

//
// Capture callback function
//
//typedef void (__stdcall *RSCaptureCallback)( int deviceHandle, int captureResult, CImage* image );
typedef void (__stdcall *RSRawCaptureCallback)( int deviceHandle, int errorCode, unsigned char* imageData, int imageWidth, int imageHeight );
typedef void (__stdcall *RSKeypadCallback)( int deviceHandle, unsigned keyCode );


#ifdef __cplusplus
extern "C" 
{
#endif

//
// Device APIs
//
REALSCANSDK_API int __stdcall RS_InitSDK( const char* configFileName, int option, int* numOfDevice );
REALSCANSDK_API	int __stdcall RS_InitDevice( int deviceIndex, int*deviceHandle );
REALSCANSDK_API int __stdcall RS_ExitDevice( int deviceHandle );
REALSCANSDK_API int __stdcall RS_ExitAllDevices();
REALSCANSDK_API int __stdcall RS_GetDeviceInfo( int deviceHandle, RSDeviceInfo* deviceInfo );
REALSCANSDK_API int __stdcall RS_UpgradeFirmware( int deviceHandle, unsigned char* firmware, int firmwareSize );

//
// Capture APIs
//
REALSCANSDK_API int __stdcall RS_SetCaptureMode( int deviceHandle, int captureMode, int captureOption, bool withModeLED );
REALSCANSDK_API int __stdcall RS_GetCaptureMode( int deviceHandle, int* captureMode, int* captureOption );

REALSCANSDK_API int __stdcall RS_SetRollFingerOption( int deviceHandle, int rollDirection, int rollTime, int rollProfile );
REALSCANSDK_API int __stdcall RS_GetRollFingerOption( int deviceHandle, int* rollDirection, int* rollTime, int* rollProfile );

REALSCANSDK_API int __stdcall RS_GetImageSize( int deviceHandle, int* imageWidth, int* imageHeight );

REALSCANSDK_API int __stdcall RS_StartCapture( int deviceHandle, bool autoCapture, int timeout );
REALSCANSDK_API	int __stdcall RS_AbortCapture( int deviceHandle );
//REALSCANSDK_API	int __stdcall RS_RegisterCaptureCallback( int deviceHandle, RSCaptureCallback captureCallback );
REALSCANSDK_API	int __stdcall RS_RegisterCaptureDataCallback( int deviceHandle, RSRawCaptureCallback captureCallback ); 
REALSCANSDK_API	int __stdcall RS_RegisterPreviewCallback( int deviceHandle, RSRawCaptureCallback captureCallback ); 

//REALSCANSDK_API	int __stdcall RS_TakeImage( int deviceHandle, int timeout, CImage** image );
REALSCANSDK_API	int __stdcall RS_TakeImageData( int deviceHandle, int timeout, unsigned char** imageData, int* imageWidth, int* imageHeight );
REALSCANSDK_API int __stdcall RS_TakeImageDataSegment( int deviceHandle, int timeout, unsigned char** imageData, int* imageWidth, int* imageHeight, int* captureResult, int slapType, int* numOfFinger, RSSlapInfo** slapInfo, unsigned char*** fingerImageData, int** fingerImageWidth, int** fingerImageHeight );
//REALSCANSDK_API	int __stdcall RS_TakeImageEx( int deviceHandle, int timeout, int fingerIndex, bool withLED, CImage** image );
REALSCANSDK_API	int __stdcall RS_TakeImageDataEx( int deviceHandle, int timeout, int fingerIndex, bool withLED, unsigned char** imageData, int* imageWidth, int* imageHeight );
//REALSCANSDK_API	int __stdcall RS_TakeCurrentImage( int deviceHandle, int timeout, CImage** image );
REALSCANSDK_API	int __stdcall RS_TakeCurrentImageData( int deviceHandle, int timeout, unsigned char** imageData, int* imageWidth, int* imageHeight );
REALSCANSDK_API	int __stdcall RS_TakeCurrentImageDataSegment( int deviceHandle, int timeout, unsigned char** imageData, int* imageWidth, int* imageHeight, int* captureResult, int slapType, int* numOfFinger, RSSlapInfo** slapInfo, unsigned char*** fingerImageData, int** fingerImageWidth, int** fingerImageHeight );
//REALSCANSDK_API	int __stdcall RS_TakeCurrentImageEx( int deviceHandle, int timeout, int fingerIndex, bool withLED, CImage** image );
REALSCANSDK_API	int __stdcall RS_TakeCurrentImageDataEx( int deviceHandle, int timeout, int fingerIndex, bool withLED, unsigned char** imageData, int* imageWidth, int* imageHeight );

REALSCANSDK_API int __stdcall RS_Segment( unsigned char* imageData, int imageWidth, int imageHeight, int slapType, int* numOfFinger, RSSlapInfo** slapInfo, unsigned char*** fingerImageData, int** fingerImageDataWidth, int** fingerImageDataHeight );
REALSCANSDK_API int __stdcall RS_SegmentImages( unsigned char* imageData, int imageWidth, int imageHeight, int slapType, int* numOfFinger, RSSlapInfo** slapInfo, const char* outFilename );
REALSCANSDK_API int __stdcall RS_GetQualityScore( unsigned char* imageData, int imageWidth, int imageHeight, int* nistQuality );
REALSCANSDK_API int __stdcall RS_VerifyImage( unsigned char* targetImageData, int targetImageWidth, int targetImageHeight, unsigned char* referenceImageData, int referenceImageWidth, int referenceImageHeight, int securityLevel );
REALSCANSDK_API int __stdcall RS_SequenceCheck( int numOfFinger, unsigned char** fingerImageData, int* fingerImageWidth, int* fingerImageHeight, unsigned char* slapImageData, int slapImageWidth, int slapImageHeight, int slapType, int* fingerSequenceInSlap, int securityLevel );
REALSCANSDK_API int __stdcall RS_SetSegRotateOption( bool isRotating );

REALSCANSDK_API	int __stdcall RS_Calibrate( int deviceHandle );
REALSCANSDK_API	int __stdcall RS_SetAutomaticCalibrate( int deviceHandle, bool automatic );
REALSCANSDK_API	int __stdcall RS_GetAutomaticCalibrate( int deviceHandle, bool* automatic );

REALSCANSDK_API	int __stdcall RS_SetAutomaticContrast( int deviceHandle, bool automatic );
REALSCANSDK_API	int __stdcall RS_GetAutomaticContrast( int deviceHandle, bool* automatic );

REALSCANSDK_API	int __stdcall RS_SetManualContrast( int deviceHandle, int contrastLevel );
REALSCANSDK_API	int __stdcall RS_GetManualContrast( int deviceHandle, int* contrastLevel );

REALSCANSDK_API	int __stdcall RS_SetAdvancedContrastEnhancement( int deviceHandle, bool enabled );
REALSCANSDK_API	int __stdcall RS_GetAdvancedContrastEnhancement( int deviceHandle, bool* enabled );

REALSCANSDK_API	int __stdcall RS_SetPostProcessing( int deviceHandle, bool contrastEnhancement, bool noiseReduction );
REALSCANSDK_API	int __stdcall RS_GetPostProcessing( int deviceHandle, bool* contrastEnhancement, bool* noiseReduction );

REALSCANSDK_API	int __stdcall RS_SetPostProcessingEx( int deviceHandle, bool contrastEnhancement, bool noiseReduction, int reductionLevel );
REALSCANSDK_API	int __stdcall RS_GetPostProcessingEx( int deviceHandle, bool* contrastEnhancement, bool* noiseReduction, int* reductionLevel );

REALSCANSDK_API int __stdcall RS_SelfTest( int deviceHandle, int testType ); 

REALSCANSDK_API int __stdcall RS_SetLFDLevel(int deviceHandle, int LFDLevel); 
REALSCANSDK_API int __stdcall RS_GetLFDLevel(int deviceHandle, int* LFDLevel); 

//
// VIEW APIs
//
REALSCANSDK_API int __stdcall RS_SetViewWindow( int deviceHandle, HWND windowHandle, RECT drawRectangle, bool autoContrast );
REALSCANSDK_API int __stdcall RS_StopViewWindow( int deviceHandle );
REALSCANSDK_API int __stdcall RS_SetViewWindow2( int deviceHandle, HWND windowHandle, RECT* drawRectangle, bool autoContrast );

REALSCANSDK_API	int	__stdcall RS_AddOverlayText( int deviceHandle, RSOverlayText* text, int* overlayhandle );
REALSCANSDK_API	int __stdcall RS_AddOverlayCross( int deviceHandle, RSOverlayCross* cross, int* overlayHandle );
REALSCANSDK_API int __stdcall RS_AddOverlayLine( int deviceHandle, RSOverlayLine* line, int* overlayHandle );
REALSCANSDK_API	int __stdcall RS_AddOverlayQuadrangle( int deviceHandle, RSOverlayQuadrangle* quadrangle, int* overlayHandle );
REALSCANSDK_API int __stdcall RS_ShowOverlay( int overlayHandle, bool show );
REALSCANSDK_API int __stdcall RS_ShowAllOverlay( int deviceHandle, bool show );
REALSCANSDK_API	int __stdcall RS_RemoveOverlay( int overlayHandle );
REALSCANSDK_API	int __stdcall RS_RemoveAllOverlay( int deviceHandle );

//
// I/O
//
REALSCANSDK_API	int __stdcall RS_SetActiveKey( int deviceHandle, unsigned keyMask );
REALSCANSDK_API int __stdcall RS_GetKeyStatus( int deviceHandle, unsigned* keyCode );
REALSCANSDK_API int __stdcall RS_RegisterKeypadCallback( int deviceHandle, RSKeypadCallback callback );

REALSCANSDK_API	int __stdcall RS_Beep( int deviceHandle, int beepPattern );
REALSCANSDK_API	int __stdcall RS_SetCaptureBeep( int deviceHandle, int startingBeep, int successBeep, int failBeep );

REALSCANSDK_API	int __stdcall RS_SetFingerLED( int deviceHandle, int fingerIndex, int ledColor );
REALSCANSDK_API	int __stdcall RS_SetModeLED( int deviceHandle, int ledIndex, bool isOn );
REALSCANSDK_API	int __stdcall RS_SetStatusLED( int deviceHandle, int ledCode );

REALSCANSDK_API int __stdcall RS_PlayWav( int deviceHandle, const char* wavFile );

REALSCANSDK_API int __stdcall RS_DisplayLCD( int deviceHandle, unsigned char* data, int dataLen, int sx, int sy, int width, int height ); 
REALSCANSDK_API int __stdcall RS_MakeLCDData( unsigned char* inputRData, unsigned char* inputGData, unsigned char* inputBData, int inputWidth, int inputHeight, unsigned char** outputData ); 
REALSCANSDK_API int __stdcall RS_ResetLCD( int deviceHandle );

//
// Misc.
//
REALSCANSDK_API int __stdcall RS_GetSDKInfo( RSSDKInfo* sdkInfo );
//REALSCANSDK_API int __stdcall RS_GetErrString( int errorCode, CString& errorMsg );
REALSCANSDK_API void __stdcall RS_FreeImageData( unsigned char* imageData );
REALSCANSDK_API int __stdcall RS_SaveBitmap( unsigned char* pixelData, int imageWidth, int imageHeight, const char* filename );
REALSCANSDK_API int __stdcall RS_SaveBitmapMem( unsigned char* pixelData, int imageWidth, int imageHeight, unsigned char* imageBuffer );

#ifdef __cplusplus
}
#endif

#endif
