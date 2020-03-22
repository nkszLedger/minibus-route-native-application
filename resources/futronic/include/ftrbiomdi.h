#ifdef FTRBIOMDI_EXPORTS
#define FTRBIOMDI_API __declspec(dllexport) 
#else
#define FTRBIOMDI_API __declspec(dllimport)
#endif

#define FIR_STD_ANSI	1
#define FIR_STD_ISO	2

#ifdef __cplusplus
extern "C" { /* assume C declarations for C++ */
#endif
	
FTRBIOMDI_API void * _stdcall ftrNewFIR(unsigned int nStdFormat, unsigned short nDeviceID);
FTRBIOMDI_API void _stdcall ftrFreeFIR(void * pFir);
FTRBIOMDI_API bool _stdcall ftrFIRAddImage(void *pFir, unsigned char *pImage, int nImageSize, int nWidth, int nHeight,
										   unsigned char nFingerPosition, unsigned char nNFIQ, unsigned char nImpressionType);
FTRBIOMDI_API bool _stdcall ftrGetFIRDataSize(void * pFir, unsigned long long *nSize);
FTRBIOMDI_API bool _stdcall ftrGetFIRData(void * pFir, unsigned long long nSize, unsigned char *pData);

#ifdef __cplusplus
}
#endif
