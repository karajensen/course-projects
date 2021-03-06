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
#include "timer.h"

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
    m_blurTarget.reset();
    m_effectsTarget.reset();

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

    glClearColor(0.22f, 0.49f, 0.85f, 0.0f);
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

    m_sceneTarget->SetHighQuality(RenderTarget::DEPTH_ID); // Require for DOF

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

void OpenGL::RenderScene(Timer& timer, float timePassed)
{
    timer.StartSection(Timer::RENDER_SCENE);
    RenderSceneMap(timer, timePassed);
    timer.StopSection(Timer::RENDER_SCENE);

    timer.StartSection(Timer::RENDER_EFFECTS);
    RenderPreEffects();
    timer.StopSection(Timer::RENDER_EFFECTS);

    timer.StartSection(Timer::RENDER_BLUR);
    RenderBlur();
    timer.StopSection(Timer::RENDER_BLUR);

    timer.StartSection(Timer::RENDER_POST);
    RenderPostProcessing();
    timer.StopSection(Timer::RENDER_POST);
}

void OpenGL::EndRender()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void OpenGL::RenderSceneMap(Timer& timer, float timePassed)
{
    m_sceneTarget->SetActive();

    if (m_isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    }

    timer.StartSection(Timer::RENDER_TERRAIN);
    RenderTerrain();
    timer.StopSection(Timer::RENDER_TERRAIN);

    timer.StartSection(Timer::RENDER_SHADOWS);
    RenderShadows();
    timer.StopSection(Timer::RENDER_SHADOWS);

    timer.StartSection(Timer::RENDER_MESHES);
    RenderMeshes();
    timer.StopSection(Timer::RENDER_MESHES);

    timer.StartSection(Timer::RENDER_WATER);
    RenderWater(timePassed);
    timer.StopSection(Timer::RENDER_WATER);

    timer.StartSection(Timer::RENDER_EMITTERS);
    RenderEmitters();
    timer.StopSection(Timer::RENDER_EMITTERS);

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
            mesh->Render([this](const glm::mat4& world, int texture)
            { 
                UpdateShader(world, texture); 
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
            terrain->Render([this](const glm::mat4& world, int texture)
            { 
                UpdateShader(world, texture); 
            });
        }
    }
}

void OpenGL::RenderWater(float timePassed)
{
    for (const auto& water : m_scene.Waters())
    {
        if (UpdateShader(*water, timePassed))
        {
            water->PreRender();
            EnableSelectedShader();
            water->Render([this](const glm::mat4& world, int texture)
            { 
                UpdateShader(world, texture); 
            });
        }
    }
}

void OpenGL::RenderShadows()
{
	EnableDepthWrite(false);

    auto& shadows = *m_scene.Shadows();
    if (UpdateShader(shadows))
    {
        shadows.PreRender();
        EnableSelectedShader();
        shadows.Render([this](const glm::mat4& world, int texture)
        { 
            UpdateShader(world, texture);
        });
    }

	EnableDepthWrite(true);
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
    EnableAlphaBlending(false, false);
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
    postShader.SendUniform("depthMask", post.Mask(PostProcessing::DEPTH_MAP));
    postShader.SendUniform("blurSceneMask", post.Mask(PostProcessing::BLUR_MAP));
    postShader.SendUniform("depthOfFieldMask", post.Mask(PostProcessing::DOF_MAP));
    postShader.SendUniform("fogMask", post.Mask(PostProcessing::FOG_MAP));
    postShader.SendUniform("bloomMask", post.Mask(PostProcessing::BLOOM_MAP));

    postShader.SendTexture("SceneSampler", *m_effectsTarget, RenderTarget::SCENE_ID);
    postShader.SendTexture("BlurSampler", *m_blurTarget, RenderTarget::BLUR_ID);
    postShader.SendTexture("DepthSampler", *m_sceneTarget, RenderTarget::DEPTH_ID);

    m_backBuffer->SetActive();
    m_screenQuad->PreRender();
    postShader.EnableShader();
    m_screenQuad->Render();

    postShader.ClearTexture("SceneSampler", *m_effectsTarget);
    postShader.ClearTexture("BlurSampler", *m_blurTarget);
    postShader.ClearTexture("DepthSampler", *m_sceneTarget);
}

void OpenGL::RenderBlur()
{
    EnableAlphaBlending(false, false);
    EnableBackfaceCull(false);

    m_blurTarget->SetActive();
    m_blurTarget->SwitchTextures();
    const auto& post = m_scene.Post();
    
    SetSelectedShader(Shader::ID_BLUR_HORIZONTAL);
    auto& blurHorizontalShader = m_scene.GetShader(Shader::ID_BLUR_HORIZONTAL);

    blurHorizontalShader.SendUniform("blurStep", post.BlurStep());
    blurHorizontalShader.SendTexture("SceneSampler", *m_effectsTarget);

    m_screenQuad->PreRender();
    blurHorizontalShader.EnableShader();
    m_screenQuad->Render();

    blurHorizontalShader.ClearTexture("SceneSampler", *m_effectsTarget);

    SetSelectedShader(Shader::ID_BLUR_VERTICAL);
    auto& blurVerticalShader = m_scene.GetShader(Shader::ID_BLUR_VERTICAL);

    blurVerticalShader.SendUniform("blurStep", post.BlurStep());
    blurVerticalShader.SendTexture("SceneSampler", *m_blurTarget);

    m_blurTarget->SwitchTextures();
    m_screenQuad->PreRender();
    blurVerticalShader.EnableShader();
    m_screenQuad->Render();

    blurVerticalShader.ClearTexture("SceneSampler", *m_blurTarget);
}

void OpenGL::RenderPreEffects()
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false, false);

    SetSelectedShader(Shader::ID_PRE_PROCESSING);
    auto& preShader = m_scene.GetShader(Shader::ID_PRE_PROCESSING);
    const auto& post = m_scene.Post();
    
    preShader.SendUniform("bloomStart", post.BloomStart());
    preShader.SendUniform("bloomFade", post.BloomFade());

    preShader.SendTexture("SceneSampler", *m_sceneTarget, RenderTarget::SCENE_ID);
    
    m_effectsTarget->SetActive();
    m_screenQuad->PreRender();
    preShader.EnableShader();
    m_screenQuad->Render();

    preShader.ClearTexture("SceneSampler", *m_sceneTarget);
}

bool OpenGL::UpdateShader(const Quad& quad)
{
    const int index = quad.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = m_scene.GetShader(index);
        if(index != m_selectedShader)
        {
            SetSelectedShader(index);
            shader.SendUniform("viewProjection", m_camera.ViewProjection());
        }

        EnableBackfaceCull(false);
        EnableAlphaBlending(true, true);
        return true;
    }
    return false;
}

void OpenGL::UpdateShader(const glm::mat4& world, int texture)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    shader.SendUniform("world", world);
    SendTexture("DiffuseSampler", texture);
}

bool OpenGL::UpdateShader(const MeshData& mesh, bool alphaBlend, float timePassed)
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
                shader.SendUniform("timer", timePassed);
            }
        }

        SendTextures(mesh.TextureIDs());
        EnableBackfaceCull(mesh.BackfaceCull());
        EnableAlphaBlending(alphaBlend, false);
        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Mesh& mesh)
{
    if (UpdateShader(mesh, false))
    {
        SendAttributes(mesh);
        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Terrain& terrain)
{
    if (UpdateShader(terrain, false))
    {
        SendAttributes(terrain);
        return true;
    }
    return false;
}

bool OpenGL::UpdateShader(const Water& water, float timePassed)
{
    if (UpdateShader(water, true, timePassed))
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
            shader.SendUniform("waveAmplitude", waves[i].frequency, i);
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
            shader.SendUniform("depthNear", m_scene.Post().DepthNear());
            shader.SendUniform("depthFar", m_scene.Post().DepthFar());
        }

        shader.SendUniform("tint", emitter.Tint());

        EnableBackfaceCull(false);
        EnableAlphaBlending(true, false);
        return true;
    }
    return false;
}

void OpenGL::UpdateShader(const glm::mat4& world, const Particle& particle)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    shader.SendUniform("worldViewProjection", m_camera.ViewProjection() * world);
    shader.SendUniform("alpha", particle.Alpha());
    SendTexture("DiffuseSampler", particle.Texture());
}

void OpenGL::SendAttributes(const MeshAttributes& attributes)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    shader.SendUniform("meshCausticAmount", attributes.CausticsAmount());
    shader.SendUniform("meshCausticScale", attributes.CausticsScale());
    shader.SendUniform("meshAmbience", attributes.Ambience());
    shader.SendUniform("meshBump", attributes.Bump());
    shader.SendUniform("meshSpecularity", attributes.Specularity());
    shader.SendUniform("meshSpecular", attributes.Specular());
    shader.SendUniform("meshDiffuse", attributes.Diffuse());
}

void OpenGL::SendLights()
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    const auto& lights = m_scene.Lights();

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        const int offset = i * 3;
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
    SendTexture("NormalSampler", textures[MeshData::NORMAL]);
    SendTexture("SpecularSampler", textures[MeshData::SPECULAR]);
    SendTexture("EnvironmentSampler", textures[MeshData::ENVIRONMENT]);
    SendTexture("CausticsSampler", textures[MeshData::CAUSTICS]);
}

bool OpenGL::SendTexture(const std::string& sampler, int ID)
{
    auto& shader = m_scene.GetShader(m_selectedShader);
    if (ID != NO_INDEX)
    {
        const auto& texture = m_scene.Textures()[ID];
        shader.SendTexture(sampler, texture->GetID(), texture->IsCubeMap());
        return true;
    }
    return false;
}

void OpenGL::EnableAlphaBlending(bool enable, bool multiply)
{
    if (enable != m_isAlphaBlend)
    {
        m_isAlphaBlend = enable;
        enable ? glEnablei(GL_BLEND, 0) : glDisablei(GL_BLEND, 0);
        enable ? glEnablei(GL_BLEND, 1) : glDisablei(GL_BLEND, 1);
    }
    if (multiply != m_isBlendMultiply)
    {
        m_isBlendMultiply = multiply;
        if (multiply)
        {
            glBlendFuncSeparate(GL_DST_COLOR, GL_ZERO, GL_DST_ALPHA, GL_ZERO);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        }
        else
        {
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        }
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