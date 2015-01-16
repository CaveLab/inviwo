/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#ifndef IVW_PICKINGCALLBACK_H
#define IVW_PICKINGCALLBACK_H

#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class PickingObject;

class BasePickingCallBack {
public:
    BasePickingCallBack() {}
    virtual ~BasePickingCallBack() {}
    virtual void invoke(const PickingObject*) const=0;
};

template <typename T>
class MemberFunctionPickingCallback : public BasePickingCallBack {
public:
    MemberFunctionPickingCallback() {}
    virtual ~MemberFunctionPickingCallback() {}

    typedef void (T::*fPointerPicking)(const PickingObject*);

    MemberFunctionPickingCallback(T* obj, fPointerPicking functionPtr)
        : functionPtr_(functionPtr)
        , obj_(obj) {}

    void invoke(const PickingObject* p) const {
        if (functionPtr_)(*obj_.*functionPtr_)(p);
    }

private:
    fPointerPicking functionPtr_;
    T* obj_;
};

class PickingCallback {
public:
    PickingCallback() : callBack_(0) {}
    virtual ~PickingCallback() {
        delete callBack_;
    }


    void invoke(const PickingObject* p) const {
        if (callBack_)
            callBack_->invoke(p);
    }

    template <typename T>
    void addMemberFunction(T* o, void (T::*m)(const PickingObject*)) {
        if (callBack_)
            delete callBack_;

        callBack_ = new MemberFunctionPickingCallback<T>(o,m);
    }

private:
    BasePickingCallBack* callBack_;
};


} // namespace

#endif // IVW_PICKINGCALLBACK_H
