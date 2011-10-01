/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineFrustum.h
 *
 * @brief   Declaration of CPepeEngineFrustum class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-08-19
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEFRUSTUM_H
#define CPEPEENGINEFRUSTUM_H

#include "PepeEngineInit.h"

#include "CPepeEnginePlane.h"

_PEPE_ENGINE_START

/**
 *  Specifies perspective (realistic) or orthographic (architectural) projection.
 */
enum ProjectionType {
    PT_ORTHOGRAPHIC,
    PT_PERSPECTIVE
};

/**
 *  Worldspace clipping planes.
 */
enum FrustumPlane {
    FRUSTUM_PLANE_NEAR   = 0,
    FRUSTUM_PLANE_FAR    = 1,
    FRUSTUM_PLANE_LEFT   = 2,
    FRUSTUM_PLANE_RIGHT  = 3,
    FRUSTUM_PLANE_TOP    = 4,
    FRUSTUM_PLANE_BOTTOM = 5
};

class _PepeEngineExport CPepeEngineFrustum
{
    mutable CPepeEngineMatrix4  m_projectionMatrix;
    mutable CPepeEngineMatrix4  m_projectionMatrixRS;
    mutable CPepeEngineVector3  m_worldSpaceCorners[8];

    mutable bool m_bRecalcFrustumPlanes;

    void updateFrustum() const;
protected:
    mutable bool                m_bFrustumOutOfDate;
    mutable CPepeEnginePlane    m_frustumPlanes[6];
    ProjectionType              m_projectionType;
    Radian                      m_fFOVy;                /**< y-direction field-of-view default 45       */
    float                       m_fNearDist;            /**< near clip distance - default 100           */
    float                       m_fFarDist;             /**< far clip distance  - default 100000        */
    float                       m_fAspect;              /**< x/y ration - default 1.3333                */

    void updateFrustumPlanes(const CPepeEngineMatrix4& viewMatrix) const;
public:
    CPepeEngineFrustum();
    virtual ~CPepeEngineFrustum();

    void setFOVy(const Radian& fovy);
    inline void setFOVy(float fovy) {
        setFOVy(Degree(fovy));
    }

    void setNearClipDistance(float fNearDist);
    float getNearClipDistance() const {
        return m_fNearDist;
    }

    void setFarClipDistance(float fFarDist);
    float getFarClipDistance() const {
        return m_fFarDist;
    }

    void setAspectRatio(float fAspect);
    float getAspectRatio() const {
        return m_fAspect;
    }

    void setProjectionType(ProjectionType projectionType);
    ProjectionType getProjectionType() const {
        return m_projectionType;
    }

    virtual const CPepeEnginePlane* getFrustumPlanes(void);
    virtual const CPepeEnginePlane& getFrustumPlane(unsigned short nPlane);

    virtual bool isVisible(const CPepeEngineAxisAlignedBox& bound, FrustumPlane* culledBy = 0) const;
    virtual bool isVisible(const CPepeEngineSphere& sphere, FrustumPlane* culledBy = 0) const;
    virtual bool isVisible(const CPepeEngineVector3& vert, FrustumPlane* culledBy = 0) const;

    CPepeEngineMatrix4 getProjectionMatrix();
    CPepeEngineMatrix4 getProjectionMatrixRS();

    static const float INFINITE_FAR_PLANE_ADJUST;
};

_PEPE_ENGINE_END

#endif