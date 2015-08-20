﻿////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SoundManager.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/**
* Cross-scene management of sound and music 
*/
public class SoundManager : MonoBehaviour 
{
    /**
    * References to be added to the manager
    */
    public AudioSource menuMusic = null;
    public AudioSource gameMusic = null;
    public AudioSource buttonClick = null;

    /**
    * Avaliable music IDs
    */
    public enum MusicID
    {
        GAME_TRACK,
        MENU_TRACK,
        MAX_MUSIC
    };
    
    /**
    * Avaliable sound effect IDs
    */
    public enum SoundID
    {
        BUTTON_CLICK,
        MAX_SOUNDS
    };

    /**
    * Information for music shared across scenes
    */
    class SharedMusic
    {
        public AudioSource sound = null;                 /// Sound file
        public float maxVolume = 0.0f;                   /// Maximum volume the sound can reach
        public FadeState fadeState = FadeState.NO_FADE;  /// State for fading in/ou
    };

    /**
    * Information for a sound shared across scenes
    */
    class SharedSound
    {
        public bool shouldPlaySound = false;         /// Whether the sound should play this tick
        public List<AudioSource> instances = null;   /// Sound file
    };
   
    private static GameObject sm_soundParent = null; /// Parent to hold all sound
    private static List<SharedMusic> sm_sharedMusic; /// Shared music across scenes
    private static List<SharedSound> sm_sharedSound; /// Shared sounds across scenes
    private static bool sm_initialised = false;      /// Whether the manager is initialised
    private float m_fadeSpeed = 0.75f;               /// Speed to fade the music

    /**
    * Initialises the manager if needed
    */
	void Start () 
    {
        if(!sm_initialised)
        {
            sm_initialised = true;
            sm_sharedMusic = new List<SharedMusic>();
            sm_sharedSound = new List<SharedSound>();
            sm_soundParent = new GameObject();

            sm_soundParent.name = "SharedSounds";
            DontDestroyOnLoad(sm_soundParent);

            for(int i = 0; i < (int)MusicID.MAX_MUSIC; ++i)
            {
                sm_sharedMusic.Add(new SharedMusic());
            }

            for(int i = 0; i < (int)SoundID.MAX_SOUNDS; ++i)
            {
                sm_sharedSound.Add(new SharedSound());;
            }

            CreateSound(SoundID.BUTTON_CLICK, buttonClick, 1);
            CreateMusic(MusicID.GAME_TRACK, gameMusic);
            CreateMusic(MusicID.MENU_TRACK, menuMusic);
        }
	}

    /**
    * Creates and adds a new sound effect
    */
    void CreateSound(SoundID ID, AudioSource source, int instances)
    {
        int index = (int)ID;
        sm_sharedSound[index].instances = new List<AudioSource>();

        for(int i = 0; i < instances; ++i)
        {
            sm_sharedSound[index].instances.Add(Instantiate(source));
            sm_sharedSound[index].instances[i].transform.parent = sm_soundParent.transform;
            sm_sharedSound[index].instances[i].Stop();
            DontDestroyOnLoad(sm_sharedSound[index].instances[i]);
        }
    }

    /**
    * Creates and adds a new music track
    */
    void CreateMusic(MusicID ID, AudioSource source)
    {
        int index = (int)ID;
        sm_sharedMusic[index].sound = Instantiate(source);
        sm_sharedMusic[index].maxVolume = sm_sharedMusic[index].sound.volume;
        sm_sharedMusic[index].sound.volume = 0.0f;
        sm_sharedMusic[index].sound.transform.parent = sm_soundParent.transform;
        sm_sharedMusic[index].sound.Stop();
        DontDestroyOnLoad(sm_sharedMusic[index].sound);
    }
	
    /**
    * Fades the music in/out
    */
	void Update () 
    {
        foreach(var music in sm_sharedMusic)
        {
            if(music.fadeState != FadeState.NO_FADE)
            {
                bool fadeIn = music.fadeState == FadeState.FADE_IN;

                music.sound.volume += Time.deltaTime * 
                    (fadeIn ? m_fadeSpeed : -m_fadeSpeed);

                music.sound.volume = Mathf.Min(Mathf.Max(
                    0.0f, music.sound.volume), 1.0f);

                if(fadeIn && music.sound.volume >= 1.0f)
                {
                    music.fadeState = FadeState.NO_FADE;
                }
                else if(!fadeIn && music.sound.volume <= 0.0f)
                {
                    music.fadeState = FadeState.NO_FADE;
                    music.sound.Stop();
                }
            }
        }
	}

    /**
    * Plays sound effects
    * Post tick required as effect can be requested in any order
    */
    void LateUpdate()
    {
        foreach(var sound in sm_sharedSound)
        {
            if(sound.shouldPlaySound)
            {
                sound.shouldPlaySound = false;
                bool foundInstance = false;

                foreach (var instance in sound.instances)
                {
                    if(!instance.isPlaying)
                    {
                        instance.Play();
                        foundInstance = true;
                        break;
                    }
                }

                if(!foundInstance)
                {
                    Debug.LogError("Could not find instance for sound");
                }
            }
        }
    }

    /**
    * Plays a music track
    */
    public void PlayMusic(MusicID ID)
    {
        sm_sharedMusic[(int)ID].fadeState = FadeState.FADE_IN;
        sm_sharedMusic[(int)ID].sound.Play();
    }

    /**
    * Stops a music track
    */
    public void StopMusic(MusicID ID)
    {
        sm_sharedMusic[(int)ID].fadeState = FadeState.FADE_OUT;
    }

    /**
    * Plays a sound effect
    */
    public void PlaySound(SoundID ID)
    {
        sm_sharedSound[(int)ID].shouldPlaySound = true;
    }

    /**
    * Gets the SoundManager from the scene
    */
    public static SoundManager Get()
    {
        var obj = FindObjectOfType<SoundManager>();
        if (!obj)
        {
            Debug.LogError("SoundManager could not be found in scene");
        }
        return obj;
    }
}
