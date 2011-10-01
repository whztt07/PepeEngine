#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineColor.h"
#include "CPepeEngineMaterial.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMesh.h"
#include "CPepeEngineSubMesh.h"
#include "CPepeEngineXFileMeshLoader.h"
#include "IPepeEngineBufferManager.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

#define TEXT_BUFFER     255
#define MAX_TEMPLATES   15

struct XOF_TEMPLATEID {
    char*               strTxtID;
    unsigned short int  unTemplateID;
};

XOF_TEMPLATEID Templates[MAX_TEMPLATES] = {
    {"template" ,               X_TEMPLATE},
    {"FrameTransformMatrix",    X_FRAMETRANSFORMMATRIX},
    {"Frame",                   X_FRAME},
    {"XSkinMeshHeader",         X_SKINMESHHEADER},
    {"MeshTextureCoords",       X_MESHTEXTURECOORDS},
    {"MeshMaterialList",        X_MESHMATERIALLIST},
    {"MeshNormals",             X_MESHNORMALS},
    {"Mesh",                    X_MESH},
    {"Material",                X_MATERIAL},
    {"SkinWeights",             X_SKINWEIGHTS},
    {"TextureFilename",         X_TEXTUREFILENAME},
    {"AnimationSet",            X_ANIMATIONSET},
    {"AnimationKey",            X_ANIMATIONKEY},
    {"Animation",               X_ANIMATION},
    {"Header",                  X_HEADER}
};

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::concatenateMeshes(MeshPtr pMesh)
{
    Mesh *pConcatMesh = NULL;

    if (m_meshes.size() >= 2) {
        pConcatMesh     = new Mesh;
        Mesh* pLastMesh = m_meshes.back();

        /**
         * We create the new mesh. We get the dimensions of the new mesh
         */
        pConcatMesh->nVertices      = pLastMesh->nFirstVertex + pLastMesh->nVertices;
        pConcatMesh->nFaces         = pLastMesh->nFirstFace + pLastMesh->nFaces;
        pConcatMesh->nTexCoords     = pLastMesh->nFirstTextureCoord + pLastMesh->nTexCoords;
        pConcatMesh->nNormals       = pLastMesh->nFirstNormal + pLastMesh->nNormals;

        /**
         * Here we have a control:
        * - Texture coordinates must be as numerous as Vertices or there must be none
         * - Normal vectors must be as numerous as Vertices or there must be none
        */
        if ((pConcatMesh->nTexCoords < pConcatMesh->nVertices) && (pConcatMesh->nTexCoords != 0)) {
            pConcatMesh->nTexCoords = pConcatMesh->nVertices;
        }

        if ((pConcatMesh->nNormals < pConcatMesh->nVertices) && (pConcatMesh->nNormals != 0)) {
            pConcatMesh->nNormals = pConcatMesh->nVertices;
        }

        /**
         * We create all the arrays:
         * - Vertices and Faces
         * - Material indices per face
         * - Texture Coords
         * - Normal vectors and Face Allocation of Normal vectors
         */
        pConcatMesh->pVertices = new Vertex[pConcatMesh->nVertices];
        memset(pConcatMesh->pVertices, 0, pConcatMesh->nVertices * sizeof(Vertex));

        pConcatMesh->pFaces = new Face[pConcatMesh->nFaces];
        memset(pConcatMesh->pFaces, 0, pConcatMesh->nFaces * sizeof(Face));

        pConcatMesh->pFaceMaterials = new unsigned short int[pConcatMesh->nFaces];
        memset(pConcatMesh->pFaceMaterials, 0, pConcatMesh->nFaces * sizeof(unsigned short int));

        if (pConcatMesh->nTexCoords != 0) {
            pConcatMesh->pTexCoords = new TexCoords[pConcatMesh->nTexCoords];
            memset(pConcatMesh->pTexCoords, 0, pConcatMesh->nTexCoords * sizeof(TexCoords));
        }

        if (pConcatMesh->nNormals != 0) {
            pConcatMesh->pNormals       = new CPepeEngineVector3[pConcatMesh->nNormals];
            pConcatMesh->pFaceNormals   = new FaceNormals[pConcatMesh->nFaces];
            memset(pConcatMesh->pNormals, 0, pConcatMesh->nNormals * sizeof(CPepeEngineVector3));
            memset(pConcatMesh->pFaceNormals, 0, pConcatMesh->nFaces * sizeof(Face));
        }

        /** We fill up the arrays with each array from the m_meshes container */
        for (std::vector<Mesh*>::iterator i = m_meshes.begin(); i != m_meshes.end(); i++) {
            // ------------ update indices ----------------------------------
            for (unsigned long int j = 0; j < (*i)->nFaces; j++) {
                (*i)->pFaces[j].a += (*i)->nFirstVertex;
                (*i)->pFaces[j].b += (*i)->nFirstVertex;
                (*i)->pFaces[j].c += (*i)->nFirstVertex;

                (*i)->pFaceMaterials[j] += (*i)->nFirstMaterial;
            }

            if ((*i)->nNormals != 0) {
                for (unsigned long int j = 0; j < (*i)->nFaces; j++) {
                    (*i)->pFaceNormals[j].a += (*i)->nFirstNormal;
                    (*i)->pFaceNormals[j].b += (*i)->nFirstNormal;
                    (*i)->pFaceNormals[j].c += (*i)->nFirstNormal;
                }
            }

            //end ------------ update indices -------------------------------

            memcpy(pConcatMesh->pVertices + (*i)->nFirstVertex, (*i)->pVertices, (*i)->nVertices * sizeof(Vertex));
            memcpy(pConcatMesh->pFaces + (*i)->nFirstFace, (*i)->pFaces, (*i)->nFaces * sizeof(Face));
            memcpy(pConcatMesh->pFaceMaterials + (*i)->nFirstFace, (*i)->pFaceMaterials, (*i)->nFaces * sizeof(unsigned short int));

            if ((*i)->nTexCoords != 0) {
                memcpy(&(pConcatMesh->pTexCoords[(*i)->nFirstTextureCoord]), (*i)->pTexCoords, (*i)->nTexCoords * sizeof(TexCoords));
            }

            if ((*i)->nNormals != 0) {
                memcpy(&(pConcatMesh->pNormals[(*i)->nFirstNormal]), (*i)->pNormals, (*i)->nNormals * sizeof(CPepeEngineVector3));
                memcpy(&(pConcatMesh->pFaceNormals[(*i)->nFirstFace]), (*i)->pFaceNormals, (*i)->nFaces * sizeof(Face));
            }
        }

        m_meshes.push_back(pConcatMesh);
    } else {
        pConcatMesh = m_meshes[0];
    }

    // ---------------------- move data to pepe engine mesh ---------------
    CPepeEngineVertexData* pVertexData      = new CPepeEngineVertexData();
    pVertexData->m_nVertexCount             = pConcatMesh->nVertices;
    pVertexData->m_nVertexStart             = 0;
    pVertexData->m_pVertexDeclaration       = IPepeEngineBufferManager::getSingleton().createVertexDeclaration();

    size_t          offset  = 0;
    unsigned int    vSize   = 0;

    if (pConcatMesh->nVertices > 0) {
        pVertexData->m_pVertexDeclaration->addElement(0, offset, VET_FLOAT3, VES_POSITION);
        offset  += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
        vSize   += 3;
    }

    if (pConcatMesh->nNormals > 0) {
        pVertexData->m_pVertexDeclaration->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
        offset  += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
        vSize   += 3;
    }

    if (pConcatMesh->nTexCoords > 0) {
        pVertexData->m_pVertexDeclaration->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        vSize += 2;
    }

    float* vertexData   = new float[pConcatMesh->nVertices * vSize];

    for (unsigned int i = 0; i < pConcatMesh->nFaces; i++) {
        offset = 0;

        if (pConcatMesh->nVertices > 0) {
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset]       = pConcatMesh->pVertices[pConcatMesh->pFaces[i].a].x;
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 1]   = pConcatMesh->pVertices[pConcatMesh->pFaces[i].a].y;
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 2]   = pConcatMesh->pVertices[pConcatMesh->pFaces[i].a].z;

            vertexData[pConcatMesh->pFaces[i].b * vSize + offset]       = pConcatMesh->pVertices[pConcatMesh->pFaces[i].b].x;
            vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 1]   = pConcatMesh->pVertices[pConcatMesh->pFaces[i].b].y;
            vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 2]   = pConcatMesh->pVertices[pConcatMesh->pFaces[i].b].z;

            vertexData[pConcatMesh->pFaces[i].c * vSize + offset]       = pConcatMesh->pVertices[pConcatMesh->pFaces[i].c].x;
            vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 1]   = pConcatMesh->pVertices[pConcatMesh->pFaces[i].c].y;
            vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 2]   = pConcatMesh->pVertices[pConcatMesh->pFaces[i].c].z;
            offset += 3;
        }

        if (pConcatMesh->nNormals > 0) {
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset]       = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].a].x;
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 1]   = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].a].y;
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 2]   = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].a].z;

            vertexData[pConcatMesh->pFaces[i].b * vSize + offset]       = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].b].x;
            vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 1]   = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].b].y;
            vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 2]   = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].b].z;

            vertexData[pConcatMesh->pFaces[i].c * vSize + offset]       = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].c].x;
            vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 1]   = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].c].y;
            vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 2]   = pConcatMesh->pNormals[pConcatMesh->pFaceNormals[i].c].z;
            offset += 3;
        }

        if (pConcatMesh->nTexCoords > 0) {
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset]       = pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].a].u;
            vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 1]   = pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].a].v;

            vertexData[pConcatMesh->pFaces[i].b * vSize + offset]       = pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].b].u;
            vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 1]   = pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].b].v;

            vertexData[pConcatMesh->pFaces[i].c * vSize + offset]       = pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].c].u;
            vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 1]   = pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].c].v;
        }
    }

    pVertexData->m_pVertexBuffer = IPepeEngineBufferManager::getSingleton().createVertexBuffer(
                                       pConcatMesh->nVertices, vSize * sizeof(float), IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, pConcatMesh->nVertices * vSize * sizeof(float), false
                                   );
    pVertexData->m_pVertexBuffer->writeData(0, vSize * pConcatMesh->nVertices * sizeof(float), vertexData, false);

    pMesh->m_nFaces     = pConcatMesh->nFaces;
    pMesh->m_nNormals   = pConcatMesh->nNormals;
    pMesh->m_nTexCoords = pConcatMesh->nTexCoords;
    pMesh->m_nVertices  = pConcatMesh->nVertices;
    pMesh->m_nMaterials = pConcatMesh->nMaterials;

    pMesh->setVertexData(pVertexData);
    delete vertexData;
    //end ---------------------- move data to pepe engine mesh ------------

    // ----------------------- create sub meshes --------------------------
    unsigned int        nFaceCount  = 0;
    CPepeEngineSubMesh* pSubMesh    = NULL;

    //For each material
    for (unsigned int i = 0; i < m_materials.size(); i++) {
        //We count the number of faces using this material
        nFaceCount = 0;

        for (unsigned int j = 0; j < pConcatMesh->nFaces; j++) {
            if (pConcatMesh->pFaceMaterials[j] == i) {
                ++nFaceCount;
            }
        }

        //We initialise the mesh subset
        pSubMesh = pMesh->createSubMesh();

        pSubMesh->m_nFaces = nFaceCount;

        // ------------- material setting ------------------------------------
        pSubMesh->setMaterialName(m_materials[i]->strMaterialName);

        MaterialPtr pMaterial = CPepeEngineMaterialManager::getSingleton().create(
                                    pSubMesh->getMaterialName()
                                );

        pMaterial->setDiffuse(CPepeEngineColor(
                                  m_materials[i]->diffuse[0],
                                  m_materials[i]->diffuse[1],
                                  m_materials[i]->diffuse[2],
                                  m_materials[i]->diffuse[3]
                              ));

        pMaterial->setSpecular(CPepeEngineColor(
                                   m_materials[i]->specular[0],
                                   m_materials[i]->specular[1],
                                   m_materials[i]->specular[2]
                               ));

        pMaterial->setEmissive(CPepeEngineColor(
                                   m_materials[i]->emissive[0],
                                   m_materials[i]->emissive[1],
                                   m_materials[i]->emissive[2]
                               ));

        pMaterial->setAmbient(CPepeEngineColor(
                                  m_materials[i]->ambient[0],
                                  m_materials[i]->ambient[1],
                                  m_materials[i]->ambient[2]
                              ));

        pMaterial->setPower(m_materials[i]->fPower);

        //end ------------- material setting ---------------------------------

        // ------------------------ texture setting ----------------------------
        if (!m_materials[i]->strTextureName.empty()) {
            pMaterial->setTexture(m_materials[i]->strTextureName);
        }

        //end ------------------------ texture setting -------------------------

        int k = 0;
        //We fill in the Mesh subset
        Face *pFaces = new Face[nFaceCount];

        for (unsigned int j = 0; j < pConcatMesh->nFaces; j++) {
            if (pConcatMesh->pFaceMaterials[j] == i) {
                pFaces[k].a = pConcatMesh->pFaces[j].a;
                pFaces[k].b = pConcatMesh->pFaces[j].b;
                pFaces[k].c = pConcatMesh->pFaces[j].c;
                k++;
            }
        }

        CPepeEngineIndexData *pIndexData = new CPepeEngineIndexData();

        pIndexData->m_nIndexStart   = 0;
        pIndexData->m_nIndexCount   = nFaceCount * 3;
        pIndexData->m_pIndexBuffer  = IPepeEngineBufferManager::getSingleton().createIndexBuffer(
                                          sizeof(Face) * nFaceCount, IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, IPepeEngineIndexBuffer::IT_32BIT,
                                          false
                                      );
        pIndexData->m_pIndexBuffer->writeData(0, sizeof(Face) * nFaceCount, pFaces, false);

        pSubMesh->setIndexData(pIndexData);
        delete pFaces;
    }

    //end -------------------- create sub meshes --------------------------
}

// -----------------------------------------------------------------------------------------
short int CPepeEngineXFileMeshLoader::processBlock()
{
    std::string strText;
    char        cToken = fin.peek();

    switch (cToken) {
    case '\n':
    case ' ':
    case '\t':
        fin.get();
        return X_COMMENT; //spaces are identified as comments

    case '{':
        return X_OBRACE;

    case '}':
        fin.get();
        return X_EBRACE; //We arrived at the end of the block

    case '/':
        fin.get();

        if (fin.peek() != '/') {
            return X_ERROR;
        } //we processed a block name starting with slash ?!

    case '#':
        fin.ignore(TEXT_BUFFER, '\n');
        return X_COMMENT;

    default:
        fin >> strText;
        return blockID(strText);
    }
}

// -----------------------------------------------------------------------------------------
short int CPepeEngineXFileMeshLoader::blockID(const std::string& strText)
{
    long lPos;

    if (fin.eof()) {
        return X_COMMENT;
    }

    if (strText.empty()) {
        return X_ERROR;
    }

    for (int i = 0; i < MAX_TEMPLATES; i++) {
        lPos = strText.find(Templates[i].strTxtID);

        if (lPos > -1) {
            fin.get(); //eats the whitespace after the block name.
            return Templates[i].unTemplateID;
        }
    }

    return X_UNKNOWN;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::avoidTemplate()
{
    char cToken;

    fin.ignore(TEXT_BUFFER, '{');

    while (!fin.eof()) {
        cToken = (char)fin.peek();

        if (cToken == '{') {
            avoidTemplate();
        }

        if  (cToken == '}') {
            fin.get();
            return;
        }

        fin.get();
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processBone()
{
    short int   nToken;
    char        pData[TEXT_BUFFER];

    fin.ignore(TEXT_BUFFER, '{');

    nToken = X_OBRACE;

    while (nToken != X_EBRACE) {
        nToken = processBlock();

        switch (nToken) {
        case X_COMMENT:
            break; //used for spaces and other kind of comments

        case X_EBRACE:
            return;

        case X_FRAMETRANSFORMMATRIX:
            processFrameTransformMatrix();
            return;

        case X_OBRACE:
            fin.getline(pData, TEXT_BUFFER, '}');
            break; //a mesh name has been found here

        case X_FRAME:
            processBone();
            break;

        case X_MESH:
            processMesh();
            break;

        case X_UNKNOWN:
        default:
            avoidTemplate();
            break;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processMesh()
{
    std::string     strText;
    short int       nToken;
    char            pData[TEXT_BUFFER];

    // ---------- loaded mesh initialization ----------
    m_pLoadedMesh = new Mesh;
    m_pLoadedMesh->nFaces               = 0;
    m_pLoadedMesh->nNormals             = 0;
    m_pLoadedMesh->nTexCoords           = 0;
    m_pLoadedMesh->nVertices            = 0;
    m_pLoadedMesh->nMaterials           = 0;
    m_pLoadedMesh->nFirstVertex         = 0;
    m_pLoadedMesh->nFirstFace           = 0;
    m_pLoadedMesh->nFirstTextureCoord   = 0;
    m_pLoadedMesh->nFirstNormal         = 0;
    m_pLoadedMesh->nFirstMaterial       = 0;
    m_pLoadedMesh->pFaceNormals         = NULL;
    m_pLoadedMesh->pFaces               = NULL;
    m_pLoadedMesh->pNormals             = NULL;
    m_pLoadedMesh->pTexCoords           = NULL;
    m_pLoadedMesh->pVertices            = NULL;
    m_pLoadedMesh->pFaceMaterials       = NULL;

    if (!m_meshes.empty()) {
        Mesh *pLastMesh = m_meshes.back();

        m_pLoadedMesh->nFirstVertex         = pLastMesh->nFirstVertex + pLastMesh->nVertices;
        m_pLoadedMesh->nFirstFace           = pLastMesh->nFirstFace + pLastMesh->nFaces;
        m_pLoadedMesh->nFirstMaterial       = pLastMesh->nFirstMaterial + pLastMesh->nMaterials;
        m_pLoadedMesh->nFirstNormal         = pLastMesh->nFirstNormal + pLastMesh->nNormals;
        m_pLoadedMesh->nFirstTextureCoord   = pLastMesh->nFirstTextureCoord + pLastMesh->nTexCoords;

        if (m_pLoadedMesh->nFirstTextureCoord < m_pLoadedMesh->nFirstVertex) {
            m_pLoadedMesh->nFirstTextureCoord = m_pLoadedMesh->nFirstVertex;
        }

        if (m_pLoadedMesh->nFirstNormal < m_pLoadedMesh->nFirstVertex) {
            m_pLoadedMesh->nFirstNormal = m_pLoadedMesh->nFirstVertex;
        }
    }

    //end ---------- loaded mesh initialization -------

    nToken = fin.peek();

    if (nToken != '{') {
        std::string tmp;
        fin >> tmp;
        m_pLoadedMesh->strName = _TSTR(tmp);
    }

    fin.ignore(TEXT_BUFFER, '{');
    fin.getline(pData, TEXT_BUFFER, ';');

    m_pLoadedMesh->nVertices    = (unsigned int)CPepeEngineConverter::textToNum(pData);
    m_pLoadedMesh->pVertices    = new Vertex[m_pLoadedMesh->nVertices];

    CPepeEngineVector3 v;

    for (unsigned int i = 0; i < m_pLoadedMesh->nVertices; i++) {
        /*fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pVertices[i].x = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pVertices[i].y = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pVertices[i].z = CPepeEngineConverter::textToNum(pData);      */

        fin.getline(pData, TEXT_BUFFER, ';');
        v.x = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        v.y = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        v.z = CPepeEngineConverter::textToNum(pData);

        v = m_frameMeshTransform * v;

        m_pLoadedMesh->pVertices[i].x = v.x;
        m_pLoadedMesh->pVertices[i].y = v.y;
        m_pLoadedMesh->pVertices[i].z = v.z;

        fin.get(); //eats either the comma or the semicolon at the end of each vertex description
    }

    fin.getline(pData, TEXT_BUFFER, ';');
    m_pLoadedMesh->nFaces   = (unsigned int)CPepeEngineConverter::textToNum(pData);
    m_pLoadedMesh->pFaces   = new Face[m_pLoadedMesh->nFaces];

    for (unsigned int i = 0; i < m_pLoadedMesh->nFaces; i++) {
        fin.getline(pData, TEXT_BUFFER, ';');
        int nVNum = (int)CPepeEngineConverter::textToNum(pData);

        if (nVNum != 3) {
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_FILE_LOAD_ERROR,
                _T("Only triangles supported."),
                _T("CPepeEngineXFileMeshLoader::processMesh")
            );
        }

        fin.getline(pData, TEXT_BUFFER, ',');
        m_pLoadedMesh->pFaces[i].a = (unsigned int)CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ',');
        m_pLoadedMesh->pFaces[i].b = (unsigned int)CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pFaces[i].c = (unsigned int)CPepeEngineConverter::textToNum(pData);
        fin.get(); //eats either the comma or the semicolon at the end of each face description
    }

    nToken = X_COMMENT;

    while (nToken != X_EBRACE) {
        nToken = processBlock();

        switch (nToken) {
        case X_COMMENT:
            break; //used for spaces and other kind of comments

        case X_EBRACE:
            m_meshes.push_back(m_pLoadedMesh);
            return;

        case X_MESHNORMALS:
            processMeshNormals();
            break;

        case X_MESHTEXTURECOORDS:
            processMeshTextureCoords();
            break;

        case X_MESHMATERIALLIST:
            processMeshMaterials();
            break;

            /*
                        case X_SKINMESHHEADER:
                            avoidTemplate();
                            break;

                        case X_SKINWEIGHTS:
                            //ProcessSkinWeights();
                            //break;
            */
        default:
            avoidTemplate();
            break;
        }
    }

    m_meshes.push_back(m_pLoadedMesh);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processMeshNormals()
{
    char pData[TEXT_BUFFER];

    fin.ignore(TEXT_BUFFER, '{');
    fin.getline(pData, TEXT_BUFFER, ';');

    m_pLoadedMesh->nNormals = (unsigned int)CPepeEngineConverter::textToNum(pData);
    m_pLoadedMesh->pNormals = new CPepeEngineVector3[m_pLoadedMesh->nNormals];

    for (unsigned int i = 0; i < m_pLoadedMesh->nNormals; i++) {
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pNormals[i].x = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pNormals[i].y = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pNormals[i].z = CPepeEngineConverter::textToNum(pData);
        fin.get(); //eats either the comma or the semicolon at the end of each face description
    }

    m_pLoadedMesh->pFaceNormals = new FaceNormals[m_pLoadedMesh->nFaces];

    for (unsigned int i = 0; i < m_pLoadedMesh->nFaces; i++) {
        fin.ignore(TEXT_BUFFER, ';');
        fin.getline(pData, TEXT_BUFFER, ',');
        m_pLoadedMesh->pFaceNormals[i].a = (unsigned int)CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ',');
        m_pLoadedMesh->pFaceNormals[i].b = (unsigned int)CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pFaceNormals[i].c = (unsigned int)CPepeEngineConverter::textToNum(pData);
        fin.get(); //eats either the comma or the semicolon at the end of each face description
    }

    fin.ignore(TEXT_BUFFER, '}');
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processMeshTextureCoords()
{
    char pData[TEXT_BUFFER];

    fin.ignore(TEXT_BUFFER, '{');
    fin.getline(pData, TEXT_BUFFER, ';');

    m_pLoadedMesh->nTexCoords   = (unsigned int)CPepeEngineConverter::textToNum(pData);
    m_pLoadedMesh->pTexCoords   = new TexCoords[m_pLoadedMesh->nTexCoords];

    for (unsigned int i = 0; i < m_pLoadedMesh->nTexCoords; i++) {
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pTexCoords[i].u = CPepeEngineConverter::textToNum(pData);
        fin.getline(pData, TEXT_BUFFER, ';');
        m_pLoadedMesh->pTexCoords[i].v = CPepeEngineConverter::textToNum(pData);
        fin.get(); //eats either the comma or the semicolon at the end of each face description
    }

    fin.ignore(TEXT_BUFFER, '}');
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processMeshMaterials()
{
    std::string     strText;
    short int       nToken;
    char            pData[TEXT_BUFFER];

    fin.ignore(TEXT_BUFFER, '{');
    fin.getline(pData, TEXT_BUFFER, ';');

    m_pLoadedMesh->nMaterials = (unsigned int)CPepeEngineConverter::textToNum(pData);

    fin.getline(pData, TEXT_BUFFER, ';');

    unsigned int nFaceMaterials     = (unsigned int)CPepeEngineConverter::textToNum(pData);
    m_pLoadedMesh->pFaceMaterials   = new unsigned short int[nFaceMaterials];

    for (unsigned short i = 0; i < m_pLoadedMesh->nFaces - 1; i++) {
        fin.getline(pData, TEXT_BUFFER, ',');
        m_pLoadedMesh->pFaceMaterials[i] = (unsigned short int)CPepeEngineConverter::textToNum(pData);
    }

    fin.getline(pData, TEXT_BUFFER, ';');
    m_pLoadedMesh->pFaceMaterials[m_pLoadedMesh->nFaces - 1] = (unsigned short int)CPepeEngineConverter::textToNum(pData);
    fin.get(); //eats the last semicolon

    nToken = X_COMMENT;

    while (nToken != X_EBRACE) {
        nToken = processBlock();

        switch (nToken) {
        case X_COMMENT:
            break; //used for spaces and other kind of comments

        case X_EBRACE:
            return;

        case X_MATERIAL:
            processMaterial();
            break;

        default:
            avoidTemplate();
            break;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processMaterial()
{
    std::string     strText;
    short int       nToken;
    char            pData[TEXT_BUFFER];

    Material* newMaterial = new Material;

    nToken = fin.peek();

    if (nToken != '{') {
        std::string tmp;
        fin >> tmp;
        newMaterial->strMaterialName = _TSTR(tmp);
    } else {
        newMaterial->strMaterialName = CPepeEngineMaterialManager::getSingleton().generateGUID();
    }

    fin.ignore(TEXT_BUFFER, '{');

    for (int i = 0; i < 4; i++) {
        fin.getline(pData, TEXT_BUFFER, ';');
        newMaterial->diffuse[i] = CPepeEngineConverter::textToNum(pData);
    }

    fin.get(); //eats the last semicolon
    fin.getline(pData, TEXT_BUFFER, ';');
    newMaterial->fPower = CPepeEngineConverter::textToNum(pData);

    for (int i = 0; i < 3; i++) {
        fin.getline(pData, TEXT_BUFFER, ';');
        newMaterial->specular[i] = CPepeEngineConverter::textToNum(pData);
    }

    fin.get(); //eats the last semicolon

    for (int i = 0; i < 3; i++) {
        fin.getline(pData, TEXT_BUFFER, ';');
        newMaterial->emissive[i] = CPepeEngineConverter::textToNum(pData);
    }

    fin.get(); //eats the last semicolon

    nToken = X_COMMENT;

    while (nToken != '}') {
        nToken = processBlock();

        switch (nToken) {
        case X_COMMENT:
            break; //used for spaces and other kind of comments

        case X_EBRACE:
            m_materials.push_back(newMaterial);
            return;

        case X_TEXTUREFILENAME:
            fin.ignore(TEXT_BUFFER, '{');
            fin.ignore(TEXT_BUFFER, '"');
            fin.getline(pData, TEXT_BUFFER, '"');
            newMaterial->strTextureName = _TSTR(pData);
            fin.ignore(TEXT_BUFFER, '}');
            break;

        default:
            avoidTemplate();
            break;
        }
    }

    m_materials.push_back(newMaterial);
}

// -----------------------------------------------------------------------------------------
CPepeEngineXFileMeshLoader::CPepeEngineXFileMeshLoader()
{
    fin.clear();
    m_meshes.clear();
    m_materials.clear();

    m_pLoadedMesh           = NULL;
    m_frameMeshTransform    = CPepeEngineMatrix4::IDENTITY;
}

// -----------------------------------------------------------------------------------------
CPepeEngineXFileMeshLoader::~CPepeEngineXFileMeshLoader()
{
    reset();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::load(const tstring& strFileName, MeshPtr pMesh)
{
    fin.clear();
    fin.open(strFileName.c_str(), std::ios::in);

    m_pLoadedMesh = NULL;

    if (pMesh == NULL) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("Mesh: null pointer"),
            _T("CPepeEngineXFileMeshLoader::load")
        );
    }

    if (fin.bad()) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_FILE_LOAD_ERROR,
            _T("File load error"),
            _T("CPepeEngineXFileMeshLoader::load")
        );
    }

    XFileHeader xHeader;

    fin.read((char* )&xHeader, 16);

    if (xHeader.Magic != XOFFILE_FORMAT_MAGIC) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_FILE_LOAD_ERROR,
            _T("File load error: not a .X model file."),
            _T("CPepeEngineXFileMeshLoader::load")
        );
    }

    if (xHeader.Major_Version != XOFFILE_FORMAT_VERSION03) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_FILE_LOAD_ERROR,
            _T("File load error: major version greater than 03."),
            _T("CPepeEngineXFileMeshLoader::load")
        );
    }

    if ((xHeader.Minor_Version != XOFFILE_FORMAT_VERSION03) || (xHeader.Minor_Version != XOFFILE_FORMAT_VERSION02)) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_FILE_LOAD_ERROR,
            _T("File load error: minor version greater than 03."),
            _T("CPepeEngineXFileMeshLoader::load")
        );
    }

    if (xHeader.Format != XOFFILE_FORMAT_TEXT) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_FILE_LOAD_ERROR,
            _T("File load error: not a text format."),
            _T("CPepeEngineXFileMeshLoader::load")
        );
    }

    while (!fin.eof()) {
        switch (processBlock()) {
        case X_ERROR:
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_FILE_LOAD_ERROR,
                _T("File load error: stopped processing the file."),
                _T("CPepeEngineXFileMeshLoader::load")
            );

        case X_COMMENT:
            break; // nothing to do

        case X_EBRACE:
            break; // end of the block

        case X_MESH:
            processMesh();
            break;

        case X_FRAME:
            processBone();
            break;

        case X_ANIMATIONSET:
        case X_OBRACE:

        default:
            avoidTemplate();
            break;
        }
    }

    concatenateMeshes(pMesh);

    reset();
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineXFileMeshLoader::isAvailableFileExtension(const tstring& strFileName) const
{
    if (strFileName.rfind(_T(".x")) != -1) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::reset()
{
    for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++) {
        if (*it != NULL) {
            delete[] (*it)->pFaces;
            delete[] (*it)->pFaceNormals;
            delete[] (*it)->pVertices;
            delete[] (*it)->pNormals;
            delete[] (*it)->pTexCoords;
            delete[] (*it)->pFaceMaterials;

            delete *it;
        }
    }

    for (std::vector<Material*>::iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); itMaterial++) {
        delete *itMaterial;
    }

    m_meshes.clear();
    m_materials.clear();

    fin.close();

    m_frameMeshTransform = CPepeEngineMatrix4::IDENTITY;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineXFileMeshLoader::processFrameTransformMatrix()
{
    char pData[TEXT_BUFFER];

    fin.ignore(TEXT_BUFFER, '{');

    CPepeEngineMatrix4 m;
    int k = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (k < 15) {
                fin.getline(pData, TEXT_BUFFER, ',');
            } else {
                fin.getline(pData, TEXT_BUFFER, ';');
            }

            m.m[j][i] = CPepeEngineConverter::textToNum(pData);
            k++;
        }
    }

    m_frameMeshTransform = m;

    fin.ignore(TEXT_BUFFER, '}');
}

_PEPE_ENGINE_END