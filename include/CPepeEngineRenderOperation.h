/**
* Project:	PepeEngine
* Tier:		Frontend
* File:		CPepeEngineRenderOperation.h   
*
* @brief	Declaration of CPepeEngineRenderOperation class.
*
* @author		Piotr 'pepe' Picheta
* @date			2008-07-07
* @copyright	Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef CPEPEENGINERENDEROPERATION_H
#define CPEPEENGINERENDEROPERATION_H

#include "PepeEngineInit.h"

#include "CPepeEngineIndexData.h"
#include "CPepeEngineVertexData.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineRenderOperation
{	
public:
	
	/** The rendering operation type to perform */
	enum OperationType 
	{		
		OT_POINT_LIST		= 1,	/**< A list of points, 1 vertex per point													*/	
		OT_LINE_LIST		= 2,	/**< A list of lines, 2 vertices per line													*/		
		OT_LINE_STRIP		= 3,	/**< A strip of connected lines, 1 vertex per line plus 1 start vertex						*/		
		OT_TRIANGLE_LIST	= 4,	/**< A list of triangles, 3 vertices per triangle											*/		
		OT_TRIANGLE_STRIP	= 5,	/**< A strip of triangles, 3 vertices for the first triangle, and 1 per triangle after that	*/ 		
		OT_TRIANGLE_FAN		= 6		/**< A fan of triangles, 3 vertices for the first triangle, and 1 per triangle after that	*/
	};

	/** Vertex source data */
	CPepeEngineVertexData *m_pVertexData;	

	/** The type of operation to perform */
	OperationType m_operationType;

	/** 
	 *	Specifies whether to use indexes to determine the vertices to use as input. If false, the vertices are
	 *	simply read in sequence to define the primitives. If true, indexes are used instead to identify vertices
	 *	anywhere in the buffer, and allowing vertices to be used more than once.
	 *	If true, then the indexBuffer, indexStart and numIndexes properties must be valid. 
	 */
	bool m_bUseIndexes;

	/** Index data - only valid if bUseIndexes is true */
	CPepeEngineIndexData *m_pIndexData;


	CPepeEngineRenderOperation() : 
	m_pVertexData(NULL), m_operationType(OT_TRIANGLE_LIST), m_bUseIndexes(true), m_pIndexData(NULL) {}

	~CPepeEngineRenderOperation();
};

_PEPE_ENGINE_END

#endif