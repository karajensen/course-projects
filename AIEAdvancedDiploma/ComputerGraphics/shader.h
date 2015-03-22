////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "renderdata.h"

class RenderTarget;
class PostProcessing;

/**
* Shader used to render a mesh
*/
class Shader
{
public:

    /**
    * Index for avaliable shaders
    */
    enum Index
    {
        ID_DIFFUSE,
        ID_WATER,
        ID_PARTICLE,
        ID_BUMP,
        ID_BUMP_SPEC_CAUSTICS,
        ID_DIFFUSE_CAUSTICS,
        ID_FLAT,
        ID_POST_PROCESSING,
        ID_PRE_PROCESSING,
        ID_BLUR_HORIZONTAL,
        ID_BLUR_VERTICAL,
        MAX_SHADERS
    };

    /**
    * All shader basic component masks
    */
    enum Component
    {
        NONE = 0,
        BUMP = 1,
        SPECULAR = 2,
        CAUSTICS = 4
    };

    /**
    * Constructor
    * @param name The name of the shader
    * @param components The components this shader supports
    */
    Shader(const std::string& name, 
           const std::string& path,
           unsigned int components);

    /**
    * Destructor
    */
    ~Shader();

    /**
    * Initialises the shader
    * @return whether initialisation was successful
    */
    bool Initialise();

    /**
    * Determines whether the shader has the component
    * @param component The component to query for text
    * @return whether The shader has the component
    */
    bool HasComponent(Component component) const;

    /**
    * Sets the shader as activated for rendering
    */
    void SetActive();

    /**
    * Sends a matrix to the shader
    * @param name Name of the matrix to send. This must match on the shader to be successful
    * @param matrix The matrix to send
    */
    void SendUniform(const std::string& name, const glm::mat4& matrix);

    /**
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The float value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, float value, int offset = NO_INDEX);

    /**
    * Sends the vector to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The vector value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, const glm::vec2& value, int offset = NO_INDEX);

    /**
    * Sends the vector to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The vector value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, const glm::vec3& value, int offset = NO_INDEX);

    /**
    * Sends the vector to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The vector value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, const glm::vec4& value, int offset = NO_INDEX);

    /**
    * Enables the vertex shader 'in' attributes for the shader
    * This is required after the shader is active and when a mesh buffer is bound
    * Also sends any arrays in the scratch buffer to the shader
    */
    void EnableShader();

    /**
    * Determines whether the texture slot is available for the texture
    * @param slot The slot to query
    * @return whether the shader allows the slot to be bound to
    */
    bool HasTextureSlot(int slot);

    /**
    * Sends a texture to the shader
    * @param slot The slot to put the texture in
    * @param id The unique id for the opengl texture
    * @param cubemap Whether this texture is a cubemap
    */
    void SendTexture(int slot, GLuint id, bool cubemap);

    /**
    * Sends the render target texture to the shader
    * @param slot The slot to put the texture in
    * @param target The render target to send
    * @param ID the id of the target texture to send
    */
    void SendTexture(int slot, const RenderTarget& target, int ID = 0);

    /**
    * Clears the render target texture from the shader
    * @param slot The slot to put the texture in
    * @param target The render target to clear
    * @param ID the id of the target texture to clear
    */
    void ClearTexture(int slot, const RenderTarget& target);

    /**
    * Initialises the defined constants in the shaders
    * @param constants The constants to set
    */
    typedef std::vector<std::pair<std::string, std::string>> ShaderConstants;
    static void InitialiseConstants(const ShaderConstants& constants);

private:

    /**
    * Prevent copying
    */
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
    * Logs error information about the shader
    * @param text The text to log
    * @Param error Whether this entry is an error
    */
    void LogShader(const std::string& text);

    /**
    * Sends the array buffers to the shader if they have been updated
    */
    void SendUniformArrays();

    /**
    * Updates the cached array scratch buffer
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param count The number of floats to send
    * @param offset The index offset into the array
    */
    void UpdateUniformArray(const std::string& name, const float* value, int count, int offset);

    /**
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param count The number of floats to send
    */
    void SendUniformFloat(const std::string& name, const float* value, int count);

    /**
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param location Unique location within the shader
    * @param size The number of elements in the array (1 if not an array)
    * @param type Whether a float, vec2, vec3, vec4 
    */
    void SendUniformFloat(const std::string& name, const float* value, 
                          int location, int size, GLenum type);

    /**
    * Generates a shader from the given shader file
    * @param loadPath The path to the file
    * @param text The container to save the text to
    * @return whether the file was successfully loaded
    */
    bool LoadShaderFile(const std::string& loadPath, std::string& text);

    /**
    * Sends a texture to the shader
    * @param slot The slot to put the texture in
    * @param id The unique id for the opengl texture
    * @param multisample Whether this texture is to be multisampled
    * @param cubemap Whether this texture is a cubemap
    */
    void SendTexture(int slot, GLuint id, bool multisample, bool cubemap);

    /**
    * Clears the current texture set
    * @param slot The slot to put the texture in
    * @param multisample Whether this texture is to be multisampled
    * @param cubemap Whether this texture is a cubemap
    */
    void ClearTexture(int slot, bool multisample, bool cubemap);

    /**
    * Determines the output fragment attributes and binds them
    * @return if the call was successful
    */
    bool BindFragmentAttributes();

    /**
    * Determines the input vertex attributes and binds them
    * @return if the call was successful
    */
    bool BindVertexAttributes();

    /**
    * Determines the shader non-attribute uniform variables and samplers
    * @return if the call was successful
    */
    bool FindShaderUniforms();

    /**
    * Generates the shader for the engine
    * @param shader The description of the shader (vertex or fragment)
    * @param index The unique index of the shader (vs or fs) to compile
    * @param text Vertex or fragment shader text
    * @return Whether compilation was successful
    */
    bool CompileShader(const std::string& shader, GLint index, const std::string& text);

    /**
    * Links together all shaders within the shader program
    * @return if the call was successful
    */
    bool LinkShaderProgram();
                    
    /**
    * Determines the unique ID of the texture slot
    * @param slot The slot in the shader the texture will fill
    * @return the unique ID of the texture
    */
    static unsigned int GetTexture(int slot);

    /**
    * Determines the amount of float components from the OpenGL type
    * @param type The type of object (ie. float, vec2)
    * @return the number of floats in the object
    */
    static int GetComponents(GLenum type);

    /**
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int components = 0;  ///< Number of float components in the type
        int location = 0;    ///< The index location of the attribute
        std::string name;    ///< The name of the attribute
    };

    /**
    * Information for a non-attribute shader uniform
    * @note scratch is not allocated for matrices
    */
    struct UniformData
    {
        GLenum type = 0;            ///< Whether a float, vec2, vec3, vec4
        int location = 0;           ///< Unique location within the shader
        int size = 0;               ///< The number of elements in the array (1 if not an array)
        std::vector<float> scratch; ///< Buffer allocated with the total floats used
        bool updated = false;       ///< Whether the scratch buffer has been updated
    };

    typedef std::unordered_map<std::string, UniformData> UniformMap;

    UniformMap m_uniforms;                    ///< Vertex and fragment non-attribute uniform data
    std::vector<int> m_samplers;              ///< Fragment shader sampler locations
    std::vector<AttributeData> m_attributes;  ///< Vertex shader input attributes
    GLint m_program = NO_INDEX;               ///< Shader program
    GLint m_vs = NO_INDEX;                    ///< GLSL Vertex Shader
    GLint m_fs = NO_INDEX;                    ///< GLSL Fragment Shader
    GLsizei m_stride = 0;                     ///< Stride required for vertex attributes
    const std::string m_name;                 ///< name of the shader
    const std::string m_vertexFile;           ///< filename of the glsl shader
    const std::string m_fragmentFile;         ///< filename of the glsl shader
    const unsigned int m_components;          ///< Sections that make up this shader
    std::string m_vertexText;                 ///< The vertex shader string
    std::string m_fragmentText;               ///< The fragment shader string
    static ShaderConstants sm_constants;      ///< Holds constant data to substitute into shaders
};                              