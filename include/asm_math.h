/**
* Project:		PepeEngine
* Tier:			Frontend
* File:			asm_math.h   
*
* @brief		Declaration of math function writen in assembler.
*
* @author		Piotr 'pepe' Picheta
* @date			2008-07-22
* @copyright	Copyright (c) 2008 Piotr Picheta
*
* @version		1.0
*/

#ifndef ASM_MATH_H
#define ASM_MATH_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
float __fastcall asm_invSqrt(float r)
{
	#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC &&  PEPE_ENGINE_ARCH_TYPE == PEPE_ENGINE_ARCHITECTURE_32
		__asm 
		{
			fld1	// r0 = 1.f
			fld r	// r1 = r0, r0 = r
			fsqrt	// r0 = sqrtf( r0 )
			fdiv	// r0 = r1 / r0
		}			// returns r0
	#else
		return 1. / sqrt(r);
	#endif
}

// -----------------------------------------------------------------------------------------
float asm_sqrt(float r)
{
	#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC &&  PEPE_ENGINE_ARCH_TYPE == PEPE_ENGINE_ARCHITECTURE_32
		__asm 
		{
			fld r	// r0 = r
			fsqrt	// r0 = sqrtf( r0 )
		}			// returns r0

	#else
		return sqrt(r);
	#endif
}

// -----------------------------------------------------------------------------------------
__forceinline float asm_rand()
{
	#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC &&  PEPE_ENGINE_ARCH_TYPE == PEPE_ENGINE_ARCHITECTURE_32	
		#if PEPE_ENGINE_COMP_VER >= 1300

			static unsigned __int64 q = time( NULL );

			_asm 
			{
				movq mm0, q

				// do the magic MMX thing
				pshufw mm1, mm0, 0x1E
				paddd mm0, mm1

				// move to integer memory location and free MMX
				movq q, mm0
				emms
			}

			return float( q );	
		#else		
			return float(rand());
		#endif
	#else	
		return float(rand());
	#endif
}

// -----------------------------------------------------------------------------------------
__forceinline float asm_rand_max()
{
	#if  PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC &&  PEPE_ENGINE_ARCH_TYPE == PEPE_ENGINE_ARCHITECTURE_32
		#if PEPE_ENGINE_COMP_VER >= 1300
			return float(std::numeric_limits< unsigned __int64 >::max());				
		#else
			// VC6 does not support unsigned __int64
			return float(RAND_MAX);
		#endif
	#else	
		return float(RAND_MAX);
	#endif
}

_PEPE_ENGINE_END

#endif