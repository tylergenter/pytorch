#ifndef THCS_GENERIC_FILE
#define THCS_GENERIC_FILE "generic/THCSTensor.cu"
#else

THCTensor *THCSTensor_(toDense)(THCState *state, THCSTensor *self) {
  /*
  THLongStorage *storage;
  THCTensor *other;

  THCSTensor_(contiguous)(state, self);

  // set up the new tensor
  storage = THCSTensor_(newSizeOf)(state, self);
  other = THCTensor_(newWithSize)(state, storage, NULL);
  THCTensor_(zero)(state, other);

  // Some necessary dimensions and sizes
  const ptrdiff_t nnz = THCSTensor_(nnz)(state, self);
  const dim3 block = getApplyBlock();
  dim3 grid;
  THArgCheck(getApplyGrid(state, nnz, grid), 1, CUTORCH_DIM_WARNING);

  TensorInfo<real, uint64_t> otherInfo =
    getTensorInfo<THCTensor, uint64_t>(state, other);
  TensorInfo<int64_t, uint64_t> indicesInfo =
    getTensorInfo<THCudaLongTensor, uint64_t>(state, self->indices);
  TensorInfo<real, uint64_t> valuesInfo =
    getTensorInfo<THCTensor, uint64_t>(state, self->values);

  THCSTensor_toDenseKernel<uint64_t, real>
    <<<grid, block, 0, THCState_getCurrentStream(state)>>>(
        otherInfo, indicesInfo, valuesInfo, (uint64_t)(nnz));

  THCudaCheck(cudaGetLastError());
  THLongStorage_free(storage);
  return other;
  */
  THError("WARNING: Sparse Cuda Tensor op toDense is not implemented");
  return NULL;
}

void THCSTensor_(reorder)(THCState *state, THCSTensor *self) {
  THError("WARNING: Sparse Cuda Tensor op reorder is not implemented");
}

void THCSTensor_(contiguous)(THCState *state, THCSTensor *self) {
  if (self->contiguous) return;
  THCSTensor_(reorder)(state, self);
  self->contiguous = 1;
}

// In place transpose
void THCSTensor_(transpose)(THCState *state, THCSTensor *self, int d1, int d2) {
  /* TODO
  THCudaLongTensor *indices = THCSTensor_(indices)(state, self);
  int64_t i;
  for (i = 0; i < THCSTensor_(nnz)(state, self); i++) {
    int64_t tmp = THCTensor_fastGet2d(indices, d1, i);
    THCTensor_fastSet2d(indices, d1, i,
        THCTensor_fastGet2d(indices, d2, i));
    THCTensor_fastSet2d(indices, d2, i, tmp);
  }
  i = self->size[d1];
  self->size[d1] = self->size[d2];
  self->size[d2] = i;
  self->contiguous = 0;
  THFree(indices);
  */
  THError("WARNING: Sparse Cuda Tensor op transpose is not implemented");
}

int THCSTensor_(getDevice)(THCState* state, const THCSTensor* tensor) {
  if (!tensor->values || !tensor->values->storage) return -1;
  return THCStorage_(getDevice)(state, tensor->values->storage);
}

void THCTensor_(sparseMask)(THCState *state, THCSTensor *r_, THCTensor *t, THCSTensor *mask) {
  THError("WARNING: Sparse Cuda Tensor op sparseMask is not implemented");
}

#endif
