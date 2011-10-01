/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineEvent.h
 *
 * @brief       Declaration of event system classes.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-12-02
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINEEVENT_H
#define CPEPEENGINEEVENT_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

enum EventType {
    EVENT_OBJECT_CHANGED = 0,
    EVENT_CONFIG_CHANGED = 1
};

class _PepeEngineExport CPepeEngineEvent
{
public:
    CPepeEngineEvent(const EventType& eventType);
    inline void             setEventType(const EventType& eventType);
    inline const EventType& getEventType() const;
protected:
    EventType m_eventType;
};

class _PepeEngineExport IPepeEngineListener
{
public:
    virtual void notify(const CPepeEngineEvent& event) = 0;
};

class _PepeEngineExport CPepeEngineObservable
{
public:
    CPepeEngineObservable();
    virtual                                     ~CPepeEngineObservable();
    void                                        attachListener(IPepeEngineListener* pListener);
    void                                        detachListener(IPepeEngineListener* pListener);
    void                                        notifyListeners(const CPepeEngineEvent& event) const;
protected:
    typedef std::vector<IPepeEngineListener* >  ListenerList;
    ListenerList                                m_listenerList;
};

_PEPE_ENGINE_END

#endif