////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture.h"

/**
* Procedurally generated texture rendered on a mesh
*/
class ProceduralTexture : public Texture
{
public:

    /**
    * Avaliable types
    */
    enum Type
    {
        DIAMOND_SQUARE
    };

    /**
    * Constructor for a procedurally generated texture
    * @param name The filename of the texture
    * @param path The full path to save the texture
    * @param size The dimensions of the texture
    * @param type The type of texture to make
    * @param filter The type of filtering for this texture
    * @Param type The type of texture to create
    */
    ProceduralTexture(const std::string& name, 
                      const std::string& path,
                      int size,
                      Filter filter,
                      Type type);
                     
    /**
    * Destructor
    */
    ~ProceduralTexture();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Reloads the texture
    */
    virtual void Reload() override;

    /**
    * Saves a texture to file from its pixels
    */
    virtual void Save() override;

    /**
    * @return the pixels of the texture
    */
    virtual const std::vector<unsigned int>& GetPixels() const override;

private:

    /**
    * Prevent copying
    */
    ProceduralTexture(const ProceduralTexture&) = delete;
    ProceduralTexture& operator=(const ProceduralTexture&) = delete;

    /**
    * Generates the texture
    */
    void Generate();

    /**
    * Creates a fractal texture using the diamond square algorithm
    */
    void MakeDiamondSquareFractal();

    /**
    * Reloads the texture from pixels
    * @return whether reloading was successful
    */
    bool ReloadPixels();

    /**
    * Initialises the texture from pixels
    * @return whether initialisation was successful
    */
    virtual bool InitialiseFromPixels() override;

    /**
    * @return the colour from the row and column value
    */
    unsigned int Get(int row, int column) const;

    /**
    * @return the index from the row and column value
    */
    unsigned int Index(int row, int column) const;

    /**
    * Sets the colour from each component
    */
    void Set(unsigned int index, int r, int g, int b, int a);

    /**
    * Sets the colour from each component
    */
    void Set(unsigned int index, float r, float g, float b, float a);

    /**
    * Sets the colour as a float
    */
    void Set(int row, int column, float value);

    /**
    * Sets the colour component as an int
    */
    void SetRed(unsigned int index, int value);
    void SetGreen(unsigned int index, int value);
    void SetBlue(unsigned int index, int value);
    void SetAlpha(unsigned int index, int value);

    /**
    * Sets the colour component as a float
    */
    void SetRed(unsigned int index, float value);
    void SetGreen(unsigned int index, float value);
    void SetBlue(unsigned int index, float value);
    void SetAlpha(unsigned int index, float value);

    /**
    * Gets the colour component as a char
    */
    unsigned char RedAsChar(unsigned int index);
    unsigned char GreenAsChar(unsigned int index);
    unsigned char BlueAsChar(unsigned int index);
    unsigned char AlphaAsChar(unsigned int index);

    /**
    * Gets the colour component as an int
    */
    int RedAsInt(unsigned int index);
    int GreenAsInt(unsigned int index);
    int BlueAsInt(unsigned int index);
    int AlphaAsInt(unsigned int index);

    /**
    * Gets the colour component as a float
    */
    float RedAsFlt(unsigned int index);
    float GreenAsFlt(unsigned int index);
    float BlueAsFlt(unsigned int index);
    float AlphaAsFlt(unsigned int index);

    std::vector<unsigned int> m_pixels; ///< pixels of the texture
    int m_size = 0;                     ///< Dimensions of the texture
    Type m_type;                        ///< The type of texture this is
};