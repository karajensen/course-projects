////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameFader.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;

/// <summary>
/// Cross-scene fading to black
/// Note: Must be on top of all other sprites but not on GUI components
/// </summary>
public class SceneFader : MonoBehaviour 
{
    static GameObject sm_fadeSprite = null;            // Image to fade in/out
    static FadeState sm_fadeState = FadeState.NO_FADE; // State of fading
    static float sm_alpha = 0.0f;                      // Alpha for the fade sprite
    float m_fadeSpeed = 0.75f;                         // Speed for fading

    /// <summary>
    /// Initialises the fader
    /// </summary>
    void Start () 
    {
        if(sm_fadeSprite == null)
        {
            sm_fadeSprite = Instantiate(this.gameObject);
            sm_fadeSprite.transform.SetParent (this.transform);
            GetSprite().enabled = true;
            GetSprite().color = new Color(0.0f, 0.0f, 0.0f, sm_alpha);
        }
    }

    /// <summary>
    /// Returns the UI image for the fader
    /// </summary>
    UnityEngine.UI.Image GetSprite()
    {
        return sm_fadeSprite.GetComponent<UnityEngine.UI.Image> ();
    }

    /// <summary>
    /// Fades into black
    /// </summary>
    public void FadeIn()
    {
        sm_fadeState = FadeState.FADE_IN;
    }

    /// <summary>
    /// Fades out of black
    /// </summary>
    public void FadeOut()
    {
        sm_fadeState = FadeState.FADE_OUT;
    }

    /// <summary>
    /// Whether the scene is fully visible
    /// </summary>
    public bool IsFadedOut()
    {
        return GetSprite().color.a <= 0.0f;
    }

    /// <summary>
    /// Whether the scene is fully occluded
    /// </summary>
    public bool IsFadedIn()
    {
        return GetSprite().color.a >= 1.0f;
    }

    /// <summary>
    /// Fades the sprite in/out
    /// </summary>
    void Update () 
    {
        if(sm_fadeState != FadeState.NO_FADE)
        {
            bool fadeIn = sm_fadeState == FadeState.FADE_IN;

            sm_alpha += Time.deltaTime * (fadeIn ? m_fadeSpeed : -m_fadeSpeed);
            sm_alpha = Mathf.Min(Mathf.Max(0.0f, sm_alpha), 1.0f);

            if((fadeIn && sm_alpha >= 1.0f) || (!fadeIn && sm_alpha <= 0.0f))
            {
                sm_fadeState = FadeState.NO_FADE;
            }

            GetSprite().color = new Color(0.0f, 0.0f, 0.0f, sm_alpha);
        }
    }
    
    /// <summary>
    /// Gets the SceneFader from the scene
    /// </summary>
    public static SceneFader Get()
    {
        var obj = FindObjectOfType<SceneFader>();
        if (!obj)
        {
            Debug.LogError("SceneFader could not be found in scene");
        }
        return obj;
    }
}
