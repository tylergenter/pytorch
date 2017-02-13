#ifndef THS_GENERIC_FILE
#define THS_GENERIC_FILE "generic/THSTensor.h"
#else

typedef struct THSTensor
{  // Stored in COO format, indices + values
    int64_t *size;
    ptrdiff_t nnz;
    int nDimension;

    // 2-D tensor of nDim x nnz of indices. May have nnz dim bigger than nnz
    // as buffer, so we keep track of both
    THLongTensor *indices;
    THTensor *values;
    // Math operations can only be performed on ordered sparse tensors
    int contiguous;
    int refcount;

} THSTensor;

/**** access methods ****/
TH_API int THSTensor_(nDimension)(const THSTensor *self);
TH_API int64_t THSTensor_(size)(const THSTensor *self, int dim);
TH_API ptrdiff_t THSTensor_(nnz)(const THSTensor *self);
TH_API THLongStorage *THSTensor_(newSizeOf)(THSTensor *self);
TH_API THLongTensor *THSTensor_(indices)(const THSTensor *self);
TH_API THTensor *THSTensor_(values)(const THSTensor *self);

/**** creation methods ****/
TH_API THSTensor *THSTensor_(new)(void);
TH_API THSTensor *THSTensor_(newWithTensor)(THLongTensor *indices, THTensor *values);
TH_API THSTensor *THSTensor_(newWithTensorAndSize)(THLongTensor *indices, THTensor *values, THLongStorage *sizes);

TH_API THSTensor *THSTensor_(newWithSize)(THLongStorage *size_);
TH_API THSTensor *THSTensor_(newWithSize1d)(int64_t size0_);
TH_API THSTensor *THSTensor_(newWithSize2d)(int64_t size0_, int64_t size1_);
TH_API THSTensor *THSTensor_(newWithSize3d)(int64_t size0_, int64_t size1_, int64_t size2_);
TH_API THSTensor *THSTensor_(newWithSize4d)(int64_t size0_, int64_t size1_, int64_t size2_, int64_t size3_);

TH_API THSTensor *THSTensor_(newClone)(THSTensor *self);
TH_API THSTensor *THSTensor_(newContiguous)(THSTensor *self);
TH_API THSTensor *THSTensor_(newTranspose)(THSTensor *self, int dimension1_, int dimension2_);

/**** reshaping methods ***/
TH_API THSTensor *THSTensor_(resize)(THSTensor *self, THLongStorage *size);
TH_API THSTensor *THSTensor_(resize1d)(THSTensor *self, int64_t size0);
TH_API THSTensor *THSTensor_(resize2d)(THSTensor *self, int64_t size0, int64_t size1);
TH_API THSTensor *THSTensor_(resize3d)(THSTensor *self, int64_t size0, int64_t size1, int64_t size2);
TH_API THSTensor *THSTensor_(resize4d)(THSTensor *self, int64_t size0, int64_t size1, int64_t size2, int64_t size3);

TH_API THTensor *THSTensor_(toDense)(THSTensor *self);

TH_API void THSTensor_(transpose)(THSTensor *self, int dimension1_, int dimension2_);
TH_API int THSTensor_(isContiguous)(const THSTensor *self);
TH_API void THSTensor_(contiguous)(THSTensor *self);

TH_API void THSTensor_(free)(THSTensor *self);
TH_API void THSTensor_(retain)(THSTensor *self);


/* TODO (check function signatures too, might be wrong)
TH_API void THSTensor_(freeCopyTo)(THSTensor *self, THSTensor *dst);

TH_API void THSTensor_(narrow)(THSTensor *self, THSTensor *src, int dimension_, int64_t firstIndex_, int64_t size_);
TH_API void THSTensor_(select)(THSTensor *self, THSTensor *src, int dimension_, int64_t sliceIndex_);
*/

#endif
