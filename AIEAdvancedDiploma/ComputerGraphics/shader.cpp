////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "shader.h"
#include "renderdata.h"
#include "rendertarget.h"
#include <string>
#include <fstream>

namespace
{
    const std::string VERTEX_SHADER("_glsl_vert.fx");
    const std::string FRAGMENT_SHADER("_glsl_frag.fx");
    const std::string GLSL_IN_POSITION("in_Position");

}

Shader::ShaderConstants Shader::sm_constants;

void Shader::InitialiseConstants(const ShaderConstants& constants)
{
    sm_constants = constants;
}

Shader::Shader(const std::string& name, 
               const std::string& path,
               unsigned int components) :

    m_name(name),
    m_components(components),
    m_fragmentFile(path + FRAGMENT_SHADER),
    m_vertexFile(path + VERTEX_SHADER)
{
}

Shader::~Shader()
{
    if(m_program != NO_INDEX)
    {
        if(m_vs != NO_INDEX)
        {
            glDetachShader(m_program, m_vs);
        }
        if(m_fs != NO_INDEX)
        {
            glDetachShader(m_program, m_fs);
        }
        glDeleteProgram(m_program);
        m_program = NO_INDEX;
    }
    if(m_vs != NO_INDEX)
    {
        glDeleteShader(m_vs);
        m_vs = NO_INDEX;
    }
    if(m_fs != NO_INDEX)
    {
        glDeleteShader(m_fs);
        m_fs = NO_INDEX;
    }
}

bool Shader::Initialise()
{
    if (LoadShaderFile(m_vertexFile, m_vertexText) &&
        LoadShaderFile(m_fragmentFile, m_fragmentText))
    {
        m_vs = glCreateShader(GL_VERTEX_SHADER);
        m_fs = glCreateShader(GL_FRAGMENT_SHADER);
        m_program = glCreateProgram();

        if (CompileShader("vertex", m_vs, m_vertexText) &&
            CompileShader("fragment", m_fs, m_fragmentText) &&
            LinkShaderProgram() && 
            BindVertexAttributes() &&
            BindFragmentAttributes() && 
            FindShaderUniforms())
        {
            LogInfo("Shader: " + m_name + " compiled");
            return true;
        }
    }
    return false;
}

bool Shader::HasComponent(Component component) const
{
    return (m_components & component) == component;
}

bool Shader::LoadShaderFile(const std::string& loadPath, std::string& text)
{
    std::ifstream file(loadPath.c_str(), std::ios_base::in|std::ios_base::_Nocreate);
    if(!file.is_open())
    {
        LogShader("Could not open " + loadPath);
        return false;
    }

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);

        for (const auto& constant : sm_constants)
        {
            int index = line.find(constant.first);
            while (index != NO_INDEX)
            {
                const int size = static_cast<int>(constant.first.size());
                line.replace(index, size, constant.second);
                index = line.find(constant.first);
            }
        }

        text += line + "\n";
    }

    file.close();
    return true;
}

bool Shader::CompileShader(const std::string& shader, GLint index, const std::string& text)
{
    const char* source = text.c_str();
    const int size = static_cast<int>(text.size());

    GLint success = GL_FALSE;
    glShaderSource(index, 1, &source, &size);
    glCompileShader(index);
    glGetShaderiv(index, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        // Get the compilation error message
        int errorLength = 0;
        glGetShaderiv(index, GL_INFO_LOG_LENGTH, &errorLength);
        if(errorLength <= 0)
        {
            LogShader("Unknown Error");
            return false;
        }
        else
        {
            std::string errors(errorLength, '\0');
            glGetShaderInfoLog(index, errorLength, 0, &errors[0]);
            LogShader(shader + ": " + std::string(errors.begin(), errors.begin() + errors.find('\0')));
            return false;
        }
    }
    return true;
}

bool Shader::LinkShaderProgram()
{
    glAttachShader(m_program, m_vs);
    if(HasCallFailed())
    {
        LogShader("Vertex Shader Failed to attach");
        return false;
    }

    glAttachShader(m_program, m_fs);
    if(HasCallFailed())
    {
        LogShader("Fragment Shader Failed to attach");
        return false;
    }

    GLint linkSuccess = GL_FALSE;
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess == GL_FALSE)
    {
        const int bufferSize = 1024;
        std::string errors(bufferSize, '\0');
        glGetProgramInfoLog(m_program, bufferSize, 0, &errors[0]);
        LogShader(std::string(errors.begin(), errors.begin() + errors.find('\0')));
        return false;
    }
    return true;
}

bool Shader::BindFragmentAttributes()
{
    glBindFragDataLocation(m_program, 0, "");
    if(HasCallFailed())
    {
        LogShader("Failed to bind fragment location");
        return false;
    }

    return true;
}

bool Shader::BindVertexAttributes()
{
    int maxLength;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    int attributeCount;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    
    if(HasCallFailed())
    {
        LogShader("Could not get attribute count");
        return false;
    }

    m_stride = 0;
    for (int i = 0; i < attributeCount; ++i)
    {
        int size;
        GLenum type;
        std::string name(maxLength,'\0');
        glGetActiveAttrib(m_program, i, maxLength, 0, &size, &type, &name[0]);
        name = std::string(name.begin(), name.begin() + name.find('\0'));
        
        if(HasCallFailed())
        {
            LogShader("Could not get attribute " + std::to_string(i));
            return false;
        }

        m_attributes.emplace_back();
        m_attributes[i].location = i;
        m_attributes[i].name = name;

        // Pass position as a vec3 into a vec4 slot to use the optimization
        // where the 'w' component is automatically set as 1.0
        const bool isPosition = IsStrEqual(m_attributes[i].name, GLSL_IN_POSITION);
        m_attributes[i].components = GetComponents(isPosition ? GL_FLOAT_VEC3 : type);

        m_stride += m_attributes[i].components;
        glBindAttribLocation(m_program, i, name.c_str());

        if(HasCallFailed())
        {
            LogShader("Failed to bind attribute " + name);
            return false;
        }
    }
    return true;
}

bool Shader::FindShaderUniforms()
{
    int maxLength;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    int uniformCount;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);

    if(HasCallFailed())
    {
        LogShader("Could not get uniform count");
        return false;
    }

    for (int i = 0; i < uniformCount; ++i)
    {
        int size;
        GLenum type;
        std::string name(maxLength,'\0');
        glGetActiveUniform(m_program, i, maxLength, 0, &size, &type, &name[0]);
        name = std::string(name.begin(), name.begin() + name.find('\0'));
        if(HasCallFailed())
        {
            LogShader("Could not get uniform " + std::to_string(i));
            return false;
        }

        GLint location = glGetUniformLocation(m_program, name.c_str());
        if(HasCallFailed() || location == NO_INDEX)
        {
            LogShader("Could not find uniform " + name);
            return false;
        }
        
        if(type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_MULTISAMPLE || type == GL_SAMPLER_CUBE)
        {
            m_samplers.push_back(location);
        }
        else
        {   
            // Scratch buffer not allocated for matrices
            const bool isMatrix = type == GL_FLOAT_MAT4;
            const int scratchBufferSize = isMatrix ? 0 : GetComponents(type) * size;

            m_uniforms[name].scratch.resize(scratchBufferSize);
            m_uniforms[name].scratch.assign(scratchBufferSize, 0.0f);
            m_uniforms[name].location = location;
            m_uniforms[name].type = type;
            m_uniforms[name].size = size;
        }
    }

    return true;
}

void Shader::SendUniform(const std::string& name, const glm::mat4& matrix)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end())
    {
        glUniformMatrix4fv(itr->second.location, 1, GL_FALSE, &matrix[0][0]);

        if (itr->second.type != GL_FLOAT_MAT4)
        {
            LogShader("Uniform " + name + " isn't a matrix");
        }

        if(HasCallFailed())
        {
            LogShader("Could not send uniform " + name);
        }
    }
}

void Shader::UpdateUniformArray(const std::string& name, const float* value, int count, int offset)
{
    auto itr = m_uniforms.find(name);
    if (itr != m_uniforms.end())
    {
        for(int i = offset, j = 0; j < count; ++i, ++j)
        {
            itr->second.scratch[i] = value[j];
        }

        itr->second.updated = true;
    }
}

void Shader::SendUniformArrays()
{
    for (auto& uniform : m_uniforms)
    {
        if (uniform.second.updated)
        {
            uniform.second.updated = false;
            SendUniformFloat(
                uniform.first, 
                &uniform.second.scratch[0],
                uniform.second.location,
                uniform.second.size,
                uniform.second.type);
        }
    }
}

void Shader::SendUniformFloat(const std::string& name, 
                              const float* value, 
                              int location, 
                              int size, 
                              GLenum type)
{
    switch(type)
    {
    case GL_FLOAT:
        glUniform1fv(location, size, value);
        break;
    case GL_FLOAT_VEC2:
        glUniform2fv(location, size, value);
        break;
    case GL_FLOAT_VEC3:
        glUniform3fv(location, size, value);
        break;
    case GL_FLOAT_VEC4:
        glUniform4fv(location, size, value);
        break;
    default:
        LogShader("Unknown uniform type " + name);
    }

    if(HasCallFailed())
    {
        LogShader("Could not send uniform " + name);
    }
}

void Shader::SendUniform(const std::string& name, const glm::vec4& value, int offset)
{
    offset == NO_INDEX ? 
        SendUniformFloat(name, &value.x, 4) :
        UpdateUniformArray(name, &value.x, 4, offset);
}

void Shader::SendUniform(const std::string& name, const glm::vec3& value, int offset)
{
    offset == NO_INDEX ? 
        SendUniformFloat(name, &value.x, 3) :
        UpdateUniformArray(name, &value.x, 3, offset);
}

void Shader::SendUniform(const std::string& name, const glm::vec2& value, int offset)
{
    offset == NO_INDEX ? 
        SendUniformFloat(name, &value.x, 2) :
        UpdateUniformArray(name, &value.x, 2, offset);
}

void Shader::SendUniform(const std::string& name, float value, int offset)
{
    offset == NO_INDEX ? 
        SendUniformFloat(name, &value, 1) :
        UpdateUniformArray(name, &value, 1, offset);
}

void Shader::SendUniformFloat(const std::string& name, const float* value, int count)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end())
    {
        if (itr->second.scratch.size() != count)
        {
            LogShader("Size for uniform " + name + " doesn't match");
        }

        SendUniformFloat(name, value, itr->second.location, 
            itr->second.size, itr->second.type);
    }
}

void Shader::EnableShader()
{
    SendUniformArrays();

    int offset = 0;
    for(const AttributeData& attr : m_attributes)
    {
        glEnableVertexAttribArray(attr.location);
        if(HasCallFailed())
        {
            LogShader("Could not enable attribute " + attr.name);
        }
    
        glVertexAttribPointer(attr.location, attr.components, GL_FLOAT, 
            GL_FALSE, m_stride*sizeof(GLfloat), (void*)(offset*sizeof(GLfloat)));
        if(HasCallFailed())
        {
            LogShader("Could not set attribute " + attr.name);
        }
    
        offset += attr.components;
    }
}

void Shader::ClearTexture(int slot, bool multisample, bool cubemap)
{
    glActiveTexture(GetTexture(slot));

    if (cubemap)
    {
        glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
    }
    else
    {
        glBindTexture(!multisample ? GL_TEXTURE_2D :
            GL_TEXTURE_2D_MULTISAMPLE, 0);
    }

    if(HasCallFailed())
    {
        LogShader("Could not clear texture");
    }
}

void Shader::SendTexture(int slot, const RenderTarget& target, int ID)
{
    SendTexture(slot, target.GetTexture(ID), target.IsMultisampled(), false);
}

void Shader::ClearTexture(int slot, const RenderTarget& target)
{
    ClearTexture(slot, target.IsMultisampled(), false);
}

void Shader::SendTexture(int slot, GLuint id, bool cubemap)
{
    SendTexture(slot, id, false, cubemap);
}

void Shader::SendTexture(int slot, GLuint id, bool multisample, bool cubemap)
{
    glActiveTexture(GetTexture(slot));

    if (cubemap)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }
    else
    {
        glBindTexture(!multisample ? GL_TEXTURE_2D :
            GL_TEXTURE_2D_MULTISAMPLE, id);
    }

    glUniform1i(m_samplers[slot], slot);

    if (HasCallFailed())
    {
        LogShader("Could not send texture");
    }
}

void Shader::SetActive()
{
    glUseProgram(m_program);
}

bool Shader::HasTextureSlot(int slot)
{
    return slot < static_cast<int>(m_samplers.size());
}

unsigned int Shader::GetTexture(int slot)
{
    switch(slot)
    {
    case 0:
        return GL_TEXTURE0;
    case 1:
        return GL_TEXTURE1;
    case 2:
        return GL_TEXTURE2;
    case 3:
        return GL_TEXTURE3;
    case 4:
        return GL_TEXTURE4;
    case 5:
        return GL_TEXTURE5;
    case 6:
        return GL_TEXTURE6;
    case 7:
        return GL_TEXTURE7;
    case 8:
        return GL_TEXTURE8;
    default:
        LogError("GetTexture: Unknown slot");
        return 0;
    }
}

int Shader::GetComponents(GLenum type)
{
    switch(type)
    {
    case GL_FLOAT_MAT4:
        return 16;
    case GL_FLOAT:
        return 1;
    case GL_FLOAT_VEC2:
        return 2;
    case GL_FLOAT_VEC3:
        return 3;
    case GL_FLOAT_VEC4:
        return 4;
    default:
        LogError("GetComponents: Unknown type");
        return 0;
    }
}

void Shader::LogShader(const std::string& text)
{
    LogError("Shader " + m_name + ": " + text);
}

const std::string& Shader::Name() const
{
    return m_name;
}