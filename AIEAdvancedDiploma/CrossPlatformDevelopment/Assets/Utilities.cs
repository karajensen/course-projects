////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Utilities.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;

/// <summary>
/// Fade state
/// </summary>
enum FadeState
{
    FADE_IN,
    FADE_OUT,
    NO_FADE
};

/// <summary>
/// Scene IDS
/// </summary>
enum SceneID
{
	GAME = 0,
	MENU = 1
}

class Utilities
{
	/// <summary>
    /// Gets the Network from the scene
    /// </summary>
	static public NetworkMatchmaker GetNetworkMatchmaker()
	{
		var network = GameObject.FindObjectOfType<NetworkMatchmaker>();
		if(network == null)
		{
			Debug.LogError("Could not find Network Matchmaker");
		}
		return network;
	}

    /// <summary>
    /// Returns the game version
    /// </summary>
    static public string GameVersion()
    {
        return "0.1";
    }

    /// <summary>
    /// Returns whether a level is currently loaded
    /// </summary>
    static public bool IsGameLoaded()
    {
		return Application.loadedLevel == (int)SceneID.GAME;
    }
}