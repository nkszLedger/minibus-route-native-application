#ifdef FTRNBIS_EXPORTS
#define FTRNBIS_API __declspec(dllexport) _stdcall
#else
#define FTRNBIS_API __declspec(dllimport) _stdcall
#endif

#ifdef __cplusplus
extern "C" { /* assume C declarations for C++ */
#endif

#define MAX_BOZORTH_MINUTIAE		200
#define MAX_FILE_MINUTIAE				1000

typedef struct minutia{
   int x;
   int y;
   int ex;
   int ey;
   int direction;
   double reliability;
   int type;
   int appearing;
   int feature_id;
   int *nbrs;
   int *ridge_counts;
   int num_nbrs;
} MINUTIA;

typedef struct minutiae{
   int alloc;
   int num;
   MINUTIA **list;
} MINUTIAE;

struct xytq_struct {
	int nrows;
	int xcol[ MAX_FILE_MINUTIAE ];
	int ycol[ MAX_FILE_MINUTIAE ];
	int thetacol[ MAX_FILE_MINUTIAE ];
	int qcol[ MAX_FILE_MINUTIAE ];
};

#define XYTQ_NULL ( (struct xytq_struct *) NULL ) 

/*************************************************************************/
/*        MINUTIAE XYT REPRESENTATION SCHEMES                            */
/*************************************************************************/
#define NIST_INTERNAL_XYT_REP  0
#define M1_XYT_REP             1

int FTRNBIS_API ftrGetMinutiae(MINUTIAE **ominutiae, int **oquality_map,
                 int **odirection_map, int **olow_contrast_map,
                 int **olow_flow_map, int **ohigh_curve_map,
                 int *omap_w, int *omap_h,
                 unsigned char **obdata, int *obw, int *obh, int *obd,
                 unsigned char *idata, const int iw, const int ih,
                 const int id, const double ippmm);

void FTRNBIS_API ftrFreeMinutiae(MINUTIAE *ominutiae);

int FTRNBIS_API ftrGetMinutiaeXYTQ(xytq_struct *ostruct,
								 int reptype, unsigned char *idata, const int iw, const int ih, const int id, const double ippmm);

int FTRNBIS_API ftrBozorth3Verify( xytq_struct *pstruct, xytq_struct *gstruct, int *score );

int FTRNBIS_API ftrBozorth3SetBaseProbe( void **pHandle,  xytq_struct *pstruct );

int FTRNBIS_API ftrBozorth3Identify( void *pHandle,  xytq_struct *gstruct, int *score );

void FTRNBIS_API ftrBozorth3ReleaseBaseProbe( void *pHandle );

#ifdef __cplusplus
}
#endif
