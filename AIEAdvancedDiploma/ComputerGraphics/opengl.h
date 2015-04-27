////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <memory>

struct GLFWwindow;
class IScene;
class Camera;
class Water;
class MeshAttributes;
class Mesh;
class Emitter;
class Particle;
class Quad;
class RenderTarget;
class MeshData;
class Terrain;
class Timer;

/**
* Engine for initialising and managing OpenGL
*/
class OpenGL
{
public:

    /**
    * Constructor
    * @param scene The scene to render
    * @param camera The viewable camera
    */
    OpenGL(const IScene& scene, const Camera& camera);

    /**
    * Destructor
    */
    ~OpenGL();

    /**
    * Initialise OpenGL
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

    /**
    * @return whether OpenGL is currently running
    */
    bool IsRunning() const;

    /**
    * Renders the scene
    * @param timer Allows logging time taken for rendering steps
    * @param timePassed The time passed since scene start
    */
    void RenderScene(Timer& timer, float timePassed);

    /**
    * Ends the rendering pipeline
    */
    void EndRender();

    /**
    * Toggles whether wireframe is active
    */
    void ToggleWireframe();

    /**
    * @return the application window
    */
    GLFWwindow& GetWindow() const;

private: 

    /**
    * Prevent copying
    */
    OpenGL(const OpenGL&) = delete;
    OpenGL& operator=(const OpenGL&) = delete;

    /**
    * Releases OpenGL
    */
    void Release();

    /**
    * Updates and switches to main shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param alphaBlend Whether to use alpha blending
    * @param timePassed The timePassed to send
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const MeshData& mesh, 
                      bool alphaBlend, 
                      float timePassed = 0.0f);

    /**
    * Updates and switches to main shader the mesh requires
    * @param mesh The mesh currently rendering
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Mesh& mesh);

    /**
    * Updates and switches to main shader the terrain requires
    * @param terrain The terrain currently rendering
    * @return whether the terrain can now be rendered
    */
    bool UpdateShader(const Terrain& terrain);

    /**
    * Updates the shader for a mesh per instance
    * @param world The world matrix for the mesh
    */
    void UpdateShader(const glm::mat4& world);

    /**
    * Updates and switches to main shader the water requires
    * @param water The water currently rendering
    * @param lights All lighting in the scene
    * @param timePassed The time passed since scene start
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Water& water, float timePassed);

    /**
    * Updates and switches to the shader for an emitter
    * @return whether the emitter can now be rendered
    */
    bool UpdateShader(const Emitter& emitter);

    /**
    * Updates the shader for a particle per instance
    * @param world The world matrix for the particle
    * @param particle The data for the particle
    */
    void UpdateShader(const glm::mat4& world, const Particle& particle);

    /**
    * Sets the shader at the given index as selected
    */
    void SetSelectedShader(int index);

    /**
    * Sends light information to the selected shader
    */
    void SendLights();

    /**
    * Sends any attributes for a mesh
    * @param attributes The attributes of the mesh currently rendering
    */
    void SendAttributes(const MeshAttributes& attributes);

    /**
    * Sends all textures to the selected shader
    */
    void SendTextures(const std::vector<int>& textures);

    /**
    * Sends the given texture to the selected shader
    * @param slot Which slot in the shader should it go in
    * @param ID The texture ID
    * @return whether sending was successful
    */
    bool SendTexture(int slot, int ID);

    /**
    * Renders the scene
    * @param timer Allows logging time taken for rendering steps
    * @param timePassed The time passed since scene start
    */
    void RenderSceneMap(Timer& timer, float timePassed);

    /**
    * Renders the scene with post processing
    */
    void RenderPostProcessing();

    /**
    * Renders the scene as blurred
    */
    void RenderBlur();

    /**
    * Renders the scene for preparing for post processing
    */
    void RenderPreEffects();

    /**
    * Renders all emitters
    */
    void RenderEmitters();

    /**
    * Renders all meshes
    */
    void RenderMeshes();

    /**
    * Renders all terrain
    */
    void RenderTerrain();

    /**
    * Renders all water
    * @param timePassed The time passed since scene start
    */
    void RenderWater(float timePassed);

    /**
    * Sets whether alpha blending is enabled or not
    */
    void EnableAlphaBlending(bool enable);

    /**
    * Sets whether values are written to the depth buffer or not
    * @note if set to false the depth buffer of the currently 
    *       selected render target till not clear
    */
    void EnableDepthWrite(bool enable);

    /**
    * Sets whether opengl should cull backfaces or not
    * @param enable whether to cull or not
    */
    void EnableBackfaceCull(bool enable);

    /**
    * Enables the selected shader for rendering
    */
    void EnableSelectedShader();

    GLFWwindow* m_window = nullptr;  ///< Handle to the application window
    const IScene& m_scene;           ///< The scene data to render
    const Camera& m_camera;          ///< The viewable camera
    bool m_isBackfaceCull = true;    ///< Whether the culling rasterize state is active
    bool m_isWireframe = false;      ///< Whether to render the scene as wireframe
    bool m_isAlphaBlend = false;     ///< Whether alpha blending is currently active
    bool m_isDepthWrite = true;      ///< Whether writing to the depth buffer is active
    int m_selectedShader = -1;       ///< Currently active shader for rendering
                                     
    std::unique_ptr<Quad> m_screenQuad;            ///< Screen space quad for effects
    std::unique_ptr<RenderTarget> m_backBuffer;    ///< Render target for the back buffer
    std::unique_ptr<RenderTarget> m_sceneTarget;   ///< Render target for the main scene
    std::unique_ptr<RenderTarget> m_effectsTarget; ///< Render target for pre-processing
    std::unique_ptr<RenderTarget> m_blurTarget;    ///< Render target for blurring
};