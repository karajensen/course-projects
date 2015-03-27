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
public:

    /**
    * Constructor
    * @param name The name of the data
    * @param shader The ID of the shader to use
    */
    Grid(const std::string& name, int shader);

    /**
    * Loads a grid into the buffers
    * @param position The center of the grid
    * @param spacing The spacing between vertices
    * @param rows How many rows for the grid
    * @param columns How many columns for the grid
    * @param normals Whether to generate normals
    */
    void CreateGrid(const glm::vec3& position, 
                    float spacing,
                    int rows, 
                    int columns, 
                    bool normals);

    /**
    * Resets the grid to default
    */
    void ResetGrid();

    /**
    * Sets the height at the given row/column
    */
    void Set(int row, int column, float height);

    /**
    * @return the value at the given row/column
    */
    float Get(int row, int column) const;

    /**
    * @return whether the given row/column is valid
    */
    bool Valid(int row, int column) const;

    /**
    * @return the number of rows of the grid
    */
    int Rows() const;

    /**
    * @return the number of columns of the grid
    */
    int Columns() const;

    /**
    * @return the initial position of the center of the grid
    */
    const glm::vec3& Position() const;

    /**
    * Determines the normals for the grid
    */
    void RecalculateNormals();

private:

    /**
    * Prevent copying
    */
    Grid(const Grid&) = delete;
    Grid& operator=(const Grid&) = delete;

    float m_spacing = 0.0f;                     ///< The spacing between vertices
    int m_columns = 0;                          ///< The number of columns of this mesh
    int m_rows = 0;                             ///< The number of rows of this mesh
    bool m_hasNormals = false;                  ///< Whether to generate normals
    glm::vec3 m_position;                       ///< The initial position of the grid
    std::vector<std::vector<float*>> m_heights; ///< Row x columns of the vertices
};