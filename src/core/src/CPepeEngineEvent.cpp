#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineEvent.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineEvent::CPepeEngineEvent(const EventType& eventType)
{
    setEventType(eventType);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEvent::setEventType(const EventType& eventType)
{
    m_eventType = eventType;
}

// -----------------------------------------------------------------------------------------
const EventType& CPepeEngineEvent::getEventType() const
{
    return m_eventType;
}

// -----------------------------------------------------------------------------------------
CPepeEngineObservable::CPepeEngineObservable()
{
    m_listenerList.clear();
}

// -----------------------------------------------------------------------------------------
CPepeEngineObservable::~CPepeEngineObservable()
{
    m_listenerList.clear();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineObservable::attachListener(IPepeEngineListener* pListener)
{
    m_listenerList.push_back(pListener);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineObservable::detachListener(IPepeEngineListener* pListener)
{
    if (m_listenerList.size() == 0) {
        return;
    }

    ListenerList::iterator it = m_listenerList.begin(), endit = m_listenerList.end();

    while (it != endit) {
        if (*it == pListener) {
            m_listenerList.erase(it);
            break;
        }

        it++;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineObservable::notifyListeners(const CPepeEngineEvent& event) const
{
    if (m_listenerList.size() == 0) {
        return;
    }

    ListenerList::const_iterator it = m_listenerList.begin(), endit = m_listenerList.end();

    while (it != endit) {
        (*it)->notify(event);
        it++;
    }
}

_PEPE_ENGINE_END