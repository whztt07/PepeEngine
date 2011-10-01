/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineTimer.h
 *
 * @brief   Declaration of IPepeEngineTimer interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-24
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINETIMER_H
#define IPEPEENGINETIMER_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineTimer
{
public:

    /**
     *  Resets timer
     */
    virtual void reset() = 0;

    /**
     *  Returns milliseconds since initialisation or last reset
     */
    virtual unsigned long getMilliseconds() = 0;

    /**
     *  Returns microseconds since initialisation or last reset
     */
    virtual unsigned long getMicroseconds() = 0;

    /**
     *  Returns milliseconds since initialisation or last reset, only CPU time measured
     */
    virtual unsigned long getMillisecondsCPU() = 0;

    /**
     *  Returns microseconds since initialisation or last reset, only CPU time measured
     */
    virtual unsigned long getMicrosecondsCPU() = 0;

};

_PEPE_ENGINE_END

#endif