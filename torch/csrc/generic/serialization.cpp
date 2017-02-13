#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/serialization.cpp"
#else

#define SYSCHECK(call) { ssize_t __result = call; if (__result < 0) throw std::system_error((int) __result, std::system_category()); }

void THPTensor_(writeMetadataRaw)(THTensor *self, int fd)
{
  SYSCHECK(write(fd, &self->nDimension, sizeof(int64_t)));
  SYSCHECK(write(fd, self->size, sizeof(int64_t) * self->nDimension));
  SYSCHECK(write(fd, self->stride, sizeof(int64_t) * self->nDimension));
  SYSCHECK(write(fd, &self->storageOffset, sizeof(int64_t)));
}

THTensor * THPTensor_(newWithMetadataFileRaw)(int fd, THStorage *storage)
{
  THTensorPtr tensor = THTensor_(new)(LIBRARY_STATE_NOARGS);
  SYSCHECK(read(fd, &tensor->nDimension, sizeof(int64_t)));
  tensor->size = (int64_t*)THAlloc(tensor->nDimension * sizeof(int64_t));
  tensor->stride = (int64_t*)THAlloc(tensor->nDimension * sizeof(int64_t));
  SYSCHECK(read(fd, tensor->size, sizeof(int64_t) * tensor->nDimension));
  SYSCHECK(read(fd, tensor->stride, sizeof(int64_t) * tensor->nDimension));
  SYSCHECK(read(fd, &tensor->storageOffset, sizeof(int64_t)));
  THStorage_(retain)(LIBRARY_STATE storage);
  tensor->storage = storage;
  return tensor.release();
}

void THPStorage_(writeFileRaw)(THStorage *self, int fd)
{
  real *data;
#ifndef THC_GENERIC_FILE
  data = self->data;
#else
  std::unique_ptr<char[]> cpu_data(new char[self->size * sizeof(real)]);
  data = (real*)cpu_data.get();
  THCudaCheck(cudaMemcpy(data, self->data, self->size * sizeof(real), cudaMemcpyDeviceToHost));
#endif
  SYSCHECK(write(fd, &self->size, sizeof(int64_t)));
  // fast track for bytes and little endian
  if (sizeof(real) == 1 || THP_nativeByteOrder() == THPByteOrder::THP_LITTLE_ENDIAN) {
    SYSCHECK(write(fd, data, sizeof(real) * self->size));
  } else {
    int64_t buffer_size = std::min(self->size, (int64_t)5000);
    std::unique_ptr<uint8_t[]> le_buffer(new uint8_t[buffer_size * sizeof(real)]);
    for (int64_t i = 0; i < self->size; i += buffer_size) {
      size_t to_convert = std::min(self->size - i, buffer_size);
      if (sizeof(real) == 2) {
        THP_encodeInt16Buffer((uint8_t*)le_buffer.get(),
            (const int16_t*)data + i,
            THPByteOrder::THP_LITTLE_ENDIAN,
            to_convert);
      } else if (sizeof(real) == 4) {
        THP_encodeInt32Buffer((uint8_t*)le_buffer.get(),
            (const int32_t*)data + i,
            THPByteOrder::THP_LITTLE_ENDIAN,
            to_convert);
      } else if (sizeof(real) == 8) {
        THP_encodeInt64Buffer((uint8_t*)le_buffer.get(),
            (const int64_t*)data + i,
            THPByteOrder::THP_LITTLE_ENDIAN,
            to_convert);
      }
      SYSCHECK(write(fd, data, to_convert * sizeof(real)));
    }
  }
}

THStorage * THPStorage_(readFileRaw)(int fd)
{
  real *data;
  int64_t size;
  SYSCHECK(read(fd, &size, sizeof(int64_t)));
  THStoragePtr storage = THStorage_(newWithSize)(LIBRARY_STATE size);

#ifndef THC_GENERIC_FILE
  data = storage->data;
#else
  std::unique_ptr<char[]> cpu_data(new char[size * sizeof(real)]);
  data = (real*)cpu_data.get();
#endif

  // fast track for bytes and little endian
  if (sizeof(real) == 1 || THP_nativeByteOrder() == THPByteOrder::THP_LITTLE_ENDIAN) {
    SYSCHECK(read(fd, data, sizeof(real) * storage->size));
  } else {
    int64_t buffer_size = std::min(size, (int64_t)5000);
    std::unique_ptr<uint8_t[]> le_buffer(new uint8_t[buffer_size * sizeof(real)]);
    for (int64_t i = 0; i < size; i += buffer_size) {
      size_t to_convert = std::min(size - i, buffer_size);
      SYSCHECK(read(fd, le_buffer.get(), sizeof(real) * to_convert));
      if (sizeof(real) == 2) {
        THP_decodeInt16Buffer((int16_t*)data + i,
            le_buffer.get(),
            THPByteOrder::THP_LITTLE_ENDIAN,
            to_convert);
      } else if (sizeof(real) == 4) {
        THP_decodeInt32Buffer((int32_t*)data + i,
            le_buffer.get(),
            THPByteOrder::THP_LITTLE_ENDIAN,
            to_convert);
      } else if (sizeof(real) == 8) {
        THP_decodeInt64Buffer((int64_t*)data + i,
            le_buffer.get(),
            THPByteOrder::THP_LITTLE_ENDIAN,
            to_convert);
      }
    }
  }

#ifdef THC_GENERIC_FILE
  THCudaCheck(cudaMemcpy(storage->data, data, size * sizeof(real), cudaMemcpyHostToDevice));
#endif
  return storage.release();
}

#undef SYSCHECK

#endif
