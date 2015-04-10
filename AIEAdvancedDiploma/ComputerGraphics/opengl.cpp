////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl.h"
#include "renderdata.h"
#include "sceneInterface.h"
#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "water.h"
#include "emitter.h"
#include "terrain.h"
#include "quad.h"
#include "shader.h"
#include "texture.h"
#include "rendertarget.h"
#include "postprocessing.h"

OpenGL::OpenGL(const IScene& scene, const Camera& camera) :
    m_scene(scene),
    m_camera(camera)
{
}

OpenGL::~OpenGL()
{
    Release();
}

void OpenGL::Release()
{
    // All resources must be destroyed before the engine
    m_screenQuad.reset();
    m_backBuffer.reset();
    m_sceneTarget.reset();

    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

bool OpenGL::IsRunning() const
{
    return !glfwWindowShouldClose(m_window) && 
          glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

bool OpenGL::Initialise()
{
    if (!glfwInit())
    {
        LogError("OpenGL: Could not initialize GLFW");
        return false;
    }

    m_window = glfwCreateWindow(WINDOW_WIDTH, 
        WINDOW_HEIGHT, "Computer Graphics", nullptr, nullptr);

    if (!m_window)
    {
        LogError("OpenGL: Could not create window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) 
    {
        LogError("OpenGL: OGL Load Failed");
        return false;
    }

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glFrontFace(GL_CCW); 
    glDisablei(GL_BLEND, 0);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    if (HasCallFailed())
    {
        LogError("OpenGL: Failed to initialise scene");
        return false;
    }

    LogInfo("OpenGL: Initialised " + std::to_string(ogl_GetMajorVersion()) +
        "." + std::to_string(ogl_GetMinorVersion()));

    m_backBuffer = std::make_unique<RenderTarget>("BackBuffer");
    m_sceneTarget = std::make_unique<RenderTarget>("Scene", RenderTarget::SCENE_TEXTURES, true);
    m_effectsTarget = std::make_unique<RenderTarget>("Effects", RenderTarget::EFFECTS_TEXTURES, false);
    m_blurTarget = std::make_unique<RenderTarget>("Blur", RenderTarget::BLUR_TEXTURES, false, true);

    if (!m_backBuffer->Initialise() ||
        !m_sceneTarget->Initialise() ||
        !m_effectsTarget->Initialise() ||
        !m_blurTarget->Initialise())
    {
        LogError("Render targets failed initialisation");
        return false;
    }

    m_screenQuad = std::make_unique<Quad>("ScreenQuad");
    if (!m_screenQuad->Initialise())
    {
        LogError("Screen quad failed initialisation");
        return false;
    }

    return true;
}

GLFWwindow& OpenGL::GetWindow() const
{
    assert(m_window);
    return *m_window;
}

void OpenGL::RenderScene(float timer)
{
    RenderSceneMap(timer);
    RenderPreEffects();
    RenderBlur();
    RenderPostProcessing();
}

void OpenGL::EndRender()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void OpenGL::RenderSceneMap(float timer)
{
    m_sceneTarget->SetActive();

    if (m_isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    }

    RenderTerrain();
    RenderMeshes();
    RenderWater(timer);
    RenderEmitters();

    if (m_isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
    }
}   

void OpenGL::RenderMeshes()
{
    for (const auto& mesh : m_scene.Meshes())
    {
        if (UpdateShader(*mesh))
        {
            mesh->PreRender();
            EnableSelectedShader();
            mesh->Render([this](const glm::mat4& world)
            { 
                UpdateShader(world); 
            });
        }
    }
}

void OpenGL::RenderTerrain()
{
    for (const auto& terrain : m_scene.Terrains())
    {
        if (UpdateShader(*terrain))
        {
            terrain->PreRender();
            EnableSelectedShader();
            terrain->Render([this](const glm::mat4& world)
            { 
                UpdateShader(world); 
            });
        }
    }
}

void OpenGL::RenderWater(float timer)
{
    for (const auto& water : m_scene.Waters())
    {
        if (UpdateShader(*water, timer))
        {
            water->PreRender();
            EnableSelectedShader();
            water->Render([this](const glm::mat4& world)
            { 
                UpdateShader(world); 
            });
        }
    }
}

void OpenGL::RenderEmitters()
{
    EnableDepthWrite(false);

    auto updateParticle = [this](const glm::mat4& world, const Particle& particle)
    {
        UpdateShader(world, particle); 
    };

    for (auto& emitter : m_scene.Emitters())
    {
        if (UpdateShader(*emitter))
        {
            emitter->PreRender();
            EnableSelectedShader();
            emitter->Render(updateParticle, m_camera.Position(), m_camera.Up());
        }
    }

    EnableDepthWrite(true);
}

void OpenGL::RenderPostProcessing()
{
    EnableAlphaBlending(false);
    EnableBackfaceCull(false);

    SetSelectedShader(Shader::ID_POST_PROCESSING);
    auto& postShader = m_scene.GetShader(Shader::ID_POST_PROCESSING);
    const auto& post = m_scene.Post();

    postShader.SendUniform("contrast", post.Contrast());
    postShader.SendUniform("saturation", post.Saturation());
    postShader.SendUniform("dofStart", post.DOFStart());
    postShader.SendUniform("dofFade", post.DOFFade());
    postShader.SendUniform("fogStart", post.FogStart());
    postShader.SendUniform("fogFade", post.FogFade());
    postShader.SendUniform("fogColor", post.FogColour());
    postShader.SendUniform("minimumColor", post.MinColour());
    postShader.SendUniform("maximumColor", post.MaxColour());
    postShader.SendUniform("bloomIntensity", post.BloomIntensity());
    postShader.SendUniform("finalMask", post.Mask(PostProcessing::FINAL_MAP));
    postShader.SendUniform("sceneMask", post.Mask(PostProcessing::SCENE_MAP));
    postShader.SendUniform("normalMask", post.Mask(PostProcessing::NORMAL_MAP));
    postShader.SendUniform("depthMask", post.Mask(PostProcessing::DEPTH_MAP));
    postShader.SendUniform("blurSceneMask", post.Mask(PostProcessing::BLUR_MAP));
    postShader.SendUniform("depthOfFieldMask", post.Mask(PostProcessing::DOF_MAP));
    postShader.SendUniform("fogMask", post.Mask(PostProcessing::FOG_MAP));
    postShader.SendUniform("bloomMask", post.Mask(PostProcessing::BLOOM_MAP));

    postShader.SendTexture(0, *m_effectsTarget, RenderTarget::SCENE_ID);
    postShader.SendTexture(1, *m_blurTarget, RenderTarget::BLUR_ID);
    postShader.SendTexture(2, *m_sceneTarget, RenderTarget::NORMAL_ID);

    m_backBuffer->SetActive();
    m_screenQuad->PreRender();
    postShader.EnableShader();
    m_screenQuad->Render();

    postShader.ClearTexture(0, *m_effectsTarget);
    postShader.ClearTexture(1, *m_blurTarget);
    postShader.ClearTexture(2, *m_sceneTarget);
}

void OpenGL::RenderBlur()
{
    EnableAlphaBlending(false);
    EnableBackfaceCull(false);

    m_blurTarget->SetActive();
    m_blurTarget->SwitchTextures();
    const auto& post = m_scene.Post();
    
    SetSelectedShader(Shader::ID_BLUR_HORIZONTAL);
    auto& blurHorizontalShader = m_scene.GetShader(Shader::ID_BLUR_HORIZONTAL);

    blurHorizontalShader.SendUniform("blurStep", post.BlurStep());
    blurHorizontalShader.SendTexture(0, *m_effectsTarget);

    m_screenQuad->PreRender();
    blurHorizontalShader.EnableShader();
    m_screenQuad->Render();

    blurHorizontalShader.ClearTexture(0, *m_effectsTarget);

    SetSelectedShader(Shader::ID_BLUR_VERTICAL);
    auto& blurVerticalShader = m_scene.GetShader(Shader::ID_BLUR_VERTICAL);

    blurVerticalShader.SendUniform("blurStep", post.BlurStep());
    blurVerticalShader.SendTexture(0, *m_blurTarget);

    m_blurTarget->SwitchTextures();
    m_screenQuad->PreRender();
    blurVerticalShader.EnableShader();
    m_screenQuad->Render();

    blurVerticalShader.ClearTexture(0, *m_blurTarget);
}

void OpenGL::RenderPreEffects()
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false);

    SetSelectedShader(Shader::ID_PRE_PROCESSING);
    auto& preShader = m_scene.GetShader(Shader::ID_PRE_PROCESSING);
    const auto& post = m_scene.Post();
    
    preShader.SendUniform("bloomStart", post.BloomStart());
    preShader.SendUniform("bloomFade", post.BloomFade());

    preShader.SendTexture(0, *m_sceneTarget, RenderTarget::SCENE_ID);
    
    m_effectsTarget->SetActive();
    m_screenQuad->PreRender();
    preShader.EnableShader();
    m_screenQuad->Render();

    preShader.ClearTexture(0, *m_sceneTarget);
}

void OpenGL::UpdateShader(const glm::mat4& world)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    shader.SendUniform("world", world);
}

bool OpenGL::UpdateShader(const MeshData& mesh, bool alphaBlend, float timer)
{
    const int index = mesh.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = m_scene.GetShader(index);
        if(index != m_selectedShader)
        {
            SetSelectedShader(index);
            SendLights();

            shader.SendUniform("viewProjection", m_camera.ViewProjection());
            shader.SendUniform("cameraPosition", m_camera.Position());
            shader.SendUniform("depthNear", m_scene.Post().DepthNear());
            shader.SendUniform("depthFar", m_scene.Post().DepthFar());

            if (index == Shader::ID_WATER)
            {
                shader.SendUniform("timer", timer);
            }
        }

        SendTextures(mesh.TextureIDs());
        EnableBackfaceCull(mesh.BackfaceCull());
        EnableAlphaBlending(alphaBlend);
        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Mesh& mesh)
{
    if (UpdateShader(mesh, false))
    {
        auto& shader = m_scene.GetShader(mesh.ShaderID());
        shader.SendUniform("meshCausticAmount", mesh.CausticsAmount());
        shader.SendUniform("meshCausticScale", mesh.CausticsScale());
        shader.SendUniform("meshAmbience", mesh.Ambience());
        shader.SendUniform("meshBump", mesh.Bump());
        shader.SendUniform("meshSpecularity", mesh.Specularity());
        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Terrain& terrain)
{
    if (UpdateShader(terrain, false))
    {
        auto& shader = m_scene.GetShader(terrain.ShaderID());
        shader.SendUniform("meshCausticAmount", terrain.CausticsAmount());
        shader.SendUniform("meshCausticScale", terrain.CausticsScale());
        shader.SendUniform("meshAmbience", terrain.Ambience());
        shader.SendUniform("meshBump", terrain.Bump());
        shader.SendUniform("meshSpecularity", terrain.Specularity());
        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Water& water, float timer)
{
    if (UpdateShader(water, true, timer))
    {
        auto& shader = m_scene.GetShader(water.ShaderID());
        shader.SendUniform("speed", water.Speed());
        shader.SendUniform("bumpIntensity", water.Bump());
        shader.SendUniform("bumpVelocity", water.BumpVelocity());
        shader.SendUniform("uvScale", water.UVScale());
        shader.SendUniform("deepColor", water.Deep());
        shader.SendUniform("shallowColor", water.Shallow());
        shader.SendUniform("reflectionTint", water.ReflectionTint());
        shader.SendUniform("reflectionIntensity", water.ReflectionIntensity());
        shader.SendUniform("fresnal", water.Fresnal());

        const auto& waves = water.Waves();
        for (unsigned int i = 0; i < waves.size(); ++i)
        {
            shader.SendUniform("waveFrequency", waves[i].amplitude, i);
            shader.SendUniform("waveAmplitude", waves[i].amplitude, i);
            shader.SendUniform("wavePhase", waves[i].phase, i);
            shader.SendUniform("waveDirectionX", waves[i].directionX, i);
            shader.SendUniform("waveDirectionZ", waves[i].directionZ, i);
        }

        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Emitter& emitter)
{
    const int index = emitter.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = m_scene.GetShader(index);
        if (index != m_selectedShader)
        {
            SetSelectedShader(index);
        }

        shader.SendUniform("tint", emitter.Tint());

        EnableBackfaceCull(false);
        EnableAlphaBlending(true);
        return true;
    }
    return false;
}

void OpenGL::UpdateShader(const glm::mat4& world, const Particle& particle)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    shader.SendUniform("worldViewProjection", m_camera.ViewProjection() * world);
    shader.SendUniform("alpha", particle.Alpha());
    SendTexture(0, particle.Texture());
}

void OpenGL::SendLights()
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    const auto& lights = m_scene.Lights();

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        const int offset = i*3; // Arrays pack tightly
        shader.SendUniform("lightSpecularity", lights[i]->Specularity(), i);
        shader.SendUniform("lightActive", lights[i]->Active(), i);
        shader.SendUniform("lightAttenuation", lights[i]->Attenuation(), offset);
        shader.SendUniform("lightPosition", lights[i]->Position(), offset);
        shader.SendUniform("lightDiffuse", lights[i]->Diffuse(), offset);
        shader.SendUniform("lightSpecular", lights[i]->Specular(), offset);
    }
}

void OpenGL::SendTextures(const std::vector<int>& textures)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    for (unsigned int i = 0, slot = 0; i < textures.size(); ++i)
    {
        if (SendTexture(slot, textures[i]))
        {
            ++slot;
        }
    }
}

bool OpenGL::SendTexture(int slot, int ID)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    if (ID != NO_INDEX && shader.HasTextureSlot(slot))
    {
        const auto& texture = m_scene.Textures()[ID];
        shader.SendTexture(slot, texture->GetID(), texture->IsCubeMap());
        return true;
    }
    return false;
}

void OpenGL::EnableAlphaBlending(bool enable)
{
    if (enable != m_isAlphaBlend)
    {
        m_isAlphaBlend = enable;
        enable ? glEnablei(GL_BLEND, 0) : glDisablei(GL_BLEND, 0);
    }
}

void OpenGL::EnableBackfaceCull(bool enable)
{
    if(enable != m_isBackfaceCull)
    {
        m_isBackfaceCull = enable;
        enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }
}

void OpenGL::EnableDepthWrite(bool enable)
{
    if (enable != m_isDepthWrite)
    {
        m_isDepthWrite = enable;
        enable ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
    }
}

void OpenGL::EnableSelectedShader()
{
    m_scene.GetShader(m_selectedShader).EnableShader();
}

void OpenGL::SetSelectedShader(int index)
{
    m_selectedShader = index;
    m_scene.GetShader(m_selectedShader).SetActive();
}

void OpenGL::ToggleWireframe()
{
    m_isWireframe = !m_isWireframe;

    LogInfo("OpenGL: Wireframe " + 
        std::string(m_isWireframe ? "on" : "off"));
}