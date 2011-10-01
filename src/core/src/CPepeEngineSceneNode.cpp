#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineSceneNode.h"
#include "CPepeEngineWireBoundingBox.h"

_PEPE_ENGINE_START


// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode::CPepeEngineSceneNode(const tstring& strName, CPepeEngineSceneManager* pSceneManager)
{
    assert(pSceneManager != NULL);

    m_strName               = strName;
    m_pSceneManager         = pSceneManager;
    m_nChildrenCount        = 0;
    m_pParent               = NULL;
    m_pObject               = NULL;
    m_pLight                = NULL;
    m_pWireBoundingBox      = NULL;

    m_worldPosition         = CPepeEngineVector3::ZERO;
    m_worldTransform        = CPepeEngineMatrix4::IDENTITY;
    m_translation           = CPepeEngineVector3::ZERO;
    m_scale                 = CPepeEngineVector3(1.0f, 1.0f, 1.0f);
    m_orientation           = CPepeEngineQuaternion::IDENTITY;

    m_derivedTranslation    = CPepeEngineVector3::ZERO;
    m_derivedScale          = CPepeEngineVector3(1.0f, 1.0f, 1.0f);
    m_derivedOrientation    = CPepeEngineQuaternion::IDENTITY;
    m_derivedPosition       = CPepeEngineVector3::ZERO;
    m_derivedTransform      = CPepeEngineMatrix4::IDENTITY;

    m_bInheritOrientation   = true;
    m_bInheritScale         = true;
    m_bVisible              = true;

    m_pSceneManager->addToNodeList(this);
    needUpdate();
    m_AABB.setNull();
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode::~CPepeEngineSceneNode()
{
    if (m_pObject != NULL) {
        delete m_pObject;
    }

    delete m_pWireBoundingBox;

    vector<CPepeEngineSceneNode *>::iterator it = m_children.begin();

    while (it != m_children.end()) {
        delete *it;
        *it = NULL;
        it++;
    }

    m_children.clear();
    m_pSceneManager->removeFromNodeList(getName());
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::attachObject(IPepeEngineRenderable* pObject)
{
    assert(pObject != NULL);
    // detach existing object
    detachObject();

    m_pObject   = pObject;
    m_AABB      = pObject->getAABB();

    if (m_AABB.isFinite()) {
        m_worldPosition = m_AABB.getCenter();
    }

    m_pObject->attachListener(this);
    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::detachObject(bool bRemoveObject)
{
    if (m_pObject != NULL) {
        if (bRemoveObject) {
            delete m_pObject;
        }

        m_AABB.setNull();

        m_pObject->detachListener(this);

        delete m_pWireBoundingBox;

        needUpdate();
    }
}

// -----------------------------------------------------------------------------------------
IPepeEngineRenderable* CPepeEngineSceneNode::getObject() const
{
    return m_pObject;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::addChildSceneNode(CPepeEngineSceneNode *pChild)
{
    pChild->setSceneManager(m_pSceneManager);
    pChild->setParentSceneNode(this);
    m_children.push_back(pChild);
    needUpdate();

    m_nChildrenCount++;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::removeChildSceneNode(CPepeEngineSceneNode *pChild)
{
    if (m_nChildrenCount == 0) {
        return;
    }

    vector<CPepeEngineSceneNode *>::iterator it = m_children.begin();

    for (unsigned int i = 0; i < m_nChildrenCount; i++) {
        if (pChild == m_children[i]) {
            m_children.erase(it + i);
            --m_nChildrenCount;
        }
    }

    needUpdate();
}

// -----------------------------------------------------------------------------------------
vector<CPepeEngineSceneNode *> CPepeEngineSceneNode::getChildSceneNodes() const
{
    return m_children;
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode* CPepeEngineSceneNode::getParentSceneNode() const
{
    return m_pParent;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setSceneManager(CPepeEngineSceneManager* pSceneManager)
{
    assert(pSceneManager != NULL);
    m_pSceneManager = pSceneManager;
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneManager* CPepeEngineSceneNode::getSceneManager() const
{
    return m_pSceneManager;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::render() const
{
    if (m_pObject != NULL && (m_bVisible && m_pSceneManager->getCamera()->isVisible(getAABB()))) {
        m_pSceneManager->m_autoParamDataSource.setCurrentRenderable(this);

        if (m_pSceneManager->isDebugMode()) {
            showWireBoundingBox();
        }

        CPepeEngineCore::getSingleton().getRenderer()->setWorldMatrix(getWorldTransforms());
        m_pObject->render();

        for (unsigned int i = 0; i < m_nChildrenCount; i++) {
            m_children[i]->render();
        }
    } else if (m_pObject == NULL) {
        for (unsigned int i = 0; i < m_nChildrenCount; i++) {
            m_children[i]->render();
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::addLight(CPepeEngineLight* pLight)
{
    assert(pLight != NULL);

    m_pSceneManager->addLight(pLight);
    m_pLight = pLight;
    pLight->setSceneNode(this);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::removeLight()
{
    if (m_pLight != NULL) {
        m_pSceneManager->removeLight(m_pLight);
        m_pLight = NULL;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::update() const
{
    updateFromParent();

    if (m_bNeedChildUpdate) {
        for (unsigned int i = 0; i < m_nChildrenCount; i++) {
            m_children[i]->needUpdate(true, false);
        }

        m_bNeedChildUpdate = false;
    }

    makeTransforms();
    updateBounds();

    if (m_pLight) {
        m_pLight->update(true);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::updateFromParent() const
{
    if (m_bNeedParentUpdate) {
        if (m_pParent) {

            const CPepeEngineQuaternion& qParent = m_pParent->getDerivedOrientation();

            if (m_bInheritOrientation) {
                m_derivedOrientation = qParent * m_orientation;
            } else {
                m_derivedOrientation = m_orientation;
            }

            const CPepeEngineVector3& vParent = m_pParent->getDerivedScale();

            if (m_bInheritScale) {
                m_derivedScale =  vParent * m_scale;
            } else {
                m_derivedScale = m_scale;
            }

            m_derivedTranslation = qParent * (vParent * m_translation);
            m_derivedTranslation += m_pParent->getDerivedTranslation();

        } else {
            m_derivedOrientation    = m_orientation;
            m_derivedScale          = m_scale;
            m_derivedTranslation    = m_translation;
        }

        m_bNeedParentUpdate = false;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::needUpdate(bool bChildNotify, bool bParentNotify) const
{
    m_bNeedParentUpdate     = true;
    m_bNeedChildUpdate      = bChildNotify;
    m_bNeedBoundsUpdate     = true;
    m_bNeedTransformUpdate  = true;
    update();

    if (bParentNotify && m_pParent) {
        m_pParent->needUpdate(false, true);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::updateBounds() const
{
    if (m_bNeedBoundsUpdate) {
        m_AABB.setNull();

        if (m_pObject) {
            m_AABB.merge(m_pObject->getAABB());
            m_AABB.transformAffine(m_worldTransform);
        }

        for (unsigned int i = 0; i < m_nChildrenCount; i++) {
            m_AABB.merge(m_children[i]->getAABB());
        }

        m_bNeedBoundsUpdate = false;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::makeTransforms() const
{
    if (m_bNeedTransformUpdate) {
        m_worldTransform.makeTransform(
            getDerivedTranslation(),
            getDerivedScale(),
            getDerivedOrientation()
        );
        m_bNeedTransformUpdate = false;
    }
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineSceneNode::getWorldPosition() const
{
    return m_worldPosition;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineSceneNode::getWorldTransforms() const
{
    return m_worldTransform;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineSceneNode::getDerivedTranslation() const
{
    updateFromParent();
    return m_derivedTranslation;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineSceneNode::getDerivedScale() const
{
    updateFromParent();
    return m_derivedScale;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineQuaternion& CPepeEngineSceneNode::getDerivedOrientation() const
{
    updateFromParent();
    return m_derivedOrientation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setPosition(const CPepeEngineVector3& position) const
{
    setTranslation(position);
    m_worldPosition = position;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setPosition(float x, float y, float z) const
{
    setPosition(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setTranslation(const CPepeEngineVector3& translation) const
{
    m_translation = translation;
    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setTranslation(float x, float y, float z) const
{
    setTranslation(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setScale(const CPepeEngineVector3& scale) const
{
    m_scale = scale;
    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setScale(float x, float y, float z) const
{
    setScale(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setOrientation(const CPepeEngineQuaternion& orientation) const
{
    m_orientation = orientation;
    m_orientation.normalise();
    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setOrientation(float angle, const CPepeEngineVector3& axis) const
{
    m_orientation.fromAngleAxis(Degree(angle), axis);
    needUpdate();
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3& CPepeEngineSceneNode::getTranslation() const
{
    return m_translation;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3& CPepeEngineSceneNode::getScale() const
{
    return m_scale;
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion& CPepeEngineSceneNode::getOrientation() const
{
    return m_orientation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::translate(float fX, float fY, float fZ, const TransformSpace& ts)
{
    translate(CPepeEngineVector3(fX, fY, fZ), ts);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::translate(const CPepeEngineVector3& v, const TransformSpace& ts)
{
    switch (ts) {
    case TS_LOCAL:
        m_translation += m_orientation * v;
        break;

    case TS_PARENT:
        m_translation += v;
        break;

    case TS_WORLD:
        if (m_pParent) {
            m_translation += (m_pParent->getDerivedOrientation().inverse() * v) / m_pParent->getDerivedScale();
        } else {
            m_translation += v;
        }

        break;
    }

    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::scale(float fX, float fY, float fZ)
{
    scale(CPepeEngineVector3(fX, fY, fZ));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::scale(const CPepeEngineVector3& v)
{
    m_scale = m_scale * v;
    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::rotate(const CPepeEngineVector3& v, const Radian& angle, const TransformSpace& ts)
{
    CPepeEngineQuaternion q;
    q.fromAngleAxis(angle, v);
    rotate(q, ts);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::rotate(const CPepeEngineQuaternion& q, const TransformSpace& ts)
{
    CPepeEngineQuaternion qnorm = q;
    qnorm.normalise();

    switch (ts) {
    case TS_LOCAL:
        m_orientation = m_orientation * qnorm;
        break;

    case TS_PARENT:
        m_orientation = qnorm * m_orientation;
        break;

    case TS_WORLD:
        m_orientation = m_orientation * getDerivedOrientation().inverse() * qnorm * getDerivedOrientation();
        break;
    }

    needUpdate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::roll(const Radian& angle, const TransformSpace& ts)
{
    rotate(CPepeEngineVector3::UNIT_Z, angle, ts);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::pitch(const Radian& angle, const TransformSpace& ts)
{
    rotate(CPepeEngineVector3::UNIT_X, angle, ts);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::yaw(const Radian& angle, const TransformSpace& ts)
{
    rotate(CPepeEngineVector3::UNIT_Y, angle, ts);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::showWireBoundingBox() const
{
    CPepeEngineCore::getSingleton().getRenderer()->setWorldMatrix(CPepeEngineMatrix4::IDENTITY);

    if (m_pWireBoundingBox == NULL) {
        m_pWireBoundingBox = new CPepeEngineWireBoundingBox();
    }

    m_pWireBoundingBox->loadDataFromAABB(getAABB());
    m_pWireBoundingBox->render();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::notify(const CPepeEngineEvent& event)
{
    switch (event.getEventType()) {
    case EVENT_OBJECT_CHANGED:
        needUpdate();
        break;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setInheritOrientation(bool bInheritOrientation)
{
    m_bInheritOrientation = bInheritOrientation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setInheritScale(bool bInheritScale)
{
    m_bInheritScale = bInheritScale;
}

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineSceneNode::getName() const
{
    return m_strName;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineSceneNode::isVisible() const
{
    return m_bVisible;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneNode::setParentSceneNode(CPepeEngineSceneNode *pParent)
{
    m_pParent = pParent;
}

_PEPE_ENGINE_END