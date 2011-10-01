/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineLight.h
 *
 * @brief       Declaration of CPepeEngineLight class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-08-23
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINELIGHT_H
#define CPEPEENGINELIGHT_H

#include "PepeEngineInit.h"

#include "CPepeEngineColor.h"
#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

enum LightType {
    LT_POINT,       /**<    Point light sources give off light equally in all directions, so require only position not direction                */
    LT_DIRECTIONAL, /**<    Directional lights simulate parallel light beams from a distant source, hence have direction but no position        */
    LT_SPOTLIGHT    /**<    Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff  */
};

typedef std::vector<CPepeEngineLight*> LightList;

class _PepeEngineExport CPepeEngineLight
{
public:
    CPepeEngineLight(const tstring& strName);
    ~CPepeEngineLight();

    inline void                         setLightType(const LightType& lightType);
    inline const LightType&             getLightType() const;

    inline void                         setDiffuse(const CPepeEngineColor& diffuse);
    inline void                         setDiffuse(float r, float g, float b);
    inline const CPepeEngineColor&      getDiffuse() const;

    inline void                         setSpecular(const CPepeEngineColor& specular);
    inline void                         setSpecular(float r, float g, float b);
    inline const CPepeEngineColor&      getSpecular() const;

    inline void                         setAmbient(const CPepeEngineColor& ambient);
    inline void                         setAmbient(float r, float g, float b);
    inline const CPepeEngineColor&      getAmbient() const;

    inline void                         setPosition(const CPepeEngineVector3& position);
    inline void                         setPosition(float x, float y, float z);
    inline const CPepeEngineVector3&    getPosition() const;

    inline void                         setDirection(const CPepeEngineVector3& direction);
    inline void                         setDirection(float x, float y, float z);
    inline const CPepeEngineVector3&    getDirection() const;

    inline const CPepeEngineVector3&    getDerivedPosition() const;
    inline const CPepeEngineVector3&    getDerivedDirection() const;

    inline void                         setRange(float fRange);
    inline float                        getRange() const;

    inline void                         setFalloff(float fFalloff);
    inline float                        getFalloff() const;

    inline void                         setConstantAttenuation(float fConstantAttenuation);
    inline float                        getConstantAttenuation() const;

    inline void                         setLinearAttenuation(float fLinearAttenuation);
    inline float                        getLinearAttenuation() const;

    inline void                         setQuadricAttenuation(float fQuadricAttenuation);
    inline float                        getQuadricAttenuation() const;

    inline void                         setTheta(const Radian& theta);
    inline const Radian&                getTheta() const;

    inline void                         setPhi(const Radian& phi);
    inline const Radian&                getPhi() const;

    inline bool                         isVisible() const;

    inline void                         setVisible(bool bVisible);

    inline const tstring&               getName();
    inline void                         setName(const tstring& strName);

    inline void                         setSceneNode(CPepeEngineSceneNode* pSceneNode);
    inline CPepeEngineSceneNode*        getSceneNode() const;

    void                                update(bool bForceUpdate = false) const;

private:
    CPepeEngineSceneNode*               m_pParentNode;

    LightType                           m_lightType;
    CPepeEngineColor                    m_diffuse;
    CPepeEngineColor                    m_specular;
    CPepeEngineColor                    m_ambient;

    mutable CPepeEngineVector3          m_position;
    mutable CPepeEngineVector3          m_direction;
    mutable CPepeEngineVector3          m_derivedPosition;
    mutable CPepeEngineVector3          m_derivedDirection;

    float                               m_fRange;
    float                               m_fFalloff;
    float                               m_fConstantAttenuation;
    float                               m_fLinearAttenuation;
    float                               m_fQuadricAttenuation;
    Radian                              m_theta;                    /**<    Inner angle of spotlight cone   */
    Radian                              m_phi;                      /**<    Outer angle of spotlight cone   */

    bool                                m_bVisible;
    mutable bool                        m_bNeedUpdate;

    tstring                             m_strName;
};

_PEPE_ENGINE_END

#endif