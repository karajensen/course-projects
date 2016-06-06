////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning (disable : 4005) // For DX10
#include <string>
#include "directx/include/d3d11.h"
#include "directx/include/d3dx11.h"
#include "directx/include/d3dx10.h"

const float CAMERA_FAR = 1000.0f;
const float CAMERA_NEAR = 1.0f;
const int MAX_ANISOTROPY = 16;
const float FIELD_OF_VIEW = 60.0f;

/**
* Sets the name of the directx object for debugging
* @param object The object to set
* @param name The name of the object
*/
template <typename T> void SetDebugName(T* object, const std::string& name)
{
    #ifdef _DEBUG
    if(object)
    {
        object->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
    }
    #endif
}

/**
* Releases the directx object safely
* @param pObject The pointer to the object to release
*/
template <typename T> void SafeRelease(T** pObject)
{
    if(*pObject)
    {
        (*pObject)->Release();
        *pObject = nullptr;
    }
}