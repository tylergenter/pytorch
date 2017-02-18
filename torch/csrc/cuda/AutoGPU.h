#ifndef THCP_AUTOGPU_INC
#define THCP_AUTOGPU_INC

#include <Python.h>
#include "THP.h"

class THP_CLASS THCPAutoGPU {
public:
  THCPAutoGPU(int device_id=-1);
  THCPAutoGPU(PyObject *args, PyObject *self=NULL);
  ~THCPAutoGPU();
  bool setObjDevice(PyObject *obj);
  bool setDevice(int new_device);
  int device = -1;
};

#endif
