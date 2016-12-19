/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2016 Inviwo Foundation
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

#include <modules/python3/pythonincluder.h>

#include "pyutil.h"

#include <modules/python3/pythoninterface/pyvalueparser.h>
#include <modules/python3/pythoninterface/pythonparameterparser.h>
#include <inviwo/core/util/systemcapabilities.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/common/inviwocore.h>
#include <inviwo/core/util/filesystem.h>

namespace inviwo {

PyObject* py_quit(PyObject* self, PyObject* args) {
    static PythonParameterParser tester;
    if (tester.parse(args) == -1) {
        return nullptr;
    }

    InviwoApplication::getPtr()->closeInviwoApplication();
    Py_RETURN_NONE;
}

PyObject* py_wait(PyObject* self, PyObject* args) {
    InviwoApplication::getPtr()->waitForPool();
    Py_RETURN_NONE;
}

PyObject* py_snapshot(PyObject* self, PyObject* args) {
    static PythonParameterParser tester(1);
    std::string filename, canvasName = "";
    if (tester.parse(args, filename, canvasName)== -1) {
        return nullptr;
    }
    
    CanvasProcessor* canvas = nullptr;

    if (canvasName.size() != 0) {
        canvas = dynamic_cast<CanvasProcessor*>(
            InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(
                canvasName));
    } else {
        if (InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()) {
            std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()
                                                         ->getProcessorNetwork()
                                                         ->getProcessorsByType<CanvasProcessor>();

            if (canvases.size() != 0)
                canvas = InviwoApplication::getPtr()
                             ->getProcessorNetwork()
                             ->getProcessorsByType<CanvasProcessor>()[0];
        }
    }

    if (!canvas) {
        PyErr_SetString(PyExc_TypeError, "snapshot() no canvas found");
        return nullptr;
    }

    canvas->saveImageLayer(filename.c_str());
    Py_RETURN_NONE;
}

PyObject* py_snapshotCanvas(PyObject* self, PyObject* args) {
    static PythonParameterParser tester;
    int index;
    std::string filename;
    if (tester.parse(args, index, filename)== -1) {
        return nullptr;
    }
    
    std::vector<CanvasProcessor*> canvases =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();

    if (index >= static_cast<int>(canvases.size())) {
        std::string msg = std::string("snapshotCanvas() index out of range with index: ") +
                          toString(index) + " ,canvases avilable: " + toString(canvases.size());
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return nullptr;
    }

    canvases[index]->saveImageLayer(filename);
    Py_RETURN_NONE;
}

PyObject* py_snapshotAllCanvases(PyObject* self, PyObject* args) {
    static PythonParameterParser tester(2);
    std::string path, prefix = "", fileEnding = "png";
    if (tester.parse(args, path, prefix, fileEnding) == -1) {
        return nullptr;
    }

    std::vector<CanvasProcessor*> canvases =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();

    for (auto& c : canvases) {
        std::stringstream ss;
        ss << path << "/" << prefix << c->getIdentifier() << "." << fileEnding;
        c->saveImageLayer(ss.str());
    }

    Py_RETURN_NONE;
}

PyObject* py_getBasePath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getBasePath());
}

PyObject* py_getOutputPath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(
        InviwoApplication::getPtr()->getCommandLineParser().getOutputPath());
}

PyObject* py_getDataPath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(
        filesystem::getPath(PathType::Data));
}

PyObject* py_getWorkspaceSavePath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(
        filesystem::getPath(PathType::Workspaces));
}
PyObject* py_getVolumePath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(
        filesystem::getPath(PathType::Volumes));
}
PyObject* py_getImagePath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(
        filesystem::getPath(PathType::Images));
}
PyObject* py_getModulePath(PyObject* self, PyObject* args) {
    static PythonParameterParser tester;
    std::string name;
    if (tester.parse(args, name) == -1) {
        return nullptr;
    }

    if (auto module = InviwoApplication::getPtr()->getModuleByIdentifier(name)) {
        return PyValueParser::toPyObject(module->getPath());
    } else {
        auto err = "Could not find a module with id: " + name;
        PyErr_SetString(PyExc_TypeError, err.c_str());
        return nullptr;
    }
}

PyObject* py_getTransferFunctionPath(PyObject* self, PyObject* args) {
    return PyValueParser::toPyObject(
        filesystem::getPath(PathType::TransferFunctions));
}

PyObject* py_getMemoryUsage(PyObject* self, PyObject* args) {
    InviwoApplication* a = InviwoApplication::getPtr();
    if (!a) {
        std::string msg =
            std::string("getMemoryUsage() failed . Inviwo Application modulde not found");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return nullptr;
    }
    InviwoModule* m = InviwoApplication::getPtr()->getModuleByType<InviwoCore>();
    if (!m) {
        std::string msg = std::string("getMemoryUsage() failed . Inviwo Core modulde not found");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return nullptr;
    }

    for (size_t i = 0; m->getCapabilities().size(); i++) {
        SystemCapabilities* sc = dynamic_cast<SystemCapabilities*>(m->getCapabilities()[i]);
        if (sc) {
            return PyValueParser::toPyObject(sc->getCurrentResidentMemoryUsage());
        }
    }

    std::string msg =
        std::string("getMemoryUsage() failed . No system capabilites found in Inivo Core");
    PyErr_SetString(PyExc_TypeError, msg.c_str());

    return nullptr;
}

PyObject* py_clearResourceManager(PyObject* self, PyObject* args) {
    if (ResourceManager::getPtr()) {
        ResourceManager::getPtr()->clearAllResources();
        Py_RETURN_NONE;
    }
    std::string msg =
        std::string("clearResourceManager() failed . ResourceManager::getPtr() return nullptr");
    PyErr_SetString(PyExc_TypeError, msg.c_str());
    return nullptr;
}

PyObject* py_disableEvaluation(PyObject* self, PyObject* args) {
    if (auto app = InviwoApplication::getPtr()) {
        if (auto network = app->getProcessorNetwork()) {
            network->lock();
            Py_RETURN_NONE;
        }

        std::string msg = std::string("disableEvaluation() could not find ProcessorNetwork");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return nullptr;
    }

    std::string msg = std::string("disableEvaluation() could not find InviwoApplication");
    PyErr_SetString(PyExc_TypeError, msg.c_str());
    return nullptr;
}

PyObject* py_enableEvaluation(PyObject* self, PyObject* args) {
    if (auto app = InviwoApplication::getPtr()) {
        if (auto network = app->getProcessorNetwork()) {
            network->unlock();
            Py_RETURN_NONE;
        }

        std::string msg = std::string("disableEvaluation() could not find ProcessorNetwork");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return nullptr;
    }

    std::string msg = std::string("disableEvaluation() could not find InviwoApplication");
    PyErr_SetString(PyExc_TypeError, msg.c_str());
    return nullptr;
}


}
