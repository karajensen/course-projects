////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opencv.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opencv.h"
#include <Windows.h>
#include "tweaker.h"
#include "directxcommon.h"
#include "directxengine.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/directx.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

OpenCV::OpenCV(ID3D11Device* device,
               ID3D11DeviceContext* context,
               const POINT& size)
    : m_device(device)
    , m_context(context)
    , m_texture(nullptr)
    , m_width(size.x)
    , m_height(size.y)
{
}

OpenCV::~OpenCV() = default;

void OpenCV::Close()
{
    m_frame_bgr.release();
    m_frame_rgba.release();

    if (m_video.isOpened())
    {
        m_video.release();
    }

    SafeRelease(&m_texture);
}

bool OpenCV::Initialize()
{   
    cv::directx::ocl::initializeContextFromD3D11Device(m_device);

    if (!m_video.open(0))
    {
        MessageBox(0, "Failed to open web cam", "ERROR", MB_OK);
        return false;
    }

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;

    if (FAILED(m_device->CreateTexture2D(&desc, 0, &m_texture)))
    {
        MessageBox(0, "Failed to create OpenCV texture", "ERROR", MB_OK);
        return false;
    }

    return true;
}

ID3D11Texture2D* OpenCV::GetFrame()
{
    return m_texture;
}

bool OpenCV::Update()
{
    if (!m_video.read(m_frame_bgr))
    {
        return false;
    }

    cv::cvtColor(m_frame_bgr, m_frame_rgba, CV_BGR2RGBA);

    UINT subResource = ::D3D11CalcSubresource(0, 0, 0);
    D3D11_MAPPED_SUBRESOURCE mappedTex;
    if (FAILED(m_context->Map(m_texture, subResource, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex)))
    {
        MessageBox(0, "Surface mapping failed", "ERROR", MB_OK);
        return false;
    }

    cv::Mat mat(m_height, m_width, CV_8UC4, mappedTex.pData, mappedTex.RowPitch);
    m_frame_rgba.copyTo(mat);

    m_context->Unmap(m_texture, subResource);

    return true;
}   

void OpenCV::AddToTweaker(Tweaker& tweaker)
{
    tweaker.SetGroup("OpenCV");

    tweaker.AddDblEntry("Camera Width",
        [this]() { return m_video.get(cv::CAP_PROP_FRAME_WIDTH); });

    tweaker.AddDblEntry("Camera Height",
        [this]() { return m_video.get(cv::CAP_PROP_FRAME_HEIGHT); });
}