////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"
#include "renderdata.h"
#include "soil/SOIL.h"
#include "glm/gtc/noise.hpp"
#include "tweaker.h"

ProceduralTexture::ProceduralTexture(const std::string& name, 
                                     const std::string& path,
                                     int size,
                                     Generation generation) :

    Texture(name, path, PROCEDURAL, NEAREST),
    m_size(size),
    m_generation(generation)
{
    m_pixels.resize(size * size);
    m_pixels.assign(m_pixels.size(), 0);

    switch (m_generation)
    {
    case PERLIN_NOISE_ROCK:
        m_scale = 3.0f;
        m_iterations = 6;
        m_amplitude = 1.0f;
        m_contrast = 1.0f;
        break;
    }

    Generate();
}

ProceduralTexture::~ProceduralTexture()
{
}

void ProceduralTexture::AddToTweaker(Tweaker& tweaker)
{
    Texture::AddToTweaker(tweaker);

    tweaker.AddEntry("Size", &m_size, TW_TYPE_INT32, true);
    tweaker.AddStrEntry("Type", GetGenerationName());

    if (m_generation != FROM_FILE)
    {
        tweaker.AddFltEntry("Scale", &m_scale, 0.01f);
        tweaker.AddFltEntry("Amplitude", &m_amplitude, 0.01f);
        tweaker.AddFltEntry("Contrast", &m_contrast, 0.01f);
        tweaker.AddIntEntry("Iterations", &m_iterations, 1, 100);
        tweaker.AddButton("Reload", [this](){ Reload(); });
        tweaker.AddButton("Save", [this](){ Save(); });
    }
}

std::string ProceduralTexture::GetGenerationName() const
{
    switch (m_generation)
    {
    case PERLIN_NOISE_ROCK:
        return "Perlin Noise Rock";
    case FROM_FILE:
        return "From File";
    default:
        return "None";
    }
}

void ProceduralTexture::MakeFromFile()
{
    int width, height;
    unsigned char* image = SOIL_load_image(Path().c_str(), 
        &width, &height, 0, SOIL_LOAD_RGBA);

    assert(m_size == width);
    assert(m_size == height);

    const int channels = 4;
    const int size = m_size * m_size * channels;
    for (int i = 0, j = 0; i < size; i += channels, ++j)
    {
        const auto r = image[i];
        const auto g = image[i+1];
        const auto b = image[i+2];
        const auto a = image[i+3];
        m_pixels[j] = Convert(r, g, b, a);
    }

    SOIL_free_image_data(image);
}

void ProceduralTexture::Generate()
{
    switch (m_generation)
    {
    case FROM_FILE:
        MakeFromFile();
        break;
    case PERLIN_NOISE_ROCK:
        MakePerlinNoiseRock();
        break;
    }
}

bool ProceduralTexture::InitialiseFromPixels()
{
    if (m_renderableTexture)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
            m_size, m_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        if(HasCallFailed())
        {
            LogError("Failed to load " + Name() + " texture");
            return false;
        }

        return SetFiltering() && ReloadPixels();
    }
    return true;
}

bool ProceduralTexture::ReloadPixels()
{
    if (m_renderableTexture)
    {
        glBindTexture(GL_TEXTURE_2D, GetID());

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size, 
            m_size, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixels[0]);

        if(HasCallFailed())
        {
            LogError("Failed " + Name() + " texture");
            return false;
        }
    }
    return true;
}

void ProceduralTexture::Reload()
{
    if (m_generation != FROM_FILE)
    {
        Generate();

        if (!ReloadPixels())
        {
            LogError("Texture: Reload failed for " + Name());
        }
    }
}

void ProceduralTexture::Save()
{
    if (m_generation != FROM_FILE)
    {
        const int channels = 3;
        std::vector<unsigned char> data(m_pixels.size() * channels);

        for (unsigned int i = 0, j = 0; i < m_pixels.size(); ++i, j += channels)
        {
            data[j] = RedAsChar(i);
            data[j + 1] = GreenAsChar(i);
            data[j + 2] = BlueAsChar(i);
        }

        if (SOIL_save_image(Path().c_str(), SOIL_SAVE_TYPE_BMP,
            m_size, m_size, channels, &data[0]) == 0)
        {
            LogError("Failed to save " + Name());
        }
        else
        {
            LogInfo("Saved texture " + Path());
        }
    }
}

const std::vector<unsigned int>& ProceduralTexture::GetPixels() const
{
    return m_pixels;
}

void ProceduralTexture::SetRed(unsigned int index, int value)
{
    Set(index, value, GreenAsInt(index),
        BlueAsInt(index), AlphaAsInt(index));
}

void ProceduralTexture::SetGreen(unsigned int index, int value)
{
    Set(index, RedAsInt(index), value, 
        BlueAsInt(index), AlphaAsInt(index));
}

void ProceduralTexture::SetBlue(unsigned int index, int value)
{
    Set(index, RedAsInt(index), 
        GreenAsInt(index), value, AlphaAsInt(index));
}

void ProceduralTexture::SetAlpha(unsigned int index, int value)
{
    Set(index, RedAsInt(index), 
        GreenAsInt(index), BlueAsInt(index), value);
}

void ProceduralTexture::SetRed(unsigned int index, float value)
{
    SetRed(index, static_cast<int>(value * 255));
}

void ProceduralTexture::SetGreen(unsigned int index, float value)
{
    SetGreen(index, static_cast<int>(value * 255));
}

void ProceduralTexture::SetBlue(unsigned int index, float value)
{
    SetBlue(index, static_cast<int>(value * 255));
}

void ProceduralTexture::SetAlpha(unsigned int index, float value)
{
    SetAlpha(index, static_cast<int>(value * 255));
}

void ProceduralTexture::Set(int row, int column, float value)
{
    Set(Index(row, column), value, value, value, value);
}

void ProceduralTexture::Set(unsigned int index, float r, float g, float b, float a)
{
    Set(index, 
        static_cast<int>(r * 255.0f), 
        static_cast<int>(g * 255.0f),
        static_cast<int>(b * 255.0f), 
        static_cast<int>(a * 255.0f));
}

unsigned int ProceduralTexture::Convert(int r, int g, int b, int a) const
{
    return ((a & 0xFF) << 24) + 
           ((b & 0xFF) << 16) + 
           ((g & 0xFF) << 8) + 
           (r & 0xFF);
}

void ProceduralTexture::Set(unsigned int index, int r, int g, int b, int a)
{
    using namespace std;
    r = min(255, max(0, r));
    g = min(255, max(0, g));
    b = min(255, max(0, b));
    a = min(255, max(0, a));
    m_pixels[index] = Convert(r, g, b, a);
}

unsigned char ProceduralTexture::RedAsChar(unsigned int index)
{
    return m_pixels[index] & 0xFF;
}

unsigned char ProceduralTexture::GreenAsChar(unsigned int index)
{
    return (m_pixels[index] >> 8) & 0xFF;
}

unsigned char ProceduralTexture::BlueAsChar(unsigned int index)
{
    return (m_pixels[index] >> 16) & 0xFF;
}

unsigned char ProceduralTexture::AlphaAsChar(unsigned int index)
{
    return (m_pixels[index] >> 24) & 0xFF;
}

int ProceduralTexture::RedAsInt(unsigned int index)
{
    return RedAsChar(index);
}

int ProceduralTexture::GreenAsInt(unsigned int index)
{
    return GreenAsChar(index);
}

int ProceduralTexture::BlueAsInt(unsigned int index)
{
    return BlueAsChar(index);
}

int ProceduralTexture::AlphaAsInt(unsigned int index)
{
    return AlphaAsChar(index);
}

float ProceduralTexture::RedAsFlt(unsigned int index)
{
    return static_cast<float>(RedAsChar(index)) / 255.0f;
}

float ProceduralTexture::GreenAsFlt(unsigned int index)
{
    return static_cast<float>(GreenAsChar(index)) / 255.0f;
}

float ProceduralTexture::BlueAsFlt(unsigned int index)
{
    return static_cast<float>(BlueAsChar(index)) / 255.0f;
}

float ProceduralTexture::AlphaAsFlt(unsigned int index)
{
    return static_cast<float>(AlphaAsChar(index)) / 255.0f;
}

unsigned int ProceduralTexture::Get(int row, int column) const
{
    return m_pixels[Index(row, column)];
}

unsigned int ProceduralTexture::Index(int row, int column) const
{
    return row * m_size + column;
}

GLuint ProceduralTexture::GetID() const
{
    assert(m_renderableTexture);
    return Texture::GetID();
}

void ProceduralTexture::MakePerlinNoiseRock()
{
    // Randomize each call of the noise
    const int randomR = Random::Generate(0, 1000);
    const int randomC = Random::Generate(0, 1000);

    const float persistance = 0.3f;
    const float scale = (1.0f / m_size) * m_scale;
    const int fadeIndex = 10;

    const int boundary = 10;
    const float outerRadius = static_cast<float>(m_size-boundary) * 0.5f;
    const float innerRadius = outerRadius * 0.6f;
    const glm::vec2 center(m_size / 2.0f, m_size / 2.0f);

    for (int r = 0; r < m_size; ++r)
    {
        for (int c = 0; c < m_size; ++c)
        {
            float amplitude = m_amplitude;
            float value = 0.0f;

            for (int i = 0; i < m_iterations; ++i)
            {
                const float frequency = powf(2, static_cast<float>(i));
                const glm::vec2 position(randomR + r, randomC + c);
                const float noise = glm::perlin(position * scale * frequency);
                value += ((noise * 0.5f) + 0.5f) * amplitude;
                amplitude *= persistance;
            }

            // Contrast Reference: Programming vertex, geometry and pixel shaders p378-379
            value -= m_contrast * (value - 1.0f) * value * (value - 0.5f);

            // Fade the boundry out to allow aligning with flat ground
            const float distance = glm::length(center - glm::vec2(r, c));
            value *= Clamp(ConvertRange(distance, innerRadius, outerRadius, 1.0f, 0.0f), 0.0f, 1.0f);

            Set(r, c, value);
        }
    }
    LogInfo("Texture: " + Name() + " generated");
}