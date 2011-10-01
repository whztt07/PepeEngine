/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineCamera.h
 *
 * @brief       Declaration of CPepeEngineCamera class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-07-30
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINECAMERA_H
#define CPEPEENGINECAMERA_H

#include "PepeEngineInit.h"

#include "CPepeEngineEntity.h"
#include "CPepeEngineFrustum.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineCamera : public CPepeEngineFrustum
{
public:
    CPepeEngineCamera();
    ~CPepeEngineCamera();

    void                                lookAt(const CPepeEngineVector3& point);
    void                                lookAt(float x, float y, float z);
    void                                lookAt(const IPepeEngineRenderable& entity);

    void                                rotate(const Radian& angle, const CPepeEngineVector3& axis);
    void                                rotate(const CPepeEngineQuaternion& q);

    void                                roll(const Radian& angle);
    void                                yaw(const Radian& angle);
    void                                pitch(const Radian& angle);

    void                                move(const CPepeEngineVector3& v);
    void                                moveRelative(const CPepeEngineVector3& v);

    CPepeEngineVector3                  getUp() const;
    CPepeEngineVector3                  getRight() const;

    inline const CPepeEngineVector3&    getPosition() const;
    inline void                         setPosition(const CPepeEngineVector3& position);
    void                                setPosition(float x, float y, float z);
    inline CPepeEngineVector3           getDirection() const;
    void                                setDirection(const CPepeEngineVector3& dir);
    void                                setDirection(float x, float y, float z);
    inline const CPepeEngineQuaternion& getOrientation() const;
    void                                setOrientation(const CPepeEngineQuaternion& orientation);

    void                                updateView() const;

    inline const CPepeEngineMatrix4&    getTransformMatrix() const;

    const CPepeEnginePlane*             getFrustumPlanes(void);
    const CPepeEnginePlane&             getFrustumPlane(unsigned short nPlane);

    inline bool                         isVisible(
        const CPepeEngineAxisAlignedBox& bound, FrustumPlane* culledBy = 0) const;
    inline bool                         isVisible(
        const CPepeEngineSphere& sphere, FrustumPlane* culledBy = 0) const;
    inline bool                         isVisible(
        const CPepeEngineVector3& vert, FrustumPlane* culledBy = 0) const;
private:
    mutable CPepeEngineVector3          m_position;
    mutable CPepeEngineMatrix4          m_viewMatrix;
    mutable CPepeEngineQuaternion       m_orientation;
    mutable bool                        m_bNeedViewUpdate;
};

_PEPE_ENGINE_END

#endif