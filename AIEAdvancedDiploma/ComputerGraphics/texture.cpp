////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "texture.h"
#include "renderdata.h"
#include "soil/SOIL.h"
#include "tweaker.h"

Texture::Texture(const std::string& name, 
                 const std::string& path, 
                 Type type,
                 Filter filter) :

    m_name(name),
    m_path(path),
    m_type(type),
    m_filter(filter)
{
}

Texture::~Texture()
{
    if(m_initialised)
    {
        glDeleteTextures(1, &m_id);
        m_initialised = false;
    }
}

void Texture::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddStrEntry("Name", m_name);
}

const std::string& Texture::Name() const
{
    return m_name;
}

const std::string& Texture::Path() const
{
    return m_path;
}

bool Texture::IsCubeMap() const
{
    return m_type == CUBE;
}

bool Texture::IsProcedural() const
{
    return m_type == PROCEDURAL;
}

bool Texture::Initialise()
{
    glGenTextures(1, &m_id);
    m_initialised = true;

    auto type = IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    glBindTexture(type, m_id);

    bool success = false;
    if (m_type == CUBE)
    {
        success = InitialiseCubeMap();
    }
    else if (m_type == FROM_FILE)
    {
        success = InitialiseFromFile();
    }
    else
    {
        success = InitialiseFromPixels();
    }

    return success && CreateMipMaps() && !HasCallFailed();
}

bool Texture::InitialiseCubeMap()
{
    const std::string path(m_path + "_c0");
    return LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, path + "0.png") &&
           LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, path + "1.png") &&
           LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, path + "2.png") &&
           LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, path + "3.png") &&
           LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, path + "4.png") &&
           LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, path + "5.png");
}

bool Texture::InitialiseFromFile()
{
    return LoadTexture(GL_TEXTURE_2D, m_path);
}

bool Texture::InitialiseFromPixels()
{
    throw std::exception("Texture::InitialiseFromPixels not implemented");
}

void Texture::Reload()
{
    throw std::exception("Texture::Reload not implemented");
}

void Texture::Save()
{
    throw std::exception("Texture::Save not implemented");
}

const std::vector<unsigned int>& Texture::GetPixels() const
{
    throw std::exception("Texture::GetPixels not implemented");
}

bool Texture::SetFiltering()
{
    const auto type = IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

    const auto magFilter = m_filter == Texture::NEAREST ? GL_NEAREST : GL_LINEAR;
    const auto minFilter = m_filter == Texture::NEAREST ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR;

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);

    if (m_filter == Texture::ANISOTROPIC)
    {
        const float MAX_ANISOTROPY = 16.0f;
        const auto GL_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FE;
        glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, MAX_ANISOTROPY);
    }

    if(HasCallFailed())
    {
        LogError("Failed to set filtering for " + m_name);
        return false;
    }
    return true;
}

bool Texture::CreateMipMaps()
{
    if (m_filter != Texture::NEAREST)
    {
        glGenerateMipmap(IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);

        if(HasCallFailed())
        {
            LogError("Mipmap creation failed for " + m_name);
            return false;
        }
    }
    return true;
}

bool Texture::LoadTexture(GLenum type, const std::string& path)
{
    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if(HasCallFailed())
    {
        LogError("Failed to load " + path + " texture");
        return false;
    }

    return SetFiltering();
}

GLuint Texture::GetID() const
{
    return m_id;
}