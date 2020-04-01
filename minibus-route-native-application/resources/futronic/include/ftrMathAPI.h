
// Definitions and prototypes for the Futronic Mathematical API.

#ifndef FTRMATHAPI_H
#define FTRMATHAPI_H

#include "ftrScanAPI.h"

#if defined(__WIN32__)
#pragma pack(push, 1)
#endif

# define XSIZE 320 
# define YSIZE 480 
# define MINSUBFSIZE 200
# define MAXSUBFSIZE 600

# define XSIZEFS50 800
# define YSIZEFS50 750
# define XSIZEFS60 1600
# define YSIZEFS60 1500

typedef struct SubfCoord_
{
	int xs, ys;
	int ws, hs;
	int err;
	int nfiq;
    int qfutr;
 
} SubfCoord;

typedef struct SegmParam_
{
	int nParamFing;
	int nParamAngle;
	int nParamNfiq;
	int nParamFixedSize;
	int nWidthSubf;
	int nHeightSubf;
	FTR_DWORD dwTimeScan;
	double dAngle;
	
} SegmParam;


enum { NFING1 = 1, NFING2 = 2, NFING3 = 3, NFING4 = 4 };
enum { NOANGLE = 0, ANGLE = 1 };
enum { NONFIQ = 0, NFIQ = 1, QFUTR = 2, QFULL = 3 };
enum { REALSIZE = 0, FIXEDSIZE = 1 };

#if defined(__WIN32__)
#pragma pack(pop)
#endif

#if defined(__WIN32__)
#if !defined( _LIB ) && !defined( USE_LIB )
#ifdef FTRMATHLIB_EXPORTS
#define FTRMATH_API __declspec(dllexport)
#else
#define FTRMATH_API __declspec(dllimport)
#endif  //FTRMATHLIB_EXPORTS
#else
#define FTRMATH_API
#endif  // _LIB
#else
#define FTRMATH_API
#endif  // defined(__WIN32__)


#ifdef __cplusplus
extern "C" {
#endif


FTRMATH_API FTR_BOOL ftrMathScanFrameNfIQ ( FTRHANDLE ftrHandle, FTR_PVOID pBuffer, PFTRSCAN_FRAME_PARAMETERS pFrameParameters, int *nNfiq, FTR_BOOL *bErrNfiq );

FTRMATH_API FTR_BOOL ftrMathScanDoseNfIQ ( FTRHANDLE ftrHandle, int nDose, FTR_PVOID pBuffer, int *nNfiq, FTR_BOOL *bErrNfiq );

FTRMATH_API FTR_BOOL ftrMathScanFrameSegment ( FTRHANDLE ftrHandle, PFTRSCAN_FRAME_PARAMETERS pFrameParameters, FTR_PVOID pBuffer,
					FTR_PVOID pBufferResult, FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm );

FTRMATH_API FTR_BOOL ftrMathScanDoseSegment ( FTRHANDLE ftrHandle, int nDose, FTR_PVOID pBuffer,FTR_PVOID pBufferResult,
					FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm );

FTRMATH_API FTR_BOOL ftrMathScanFrameSegmentPreview ( FTRHANDLE ftrHandle, PFTRSCAN_FRAME_PARAMETERS pFrameParameters, FTR_PVOID pBuffer,
					FTR_PVOID pBufferResult, FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm );

FTRMATH_API FTR_BOOL ftrMathScanDoseSegmentPreview ( FTRHANDLE ftrHandle, int nDose, FTR_PVOID pBuffer,FTR_PVOID pBufferResult,
					FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm );

FTRMATH_API FTR_BOOL ftrMathGetVersion( FTRHANDLE ftrHandle, PFTRSCAN_VERSION pMathVersion );

FTRMATH_API FTR_BOOL ftrMathImageNfIQ ( FTRHANDLE ftrHandle, FTR_PVOID pBuffer, int nWidth, int nHeight, int *nNfiq, FTR_BOOL *bErrNfiq );

FTRMATH_API FTR_BOOL ftrMathImageSegment ( FTRHANDLE ftrHandle, FTR_PVOID pBuffer,int nWidth, int nHeight, 
					FTR_PVOID pBufferResult, FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm );

FTRMATH_API FTR_BOOL ftrMathScanFrameSegmentPreviewAuto ( FTRHANDLE ftrHandle, PFTRSCAN_FRAME_PARAMETERS pFrameParameters, FTR_PVOID pBuffer,
					FTR_PVOID pBufferResult, FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm, int *nAutoThresh );

FTRMATH_API FTR_BOOL ftrMathImageSegmentAuto ( FTRHANDLE ftrHandle, FTR_PVOID pBuffer,int nWidth, int nHeight, 
					FTR_PVOID pBufferResult, FTR_PVOID pBufferSubf, SegmParam *Param, SubfCoord *Subf, FTR_BOOL *bErrSegm, int *nAutoThresh );

#ifdef __cplusplus
};
#endif


#endif