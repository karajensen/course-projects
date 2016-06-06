////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opencv.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opencv.h"
#include <Windows.h>
#include "directxcommon.h"
#include "directxtarget.h"
#include "directxengine.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/directx.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

OpenCV::OpenCV(DirectxEngine& directx, int width, int height) :
    m_directx(directx),
    m_texture(nullptr),
    m_diagnosticPosition(10, 20),
    m_width(width),
    m_height(height)
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
    cv::directx::ocl::initializeContextFromD3D11Device(m_directx.GetDevice());

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

    if (FAILED(m_directx.GetDevice()->CreateTexture2D(&desc, 0, &m_texture)))
    {
        MessageBox(0, "Failed to create OpenCV texture", "ERROR", MB_OK);
        return false;
    }

    return true;
}

void OpenCV::DiagnosticLine(cv::UMat& mat, const std::string& text)
{
    cv::Point pt(m_diagnosticPosition);
    pt.y += m_diagnosticLine * 10;
    cv::putText(mat, text, pt, 1, 0.8, cv::Scalar(255, 255, 255), 1);
    ++m_diagnosticLine;
}

void OpenCV::Update(int vectorization)
{
    m_diagnosticLine = 0;

    auto* context = m_directx.GetContext();

    cv::Mat m_frame_bgr;
    bool success = m_video.read(m_frame_bgr);
    if (!success)
    {
        MessageBox(0, "Cannot read a frame from video stream", "ERROR", MB_OK);
        return;
    }

    cv::cvtColor(m_frame_bgr, m_frame_rgba, CV_BGR2RGBA);

    UINT subResource = ::D3D11CalcSubresource(0, 0, 0);
    D3D11_MAPPED_SUBRESOURCE mappedTex;
    if (FAILED(context->Map(m_texture, subResource, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex)))
    {
        MessageBox(0, "Surface mapping failed", "ERROR", MB_OK);
        return;
    }

    cv::Mat mat(m_height, m_width, CV_8UC4, mappedTex.pData, mappedTex.RowPitch);
    m_frame_rgba.copyTo(mat);

    context->Unmap(m_texture, subResource);

    cv::UMat uMat;
    cv::directx::convertFromD3D11Texture2D(m_texture, uMat);
    
    #ifdef _DEBUG
    DiagnosticLine(uMat, "Width: " + std::to_string(m_width));
    DiagnosticLine(uMat, "Height: " + std::to_string(m_height));
    DiagnosticLine(uMat, "Vectorization: " + std::to_string(vectorization));
    #endif
    
    cv::directx::convertToD3D11Texture2D(uMat, m_texture);

    m_directx.GetContext()->CopyResource(m_directx.GetBackBuffer().GetTexture(), m_texture);
}