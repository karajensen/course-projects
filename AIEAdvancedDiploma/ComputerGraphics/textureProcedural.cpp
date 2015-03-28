////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"
#include "renderdata.h"
#include "soil/SOIL.h"

ProceduralTexture::ProceduralTexture(const std::string& name, 
                                     const std::string& path, 
                                     int size, 
                                     Filter filter) :

    Texture(name, path, PROCEDURAL, filter),
    m_size(size)
{
    m_pixels.resize(size * size);
    m_pixels.assign(m_pixels.size(), 0);
}

ProceduralTexture::~ProceduralTexture()
{
}

bool ProceduralTexture::InitialiseFromPixels()
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

bool ProceduralTexture::ReloadPixels()
{
    glBindTexture(GL_TEXTURE_2D, GetID());

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size, 
        m_size, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixels[0]);

    if(HasCallFailed())
    {
        LogError("Failed " + Name() + " texture");
        return false;
    }
    return true;
}

bool ProceduralTexture::SaveTexture()
{
    const int channels = 3;
    std::vector<unsigned char> data(m_pixels.size() * channels);

    for (unsigned int i = 0, j = 0; i < m_pixels.size(); ++i, j+=channels)
    {
        data[j] = RedAsChar(i);
        data[j+1] = GreenAsChar(i);
        data[j+2] = BlueAsChar(i);
    }

    if (SOIL_save_image(Path().c_str(), SOIL_SAVE_TYPE_BMP, 
        m_size, m_size, channels, &data[0]) == 0)
    {
        LogError("Failed to save " + Name());
        return false;
    }
    else
    {
        LogInfo("Saved texture " + Path());
        return true;
    }
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

void ProceduralTexture::Set(unsigned int index, int r, int g, int b, int a)
{
    using namespace std;
    r = min(255, max(0, r));
    g = min(255, max(0, g));
    b = min(255, max(0, b));
    a = min(255, max(0, a));
    m_pixels[index] = ((a & 0xFF) << 24) + 
                      ((b & 0xFF) << 16) + 
                      ((g & 0xFF) << 8) + 
                      (r & 0xFF);
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

bool ProceduralTexture::Valid(int row, int column) const
{
    return row >= 0 && column >= 0 && row < m_size && column < m_size;
}

bool ProceduralTexture::Valid(unsigned int index) const
{
    return index >= 0 && index < m_pixels.size();
}

void ProceduralTexture::MakeRandomNormals()
{
    for (unsigned int i = 0; i < m_pixels.size(); ++i)
    {
        glm::vec3 colour(Random::Generate(0.0f, 1.0f),
                         Random::Generate(0.0f, 1.0f),
                         Random::Generate(0.0f, 1.0f));

        colour = glm::normalize(colour);
        SetRed(i, colour.x);
        SetGreen(i, colour.y);
        SetBlue(i, colour.z);
    }
}

void ProceduralTexture::MakeDiamondSquareFractal()
{
    // Based on: http://www.gameprogrammer.com/fractal.html

	float scale = 2.0f;
    const int maxSize = m_size;
    const int maxIndex = maxSize - 1;
    int size = maxSize;
    int half = size / 2;

    // All four corners must have the same point to allow tiling
    const float initial = 0.0f;
    Set(0, 0, initial);
    Set(maxIndex, 0, initial);
    Set(maxIndex, maxIndex, initial);
    Set(0, maxIndex, initial);

    /**
    * Averages the value of the four points in a square
    * o  .  o
    * .  .  .
    * o  .  o
    */
    auto AverageSquare = [&](int row, int column) -> float
    {
        return (RedAsFlt(Index(row-half, column-half)) +
            RedAsFlt(Index(row+half-1, column+half-1)) +
            RedAsFlt(Index(row-half, column+half-1)) +
            RedAsFlt(Index(row+half-1, column-half))) * 0.25f;
    };

    /**
    * Averages the value of the four points in a diamond
    * If row/column is on edge of texture looks at opposite side to support tiling
    * .  o  .
    * o  .  o
    * .  o  .
    */
    auto AverageDiamond = [&](int row, int column) -> float
    {
        const auto top = Index(row, column-half);
        const auto bot = Index(row, column+half-1);
        const auto left = Index(row-half, column);
        const auto right = Index(row+half-1, column);

        if (row == 0)
        {
            return (RedAsFlt(top) + RedAsFlt(bot) + RedAsFlt(right)
                + RedAsFlt(Index(maxIndex-half, column))) * 0.25f;
        }
        else if (row == maxIndex)
        {
            return (RedAsFlt(top) + RedAsFlt(bot) + RedAsFlt(left) 
                + RedAsFlt(Index(half, column))) * 0.25f;
        }
        else if (column == 0)
        {
            return (RedAsFlt(right) + RedAsFlt(left) + RedAsFlt(bot) 
                + RedAsFlt(Index(row, maxIndex-half))) * 0.25f;
        }
        else if (column == maxIndex)
        {
            return (RedAsFlt(right) + RedAsFlt(left) + RedAsFlt(bot) 
                + RedAsFlt(Index(row, half))) * 0.25f;
        }

        return (RedAsFlt(right) + RedAsFlt(left) 
            + RedAsFlt(top) + RedAsFlt(bot)) * 0.25f;
    };

    // Terminate loop when power of 2 texture halves to reach 0.5
    while (half >= 1)
    {
        // Set the midpoint of the sections
        for (int r = half; r < maxSize; r += size) 
        {
            for (int c = half; c < maxSize; c += size)
            {
                Set(r, c, AverageSquare(r, c) + scale * Random::Generate(-0.5f, 0.5f));
            }
        }

        for (int r = 0; r <= maxIndex; r += half)
        {
            for (int c = (r + half) % size; c <= maxIndex; c += size)
            {
                Set(r, c, AverageDiamond(r, c) + scale * Random::Generate(-0.5f, 0.5f));
        
                if (r == 0) // Ensure opposite side has matching value for wrapping
                {
                    m_pixels[Index(maxIndex, c)] = m_pixels[Index(r, c)];
                }
                if (c == 0) // Ensure opposite side has matching value for wrapping
                {
                    m_pixels[Index(r, maxIndex)] = m_pixels[Index(r, c)];
                }
            }
        }

        size = half;
        half = size / 2;
    }   
}