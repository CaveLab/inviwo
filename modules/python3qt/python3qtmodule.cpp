/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2015 Inviwo Foundation
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

#include <modules/python3qt/python3qtmodule.h>

#include <modules/python3/pythoninterface/pymodule.h>
#include "pythonqtmethods/pythonqtmethods.h"
#include "pythonmenu.h"
#include <modules/python3/pyinviwo.h>
namespace inviwo {

Python3QtModule::Python3QtModule() : InviwoModule() {
    setIdentifier("Python3Qt");
}

Python3QtModule::~Python3QtModule() {
    delete inviwoPyQtModule_;
    delete menu_;
}



void Python3QtModule::initialize() {
    InviwoModule::initialize();
    initPyQtModule();
    menu_ = new PythonMenu();
}

void Python3QtModule::deinitialize() {
    InviwoModule::deinitialize();
}

void Python3QtModule::initPyQtModule() {
    inviwoPyQtModule_ = new PyModule("inviwoqt");
    inviwoPyQtModule_->addMethod(new PyGetPathCurrentWorkspace());
    inviwoPyQtModule_->addMethod(new PyLoadNetworkMethod());
    inviwoPyQtModule_->addMethod(new PySaveNetworkMethod());
    inviwoPyQtModule_->addMethod(new PyQuitInviwoMethod());
    inviwoPyQtModule_->addMethod(new PyPromptMethod());
    inviwoPyQtModule_->addMethod(new PyShowPropertyWidgetMethod());
    PyInviwo::getPtr()->registerPyModule(inviwoPyQtModule_);
}

} // namespace
