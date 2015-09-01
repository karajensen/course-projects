////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Utilities.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;

/**
* Fade state
*/
enum FadeState
{
    FADE_IN,
    FADE_OUT,
    NO_FADE
};

/**
* Scene IDS
*/
enum SceneID
{
	GAME = 0,
	MENU = 1
}

class Utilities
{
    /**
    * Returns the game version
    */
    static public string GameVersion()
    {
        return "0.1";
    }

    /**
    * Returns whether a level is currently loaded
    */
    static public bool IsGameLoaded()
    {
		return Application.loadedLevel == (int)SceneID.GAME;
    }
}