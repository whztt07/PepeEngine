/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineSceneNode.h 
 *
 * @brief		Declaration of CPepeEngineSceneNode class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-02-28
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINESCENENODE_H
#define CPEPEENGINESCENENODE_H

#include "PepeEngineInit.h"

#include "CPepeEngineEvent.h"
#include "CPepeEngineLight.h"
#include "CPepeEngineSceneManager.h"
#include "IPepeEngineRenderable.h"

_PEPE_ENGINE_START

/** 
 *	Enumeration denoting the spaces which a transform can be relative to.
 */
enum TransformSpace
{	
	TS_LOCAL,	/**<	Transform is relative to the local space				*/
	TS_PARENT,	/**<	Transform is relative to the space of the parent node	*/
	TS_WORLD	/**<	Transform is relative to world space					*/
};

class _PepeEngineExport CPepeEngineSceneNode : public IPepeEngineRenderable, public IPepeEngineListener
{			
public:	
										CPepeEngineSceneNode(const tstring& strName, CPepeEngineSceneManager* pSceneManager);
										~CPepeEngineSceneNode();

	void								attachObject(IPepeEngineRenderable* pObject);	
	void								detachObject(bool bRemoveObject = true);
	inline IPepeEngineRenderable*		getObject() const;

	void								addChildSceneNode(CPepeEngineSceneNode *pChild);
	void								removeChildSceneNode(CPepeEngineSceneNode *pChild);
	std::vector<CPepeEngineSceneNode *> getChildSceneNodes() const;

	CPepeEngineSceneNode*				getParentSceneNode() const;	

	inline void							setSceneManager(CPepeEngineSceneManager* pSceneManager);
	inline CPepeEngineSceneManager*		getSceneManager() const;

	void								render() const;

	void								addLight(CPepeEngineLight* pLight);
	void								removeLight();

	void								update() const;	
	void								updateFromParent() const;	
	void								needUpdate(bool bChildNotify = true, bool bParentNotify = true) const;
	void								updateBounds() const;	

	inline void							makeTransforms() const;

	inline const CPepeEngineVector3&	getWorldPosition() const;
	inline const CPepeEngineMatrix4&	getWorldTransforms() const;	
	
	inline const CPepeEngineVector3&	getDerivedTranslation() const;
	inline const CPepeEngineVector3&	getDerivedScale() const;
	inline const CPepeEngineQuaternion&	getDerivedOrientation() const;

	inline void							setPosition(const CPepeEngineVector3& position) const;
	inline void							setPosition(float x, float y, float z) const;
	inline void							setTranslation(const CPepeEngineVector3& translation) const;
	inline void							setTranslation(float x, float y, float z) const;
	inline void							setScale(const CPepeEngineVector3& scale) const;
	inline void							setScale(float x, float y, float z) const;
	inline void							setOrientation(const CPepeEngineQuaternion& rotation) const;
	inline void							setOrientation(float angle, const CPepeEngineVector3& axis) const;

	inline CPepeEngineVector3&			getTranslation() const;
	inline CPepeEngineVector3&			getScale() const;
	inline CPepeEngineQuaternion&		getOrientation() const;

	void								translate(float fX, float fY, float fZ, const TransformSpace& ts = TS_LOCAL);
	void								translate(const CPepeEngineVector3& v, const TransformSpace& ts = TS_LOCAL);

	void								scale(float fX, float fY, float fZ);
	void								scale(const CPepeEngineVector3& v);

	void								rotate(const CPepeEngineVector3& v, const Radian& angle, const TransformSpace& ts = TS_LOCAL);
	void								rotate(const CPepeEngineQuaternion& q, const TransformSpace& ts = TS_LOCAL); 	
	void								roll(const Radian& angle, const TransformSpace& ts = TS_LOCAL);	
	void								pitch(const Radian& angle, const TransformSpace& ts = TS_LOCAL);
	void								yaw(const Radian& angle, const TransformSpace& ts = TS_LOCAL);

	void								showWireBoundingBox() const;	

	void								notify(const CPepeEngineEvent& event);

	void								setInheritOrientation(bool bInheritOrientation);
	void								setInheritScale(bool bInheritScale);

	inline const tstring&				getName() const;	

	inline void							setVisible(bool bVisible);
	inline bool							isVisible() const;
private:	
	tstring								m_strName;
	CPepeEngineSceneManager*			m_pSceneManager;
	CPepeEngineSceneNode*				m_pParent;

	std::vector<CPepeEngineSceneNode *>	m_children;
	unsigned int						m_nChildrenCount;	

	mutable CPepeEngineVector3			m_derivedPosition;
	mutable CPepeEngineVector3			m_derivedTranslation;
	mutable CPepeEngineVector3			m_derivedScale;
	mutable CPepeEngineQuaternion		m_derivedOrientation;
	mutable CPepeEngineMatrix4			m_derivedTransform;

	mutable CPepeEngineVector3			m_worldPosition;
	mutable CPepeEngineVector3			m_translation;
	mutable CPepeEngineVector3			m_scale;
	mutable CPepeEngineQuaternion		m_orientation;
	mutable CPepeEngineMatrix4			m_worldTransform;

	mutable bool						m_bNeedChildUpdate;	
	mutable bool						m_bNeedParentUpdate;	
	mutable bool						m_bNeedTransformUpdate;	
	mutable bool						m_bNeedBoundsUpdate;	

	bool								m_bInheritOrientation;
	bool								m_bInheritScale;

	bool								m_bVisible;

	IPepeEngineRenderable*				m_pObject;
	CPepeEngineLight*					m_pLight;				

	mutable CPepeEngineWireBoundingBox*	m_pWireBoundingBox;

	void								setParentSceneNode(CPepeEngineSceneNode *pParent);
};

_PEPE_ENGINE_END

#endif