﻿////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - MenuLoader.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;

/// <summary>
/// Initialises/exits the scene
/// </summary>
public class MenuLoader : MonoBehaviour 
{
    bool m_initialised = false;         // Whether the scene has been initialised
    bool m_exitRequest = false;         // Whether the scene should exit
    SceneFader m_sceneFader = null;     // Shared scene fader
    SoundManager m_soundManager = null; // Shared sound manager

    /// <summary>
    /// Updates the loader
    /// </summary>
    void Update()
    {
        if(!m_initialised)
        {
            m_sceneFader = SceneFader.Get();
            m_soundManager = SoundManager.Get();
            
            m_soundManager.PlayMusic(SoundManager.MusicID.MENU_TRACK);
            m_sceneFader.FadeOut();

            m_initialised = true;
        }

        if(m_exitRequest && m_sceneFader.IsFadedIn())
        {
            Application.LoadLevel((int)SceneID.GAME);
        }
    }

    /// <summary>
    /// Exits the scene
    /// </summary>
    public void ExitScene()
    {
        if(!m_exitRequest)
        {
            m_exitRequest = true;
            m_soundManager.StopMusic(SoundManager.MusicID.MENU_TRACK);
            m_sceneFader.FadeIn();
        }
    }

    /// <summary>
    /// Gets the Loader from the scene
    /// </summary>
    public static MenuLoader Get()
    {
        var obj = FindObjectOfType<MenuLoader>();
        if (!obj)
        {
            Debug.LogError("MenuLoader could not be found in scene");
        }
        return obj;
    }
}
