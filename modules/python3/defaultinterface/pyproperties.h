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

#ifndef IVW_PROPERTIESMEHTODSINVIWO_H
#define IVW_PROPERTIESMEHTODSINVIWO_H

#include <modules/python3/python3moduledefine.h>

namespace inviwo {

PyObject* py_setPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_clickButton(PyObject* /*self*/, PyObject* /*args*/);

PyObject* py_hasPropertyEditor(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyEditorVisible(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_isPropertyEditorVisible(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyEditorPosition(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyEditorPosition(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyEditorDimensions(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyEditorDimensions(PyObject* /*self*/, PyObject* /*args*/);

}  // namespace

#endif  // IVW_PROPERTIESMEHTODSINVIWO_H
