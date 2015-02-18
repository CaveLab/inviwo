/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2015 Inviwo Foundation
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

#ifndef IVW_COORDINATETRANSFORMER_H
#define IVW_COORDINATETRANSFORMER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

template <unsigned int N>
class SpatialEntity;

template <unsigned int N>
class StructuredGridEntity;

template <unsigned int N>
class Camera {};

template <>
class Camera<3> {
public:
    Camera<3>(const CameraProperty* camera) : camera_(camera) {}

    const Matrix<4, float> getViewMatrix() const {
        return camera_->viewMatrix();
    }
    const Matrix<4, float> getProjectionMatrix() const {
        return camera_->projectionMatrix();
    }

private:
    const CameraProperty* camera_;
};

template <>
class Camera<2> {
public:
    Camera<2>(const CameraProperty* camera) {}

    const Matrix<3, float> getViewMatrix() const {
        return Matrix<3, float>(1.0f);
    }
    const Matrix<3, float> getProjectionMatrix() const {
        return Matrix<3, float>(1.0f);
    }
};

template<unsigned int N>
class SpatialCoordinateTransformer {
public:
    virtual ~SpatialCoordinateTransformer(){}
    virtual SpatialCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const = 0;
};

template<unsigned int N>
class StructuredCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    virtual ~StructuredCoordinateTransformer(){}
    virtual StructuredCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getModelToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getModelToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getTextureToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getDataToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getTextureToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getTextureToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to model space coordinates, i.e. from [0, number of voxels) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getIndexToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getIndexToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getIndexToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to world space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getIndexToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getWorldToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getWorldToIndexMatrix() const = 0;
};

template<unsigned int N>
class SpatialCameraCoordinateTransformer : public SpatialCoordinateTransformer<N> {
public:
    virtual ~SpatialCameraCoordinateTransformer(){}
    virtual SpatialCameraCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to model space coordinates, i.e. from [-1,1] to (data min, data max)
     */
    virtual const Matrix<N+1, float> getClipToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to raw data numbers, i.e. from [-1,1] to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getClipToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to view space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getClipToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to world space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getClipToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to clip space coordinates, i.e. from (data min, data max) to [-1,1]
     */
    virtual const Matrix<N+1, float> getModelToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to view space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to clip space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [-1,1]
     */
    virtual const Matrix<N+1, float> getDataToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to view space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getDataToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N+1, float> getViewToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getViewToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getViewToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to world space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getViewToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N+1, float> getWorldToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to view space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getWorldToViewMatrix() const = 0;
};

template<unsigned int N>
class StructuredCameraCoordinateTransformer : public SpatialCameraCoordinateTransformer<N> {
public:
    virtual ~StructuredCameraCoordinateTransformer(){}
    virtual StructuredCameraCoordinateTransformer<N>* clone() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to model space coordinates, i.e. from [-1,1] to (data min, data max)
     */
    virtual const Matrix<N+1, float> getClipToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to raw data numbers, i.e. from [-1,1] to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getClipToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to raw data numbers, i.e. from [-1,1] to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getClipToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to view space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getClipToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to voxel index coordinates, i.e. from [-1,1] to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getClipToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from clip space coordinates
     * to world space coordinates, i.e. from [-1,1] to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getClipToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to clip space coordinates, i.e. from (data min, data max) to [-1,1]
     */
    virtual const Matrix<N+1, float> getModelToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getModelToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to raw data numbers, i.e. from (data min, data max) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getModelToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to view space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to voxel index coordinates, i.e. from (data min, data max) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getModelToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from model space coordinates
     * to world space coordinates, i.e. from (data min, data max) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to clip space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [-1,1]
     */
    virtual const Matrix<N+1, float> getDataToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to clip space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [-1,1]
     */
    virtual const Matrix<N+1, float> getTextureToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getDataToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to model space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getTextureToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to view space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getDataToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to view space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getTextureToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getDataToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to voxel index coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getTextureToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from raw data numbers
     * to world space coordinates, i.e. from generally (-inf, inf), ([0,1] for textures) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getTextureToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N+1, float> getViewToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getViewToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getViewToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getViewToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getViewToIndexMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from view space coordinates
     * to world space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getViewToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to clip space coordinates, i.e. from [0, number of voxels) to [-1,1]
     */
    virtual const Matrix<N+1, float> getIndexToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to model space coordinates, i.e. from [0, number of voxels) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getIndexToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getIndexToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to raw data numbers, i.e. from [0, number of voxels) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getIndexToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to view space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getIndexToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from voxel index coordinates
     * to world space coordinates, i.e. from [0, number of voxels) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getIndexToWorldMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to clip space coordinates, i.e. from (-inf, inf) to [-1,1]
     */
    virtual const Matrix<N+1, float> getWorldToClipMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to model space coordinates, i.e. from (-inf, inf) to (data min, data max)
     */
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to raw data numbers, i.e. from (-inf, inf) to generally (-inf, inf), ([0,1] for textures)
     */
    virtual const Matrix<N+1, float> getWorldToTextureMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to view space coordinates, i.e. from (-inf, inf) to (-inf, inf)
     */
    virtual const Matrix<N+1, float> getWorldToViewMatrix() const = 0;
    /**
     * Returns the matrix transformation mapping from world space coordinates
     * to voxel index coordinates, i.e. from (-inf, inf) to [0, number of voxels)
     */
    virtual const Matrix<N+1, float> getWorldToIndexMatrix() const = 0;
};

template<unsigned int N>
class SpatialCoordinateTransformerImpl : public SpatialCoordinateTransformer<N> {
public:
    SpatialCoordinateTransformerImpl(const SpatialEntity<N>* entity);
    SpatialCoordinateTransformerImpl(const SpatialCoordinateTransformerImpl<N>& rhs);
    SpatialCoordinateTransformerImpl<N>& operator=(const SpatialCoordinateTransformerImpl<N>& that);
    virtual SpatialCoordinateTransformerImpl<N>* clone() const;
    virtual ~SpatialCoordinateTransformerImpl(){}

    void setEntity(const SpatialEntity<N>* entity);

    virtual const Matrix<N+1, float> getDataToModelMatrix() const;
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N+1, float> getModelToDataMatrix() const;
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const;

protected:
    virtual const Matrix<N+1, float> getModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldMatrix() const;

private:
    const SpatialEntity<N>* entity_;
};


template<unsigned int N>
class StructuredCoordinateTransformerImpl : public StructuredCoordinateTransformer<N> {
public:
    StructuredCoordinateTransformerImpl(const StructuredGridEntity<N>* entity);
    StructuredCoordinateTransformerImpl(const StructuredCoordinateTransformerImpl<N>& rhs);
    StructuredCoordinateTransformerImpl<N>& operator=(const StructuredCoordinateTransformerImpl<N>& that);
    virtual StructuredCoordinateTransformerImpl<N>* clone() const;
    virtual ~StructuredCoordinateTransformerImpl(){}

    void setEntity(const StructuredGridEntity<N>* entity);

    virtual const Matrix<N+1, float> getDataToIndexMatrix() const;
    virtual const Matrix<N+1, float> getDataToModelMatrix() const;
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N+1, float> getIndexToDataMatrix() const;
    virtual const Matrix<N+1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N+1, float> getIndexToTextureMatrix() const;
    virtual const Matrix<N+1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N+1, float> getModelToDataMatrix() const;
    virtual const Matrix<N+1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N+1, float> getModelToTextureMatrix() const;
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N+1, float> getTextureToIndexMatrix() const;
    virtual const Matrix<N+1, float> getTextureToModelMatrix() const;
    virtual const Matrix<N+1, float> getTextureToWorldMatrix() const;
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N+1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldToTextureMatrix() const;

protected:
    virtual const Matrix<N+1, float> getIndexMatrix() const;
    virtual const Matrix<N+1, float> getModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldMatrix() const;

private:
    const StructuredGridEntity<N>* entity_;
};


template<unsigned int N>
class SpatialCameraCoordinateTransformerImpl : public SpatialCameraCoordinateTransformer<N> {
public:
    SpatialCameraCoordinateTransformerImpl(const SpatialEntity<N>* entity, Camera<N> camera);
    SpatialCameraCoordinateTransformerImpl(const SpatialCameraCoordinateTransformerImpl<N>& rhs);
    SpatialCameraCoordinateTransformerImpl<N>& operator=(const SpatialCameraCoordinateTransformerImpl<N>& that);
    virtual SpatialCameraCoordinateTransformerImpl<N>* clone() const;
    virtual ~SpatialCameraCoordinateTransformerImpl(){}

    void setEntity(const SpatialEntity<N>* entity);
    void setCamera(Camera<N> camera);

    virtual const Matrix<N+1, float> getClipToDataMatrix() const;
    virtual const Matrix<N+1, float> getClipToModelMatrix() const;
    virtual const Matrix<N+1, float> getClipToViewMatrix() const;
    virtual const Matrix<N+1, float> getClipToWorldMatrix() const;
    virtual const Matrix<N+1, float> getDataToClipMatrix() const;
    virtual const Matrix<N+1, float> getDataToModelMatrix() const;
    virtual const Matrix<N+1, float> getDataToViewMatrix() const;
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N+1, float> getModelToClipMatrix() const;
    virtual const Matrix<N+1, float> getModelToDataMatrix() const;
    virtual const Matrix<N+1, float> getModelToViewMatrix() const;
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N+1, float> getViewToClipMatrix() const;
    virtual const Matrix<N+1, float> getViewToDataMatrix() const;
    virtual const Matrix<N+1, float> getViewToModelMatrix() const;
    virtual const Matrix<N+1, float> getViewToWorldMatrix() const;
    virtual const Matrix<N+1, float> getWorldToClipMatrix() const;
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldToViewMatrix() const;

protected:
    virtual const Matrix<N+1, float> getModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldMatrix() const;
    virtual const Matrix<N+1, float> getViewMatrix() const;
    virtual const Matrix<N+1, float> getProjectionMatrix() const;

private:
    const SpatialEntity<N>* entity_;
    Camera<N> camera_;
};


template<unsigned int N>
class StructuredCameraCoordinateTransformerImpl : public StructuredCameraCoordinateTransformer<N> {
public:
    StructuredCameraCoordinateTransformerImpl(const StructuredGridEntity<N>* entity, Camera<N> camera);
    StructuredCameraCoordinateTransformerImpl(const StructuredCameraCoordinateTransformerImpl<N>& rhs);
    StructuredCameraCoordinateTransformerImpl<N>& operator=(const StructuredCameraCoordinateTransformerImpl<N>& that);
    virtual StructuredCameraCoordinateTransformerImpl<N>* clone() const;
    virtual ~StructuredCameraCoordinateTransformerImpl(){}

    void setEntity(const StructuredGridEntity<N>* entity);
    void setCamera(Camera<N> camera);

    virtual const Matrix<N+1, float> getClipToDataMatrix() const;
    virtual const Matrix<N+1, float> getClipToIndexMatrix() const;
    virtual const Matrix<N+1, float> getClipToModelMatrix() const;
    virtual const Matrix<N+1, float> getClipToTextureMatrix() const;
    virtual const Matrix<N+1, float> getClipToViewMatrix() const;
    virtual const Matrix<N+1, float> getClipToWorldMatrix() const;
    virtual const Matrix<N+1, float> getDataToClipMatrix() const;
    virtual const Matrix<N+1, float> getDataToIndexMatrix() const;
    virtual const Matrix<N+1, float> getDataToModelMatrix() const;
    virtual const Matrix<N+1, float> getDataToViewMatrix() const;
    virtual const Matrix<N+1, float> getDataToWorldMatrix() const;
    virtual const Matrix<N+1, float> getIndexToClipMatrix() const;
    virtual const Matrix<N+1, float> getIndexToDataMatrix() const;
    virtual const Matrix<N+1, float> getIndexToModelMatrix() const;
    virtual const Matrix<N+1, float> getIndexToTextureMatrix() const;
    virtual const Matrix<N+1, float> getIndexToViewMatrix() const;
    virtual const Matrix<N+1, float> getIndexToWorldMatrix() const;
    virtual const Matrix<N+1, float> getModelToClipMatrix() const;
    virtual const Matrix<N+1, float> getModelToDataMatrix() const;
    virtual const Matrix<N+1, float> getModelToIndexMatrix() const;
    virtual const Matrix<N+1, float> getModelToTextureMatrix() const;
    virtual const Matrix<N+1, float> getModelToViewMatrix() const;
    virtual const Matrix<N+1, float> getModelToWorldMatrix() const;
    virtual const Matrix<N+1, float> getTextureToClipMatrix() const;
    virtual const Matrix<N+1, float> getTextureToIndexMatrix() const;
    virtual const Matrix<N+1, float> getTextureToModelMatrix() const;
    virtual const Matrix<N+1, float> getTextureToViewMatrix() const;
    virtual const Matrix<N+1, float> getTextureToWorldMatrix() const;
    virtual const Matrix<N+1, float> getViewToClipMatrix() const;
    virtual const Matrix<N+1, float> getViewToDataMatrix() const;
    virtual const Matrix<N+1, float> getViewToIndexMatrix() const;
    virtual const Matrix<N+1, float> getViewToModelMatrix() const;
    virtual const Matrix<N+1, float> getViewToTextureMatrix() const;
    virtual const Matrix<N+1, float> getViewToWorldMatrix() const;
    virtual const Matrix<N+1, float> getWorldToClipMatrix() const;
    virtual const Matrix<N+1, float> getWorldToDataMatrix() const;
    virtual const Matrix<N+1, float> getWorldToIndexMatrix() const;
    virtual const Matrix<N+1, float> getWorldToModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldToTextureMatrix() const;
    virtual const Matrix<N+1, float> getWorldToViewMatrix() const;

protected:
    virtual const Matrix<N+1, float> getIndexMatrix() const;
    virtual const Matrix<N+1, float> getModelMatrix() const;
    virtual const Matrix<N+1, float> getWorldMatrix() const;
    virtual const Matrix<N+1, float> getViewMatrix() const;
    virtual const Matrix<N+1, float> getProjectionMatrix() const;

private:
    const StructuredGridEntity<N>* entity_;
    Camera<N> camera_;
};


/*********************************************************************************
 *  Implementations
 *  SpatialCoordinateTransformerImpl
 *********************************************************************************/

template<unsigned int N>
SpatialCoordinateTransformerImpl<N>::SpatialCoordinateTransformerImpl(const SpatialEntity<N>* entity)
    : SpatialCoordinateTransformer<N>()
    , entity_(entity) {}

template<unsigned int N>
SpatialCoordinateTransformerImpl<N>::SpatialCoordinateTransformerImpl(const SpatialCoordinateTransformerImpl<N>& rhs)
    : SpatialCoordinateTransformer<N>(rhs)
    , entity_(rhs.entity_) {}

template<unsigned int N>
SpatialCoordinateTransformerImpl<N>& SpatialCoordinateTransformerImpl<N>::operator=(const SpatialCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        SpatialCoordinateTransformer<N>::operator=(that);
        entity_ = that.entity_;
    }
    return *this;
}

template<unsigned int N>
SpatialCoordinateTransformerImpl<N>* SpatialCoordinateTransformerImpl<N>::clone() const {
    return new SpatialCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getModelMatrix() const {
    return entity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return entity_->getWorldMatrix();
}

template <unsigned int N>
void SpatialCoordinateTransformerImpl<N>::setEntity(const SpatialEntity<N>* entity) {
    entity_ = entity;
}

template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
	return getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
	return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
	return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
	return MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
	return MatrixInvert(getWorldMatrix());
}

/*********************************************************************************
 *  Implementations
 *  StructuredCoordinateTransformerImpl
 *********************************************************************************/

template<unsigned int N>
StructuredCoordinateTransformerImpl<N>::StructuredCoordinateTransformerImpl(const StructuredGridEntity<N>* entity)
    : StructuredCoordinateTransformer<N>()
    , entity_(entity) {}

template<unsigned int N>
StructuredCoordinateTransformerImpl<N>::StructuredCoordinateTransformerImpl(const StructuredCoordinateTransformerImpl<N>& rhs)
    : StructuredCoordinateTransformer<N>(rhs)
    , entity_(rhs.entity_) {}

template<unsigned int N>
StructuredCoordinateTransformerImpl<N>& StructuredCoordinateTransformerImpl<N>::operator=(const StructuredCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        StructuredCoordinateTransformer<N>::operator=(that);
        entity_ = that.entity_;
    }
    return *this;
}

template<unsigned int N>
StructuredCoordinateTransformerImpl<N>* StructuredCoordinateTransformerImpl<N>::clone() const {
    return new StructuredCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getIndexMatrix() const {
    return entity_->getIndexMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getModelMatrix() const {
    return entity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return entity_->getWorldMatrix();
}

template <unsigned int N>
void StructuredCoordinateTransformerImpl<N>::setEntity(const StructuredGridEntity<N>* entity) {
    entity_ = entity;
}

template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getDataToIndexMatrix() const {
	return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
	return getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getIndexToDataMatrix() const {
	return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getIndexToModelMatrix() const {
	return getModelMatrix()*MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getIndexToTextureMatrix() const {
	return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getIndexToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix()*MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
	return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getModelToIndexMatrix() const {
	return getIndexMatrix()*MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getModelToTextureMatrix() const {
	return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
	return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getTextureToIndexMatrix() const {
	return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getTextureToModelMatrix() const {
	return getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getTextureToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
	return MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getWorldToIndexMatrix() const {
	return getIndexMatrix()*MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
	return MatrixInvert(getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCoordinateTransformerImpl<N>::getWorldToTextureMatrix() const {
	return MatrixInvert(getWorldMatrix()*getModelMatrix());
}

/*********************************************************************************
 *  Implementations
 *  SpatialCameraCoordinateTransformerImpl
 *********************************************************************************/

template<unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>::SpatialCameraCoordinateTransformerImpl(const SpatialEntity<N>* entity, Camera<N> camera)
    : SpatialCameraCoordinateTransformer<N>()
    , entity_(entity)
    , camera_(camera) {}

template<unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>::SpatialCameraCoordinateTransformerImpl(const SpatialCameraCoordinateTransformerImpl<N>& rhs)
    : SpatialCameraCoordinateTransformer<N>(rhs)
    , entity_(rhs.entity_)
    , camera_(rhs.camera_) {}

template<unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>& SpatialCameraCoordinateTransformerImpl<N>::operator=(const SpatialCameraCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        SpatialCameraCoordinateTransformer<N>::operator=(that);
        entity_ = that.entity_;
        camera_ = that.camera_;
    }
    return *this;
}

template<unsigned int N>
SpatialCameraCoordinateTransformerImpl<N>* SpatialCameraCoordinateTransformerImpl<N>::clone() const {
    return new SpatialCameraCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelMatrix() const {
    return entity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return entity_->getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewMatrix() const {
    return camera_.getViewMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getProjectionMatrix() const {
    return camera_.getProjectionMatrix();
}

template <unsigned int N>
void SpatialCameraCoordinateTransformerImpl<N>::setEntity(const SpatialEntity<N>* entity) {
    entity_ = entity;
}
template <unsigned int N>
void SpatialCameraCoordinateTransformerImpl<N>::setCamera(Camera<N> camera) {
    camera_ = camera;
}

template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToDataMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToModelMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix()*getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToViewMatrix() const {
	return MatrixInvert(getProjectionMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getClipToWorldMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
	return getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToViewMatrix() const {
	return getViewMatrix()*getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix()*getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
	return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToViewMatrix() const {
	return getViewMatrix()*getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
	return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToClipMatrix() const {
	return getProjectionMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToDataMatrix() const {
	return MatrixInvert(getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToModelMatrix() const {
	return MatrixInvert(getViewMatrix()*getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getViewToWorldMatrix() const {
	return MatrixInvert(getViewMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
	return MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
	return MatrixInvert(getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> SpatialCameraCoordinateTransformerImpl<N>::getWorldToViewMatrix() const {
	return getViewMatrix();
}

/*********************************************************************************
 *  Implementations
 *  StructuredCameraCoordinateTransformerImpl
 *********************************************************************************/

template<unsigned int N>
StructuredCameraCoordinateTransformerImpl<N>::StructuredCameraCoordinateTransformerImpl(const StructuredGridEntity<N>* entity, Camera<N> camera)
    : StructuredCameraCoordinateTransformer<N>()
    , entity_(entity)
    , camera_(camera) {}

template<unsigned int N>
StructuredCameraCoordinateTransformerImpl<N>::StructuredCameraCoordinateTransformerImpl(const StructuredCameraCoordinateTransformerImpl<N>& rhs)
    : StructuredCameraCoordinateTransformer<N>(rhs)
    , entity_(rhs.entity_)
    , camera_(rhs.camera_) {}

template<unsigned int N>
StructuredCameraCoordinateTransformerImpl<N>& StructuredCameraCoordinateTransformerImpl<N>::operator=(const StructuredCameraCoordinateTransformerImpl<N>& that) {
    if (this != &that) {
        StructuredCameraCoordinateTransformer<N>::operator=(that);
        entity_ = that.entity_;
        camera_ = that.camera_;
    }
    return *this;
}

template<unsigned int N>
StructuredCameraCoordinateTransformerImpl<N>* StructuredCameraCoordinateTransformerImpl<N>::clone() const {
    return new StructuredCameraCoordinateTransformerImpl<N>(*this);
}

template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexMatrix() const {
    return entity_->getIndexMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelMatrix() const {
    return entity_->getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldMatrix() const {
    return entity_->getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewMatrix() const {
    return camera_.getViewMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getProjectionMatrix() const {
    return camera_.getProjectionMatrix();
}

template <unsigned int N>
void StructuredCameraCoordinateTransformerImpl<N>::setEntity(const StructuredGridEntity<N>* entity) {
    entity_ = entity;
}
template <unsigned int N>
void StructuredCameraCoordinateTransformerImpl<N>::setCamera(Camera<N> camera) {
    camera_ = camera;
}

template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getClipToDataMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getClipToIndexMatrix() const {
	return getIndexMatrix()*MatrixInvert(getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getClipToModelMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix()*getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getClipToTextureMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getClipToViewMatrix() const {
	return MatrixInvert(getProjectionMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getClipToWorldMatrix() const {
	return MatrixInvert(getProjectionMatrix()*getViewMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getDataToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getDataToIndexMatrix() const {
	return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getDataToModelMatrix() const {
	return getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getDataToViewMatrix() const {
	return getViewMatrix()*getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getDataToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix()*MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexToDataMatrix() const {
	return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexToModelMatrix() const {
	return getModelMatrix()*MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexToTextureMatrix() const {
	return MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexToViewMatrix() const {
	return getViewMatrix()*getWorldMatrix()*getModelMatrix()*MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getIndexToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix()*MatrixInvert(getIndexMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix()*getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelToDataMatrix() const {
	return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelToIndexMatrix() const {
	return getIndexMatrix()*MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelToTextureMatrix() const {
	return MatrixInvert(getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelToViewMatrix() const {
	return getViewMatrix()*getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getModelToWorldMatrix() const {
	return getWorldMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getTextureToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix()*getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getTextureToIndexMatrix() const {
	return getIndexMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getTextureToModelMatrix() const {
	return getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getTextureToViewMatrix() const {
	return getViewMatrix()*getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getTextureToWorldMatrix() const {
	return getWorldMatrix()*getModelMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewToClipMatrix() const {
	return getProjectionMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewToDataMatrix() const {
	return MatrixInvert(getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewToIndexMatrix() const {
	return getIndexMatrix()*MatrixInvert(getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewToModelMatrix() const {
	return MatrixInvert(getViewMatrix()*getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewToTextureMatrix() const {
	return MatrixInvert(getViewMatrix()*getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getViewToWorldMatrix() const {
	return MatrixInvert(getViewMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldToClipMatrix() const {
	return getProjectionMatrix()*getViewMatrix();
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldToDataMatrix() const {
	return MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldToIndexMatrix() const {
	return getIndexMatrix()*MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldToModelMatrix() const {
	return MatrixInvert(getWorldMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldToTextureMatrix() const {
	return MatrixInvert(getWorldMatrix()*getModelMatrix());
}
template <unsigned int N>
const Matrix<N+1, float> StructuredCameraCoordinateTransformerImpl<N>::getWorldToViewMatrix() const {
	return getViewMatrix();
}

} // namespace

#endif // IVW_COORDINATETRANSFORMER_H