////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "texture.h"
#include "renderdata.h"
#include "soil/SOIL.h"
#include "colour.h"

Texture::Texture(const std::string& name, const std::string& path, Type type) :
    m_name(name),
    m_path(path),
    m_image(type)
{
}

Texture::Texture(const std::string& name, const std::string& path, int size, Type type) :
    m_name(name),
    m_path(path),
    m_image(type)
{
    m_size = size;
    m_pixels.resize(size * size);

    switch (type)
    {
    case RANDOM:
        MakeRandomNormals();
        break;
    }
}

Texture::~Texture()
{
    if(m_initialised)
    {
        glDeleteTextures(1, &m_id);
        m_initialised = false;
    }
}

const std::string& Texture::Name() const
{
    return m_name;
}

bool Texture::IsCubeMap() const
{
    return m_image == CUBE;
}

bool Texture::Initialise()
{
    glGenTextures(1, &m_id);
    m_initialised = true;

    auto type = IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    glBindTexture(type, m_id);

    bool success = false;
    if (m_image == CUBE)
    {
        success = InitialiseCubeMap();
    }
    else if (m_image == FROM_FILE)
    {
        success = InitialiseFromFile();
    }
    else
    {
        success = InitialiseFromPixels();
    }

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const auto GL_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FE;
    const float MAX_ANISOTROPY = 16.0f;
    glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, MAX_ANISOTROPY);

    if(!success || HasCallFailed())
    {
        LogError("Failed " + m_path + " texture");
        return false;
    }

    return true;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        m_size, m_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    if(HasCallFailed())
    {
        LogError("Failed " + m_name + " texture");
        return false;
    }

    return ReloadPixels();
}

bool Texture::ReloadPixels()
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size, 
        m_size, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixels[0]);

    if(HasCallFailed())
    {
        LogError("Failed " + m_name + " texture");
        return false;
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
        LogError("Failed " + path + " texture");
        return false;
    }
    return true;
}

GLuint Texture::GetID() const
{
    return m_id;
}

bool Texture::SaveTexture()
{
    const int channels = 3;
    std::vector<unsigned char> data(m_pixels.size() * channels);

    for (unsigned int i = 0, j = 0; i < m_pixels.size(); ++i, j+=channels)
    {
        const unsigned int colour = m_pixels[i];
        data[j] = Colour::RedAsChar(colour);
        data[j+1] = Colour::GreenAsChar(colour);
        data[j+2] = Colour::BlueAsChar(colour);
    }

    if (SOIL_save_image(m_path.c_str(), SOIL_SAVE_TYPE_BMP, 
        m_size, m_size, channels, &data[0]) == 0)
    {
        LogError("Failed to save " + m_path);
        return false;
    }
    else
    {
        LogInfo("Saved texture " + m_path);
        return true;
    }
}

void Texture::MakeRandomNormals()
{
    for (unsigned int i = 0; i < m_pixels.size(); ++i)
    {
        glm::vec3 colour(Random::Generate(0.0f, 1.0f),
                         Random::Generate(0.0f, 1.0f),
                         Random::Generate(0.0f, 1.0f));

        colour = glm::normalize(colour);
        Colour::SetRed(m_pixels[i], colour.x);
        Colour::SetGreen(m_pixels[i], colour.y);
        Colour::SetBlue(m_pixels[i], colour.z);
    }
}

bool Texture::IsProcedural() const
{
    return m_image != FROM_FILE && !IsCubeMap();
}