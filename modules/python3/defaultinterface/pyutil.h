/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2017 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#ifndef IVW_PYSNAPSHOTMEHTODINVIWO_H
#define IVW_PYSNAPSHOTMEHTODINVIWO_H

#include <modules/python3/python3moduledefine.h>

namespace inviwo {

PyObject* py_quit(PyObject* self, PyObject* args);
PyObject* py_wait(PyObject* self, PyObject* args);
PyObject* py_snapshot(PyObject* self, PyObject* args);
PyObject* py_snapshotCanvas(PyObject* self, PyObject* args);
PyObject* py_snapshotAllCanvases(PyObject* self, PyObject* args);
PyObject* py_getBasePath(PyObject* self, PyObject* args);
PyObject* py_getOutputPath(PyObject* self, PyObject* args);
PyObject* py_getDataPath(PyObject* self, PyObject* args);
PyObject* py_getWorkspaceSavePath(PyObject* self, PyObject* args);
PyObject* py_getVolumePath(PyObject* self, PyObject* args);
PyObject* py_getImagePath(PyObject* self, PyObject* args);
PyObject* py_getModulePath(PyObject* self, PyObject* args);
PyObject* py_getTransferFunctionPath(PyObject* self, PyObject* args);

PyObject* py_getMemoryUsage(PyObject* self, PyObject* args);
PyObject* py_clearResourceManager(PyObject* self, PyObject* args);

PyObject* py_disableEvaluation(PyObject* self, PyObject* args);
PyObject* py_enableEvaluation(PyObject* self, PyObject* args);

}  // namespace

#endif  // IVW_PYSNAPSHOTMEHTODINVIWO_H
