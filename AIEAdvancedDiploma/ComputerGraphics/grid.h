////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - grid.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "meshdata.h"

/**
* Holds information of a grid of vertices
*/
class Grid : public MeshData
{
protected:

    /**
    * Constructor
    * @param name The name of the data
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    */
    Grid(const std::string& name, 
         const std::string& shaderName, 
         int shaderID);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Loads a grid into the buffers
    * @param uvStretch Texture stretch multiplier
    * @param spacing The spacing between vertices
    * @param rows How many rows for the grid
    * @param columns How many columns for the grid
    * @param normals Whether to generate normals
    * @param tangents Whether to generate the tangent/bitangent
    */
    bool CreateGrid(const glm::vec2& uvStretch,
                    float spacing,
                    int rows, 
                    int columns, 
                    bool normals, 
                    bool tangents);

    /**
    * Resets the grid to default
    */
    void ResetGrid();

    /**
    * Sets the height at the given row/column
    */
    void SetHeight(int row, int column, float height);

    /**
    * @return the height at the given row/column
    */
    float GetHeight(int row, int column) const;

    /**
    * @return the number of rows of the grid
    */
    int Rows() const;

    /**
    * @return the number of columns of the grid
    */
    int Columns() const;

    /**
    * Determines the normals for the grid
    */
    void RecalculateNormals();

public:

    /**
    * @return the size of the grid
    * @note only valid for grids with the same rows/column size
    */
    float Size() const;

private:

    /**
    * Prevent copying
    */
    Grid(const Grid&) = delete;
    Grid& operator=(const Grid&) = delete;

    /**
    * @return the index of the start of the vertex
    * @param row The row of the vertex
    * @param column The column of the vertex
    */
    unsigned int GetIndex(int row, int column) const;

    /**
    * Gets the position from the vertex
    * @param index The index of the start of the vertex
    * @return the position
    */
    glm::vec3 GetPosition(int index) const;

    /**
    * Gets the normal from the vertex
    * @param index The index of the start of the vertex
    * @return the normal
    */
    glm::vec3 GetNormal(int index) const;

    /**
    * Gets the tangent from the vertex
    * @param index The index of the start of the vertex
    * @return the tangent
    */
    glm::vec3 GetTangent(int index) const;

    /**
    * Gets the uvs from the vertex
    * @param index The index of the start of the vertex
    * @return the uvs
    */
    glm::vec2 GetUVs(int index) const;

    glm::vec2 m_uvStretch;                      ///< Texture stretch multiplier
    float m_spacing = 0.0f;                     ///< The spacing between vertices
    int m_columns = 0;                          ///< The number of columns of this mesh
    int m_rows = 0;                             ///< The number of rows of this mesh
    bool m_hasNormals = false;                  ///< Whether to generate normals
    bool m_hasTangents = false;                 ///< Whether to generate tangent/bitangents
    std::vector<std::vector<float*>> m_heights; ///< Row x columns of the vertices
};