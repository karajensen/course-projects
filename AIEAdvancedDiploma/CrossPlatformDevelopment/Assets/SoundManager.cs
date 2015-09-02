﻿////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SoundManager.cs
////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/// <summary>
/// Cross-scene management of sound and music 
/// </summary>
public class SoundManager : MonoBehaviour 
{
	/// <summary>
    /// References to be added to the manager
    /// </summary>
	public AudioSource menuMusic = null;
	public AudioSource gameMusic = null;
	public AudioSource buttonClick = null;
	public AudioSource hitSound = null;
	public AudioSource shootSound = null;
	public AudioSource explodeSound = null;
	public AudioSource ramSound = null;
	
	/// <summary>
    /// Avaliable music IDs
    /// </summary>
	public enum MusicID
	{
		GAME_TRACK,
		MENU_TRACK,
		MAX_MUSIC
	};
	
	/// <summary>
    /// Avaliable sound effect IDs
    /// </summary>
	public enum SoundID
	{
		BUTTON_CLICK,
		HIT,
		SHOOT,
		EXPLODE,
		RAM,
		MAX_SOUNDS
	};
	
	/// <summary>
    /// Information for music shared across scenes
    /// </summary>
	class SharedMusic
	{
		public AudioSource sound = null;                 /// Sound file
		public float maxVolume = 0.0f;                   /// Maximum volume the sound can reach
		public FadeState fadeState = FadeState.NO_FADE;  /// State for fading in/ou
	};
	
	/// <summary>
    /// Information for a sound shared across scenes
    /// </summary>
	class SharedSound
	{
		public bool shouldPlaySound = false;         /// Whether the sound should play this tick
		public List<AudioSource> instances = null;   /// Sound file
		public string name;
	};
	
	private static SoundManager sm_soundManager = null; /// Instance of the sound manager
	private static GameObject sm_soundParent = null;    /// Parent to hold all sound
	private static List<SharedMusic> sm_sharedMusic;    /// Shared music across scenes
	private static List<SharedSound> sm_sharedSound;    /// Shared sounds across scenes
	private static bool sm_initialised = false;         /// Whether the manager is initialised
	private float m_fadeSpeed = 0.75f;                  /// Speed to fade the music
	private float m_overallMusicMultiplier = 0.5f;
	private float m_overallSoundMultiplier = 0.65f;
	
	/// <summary>
    /// Initialises the manager if needed
    /// </summary>
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
			
			CreateSound(SoundID.EXPLODE, explodeSound, 5, "Explode");
			CreateSound(SoundID.SHOOT, shootSound, 20, "Shoot");
			CreateSound(SoundID.HIT, hitSound, 20, "Hit");
			CreateSound(SoundID.RAM, ramSound, 5, "Ram");
			CreateSound(SoundID.BUTTON_CLICK, buttonClick, 1, "Button");
			CreateMusic(MusicID.GAME_TRACK, gameMusic);
			CreateMusic(MusicID.MENU_TRACK, menuMusic);
		}
	}
	
	/// <summary>
    /// Creates and adds a new sound effect
    /// </summary>
	void CreateSound(SoundID ID, AudioSource source, int instances, string name)
	{
		int index = (int)ID;
		float volume = source.volume * m_overallSoundMultiplier;
		sm_sharedSound[index].instances = new List<AudioSource>();
		sm_sharedSound[index].name = name;
		
		for(int i = 0; i < instances; ++i)
		{
			sm_sharedSound[index].instances.Add(
				i == 0 ? source : Instantiate(source));
			
			sm_sharedSound[index].instances[i].transform.parent = sm_soundParent.transform;
			sm_sharedSound[index].instances[i].Stop();
			sm_sharedSound[index].instances[i].volume = volume;
			DontDestroyOnLoad(sm_sharedSound[index].instances[i]);
		}
	}
	
	/// <summary>
    /// Creates and adds a new music track
    /// </summary>
	void CreateMusic(MusicID ID, AudioSource source)
	{
		int index = (int)ID;
		sm_sharedMusic[index].sound = Instantiate(source);
		sm_sharedMusic[index].maxVolume = sm_sharedMusic[index].sound.volume * m_overallMusicMultiplier;
		sm_sharedMusic[index].sound.volume = 0.0f;
		sm_sharedMusic[index].sound.transform.parent = sm_soundParent.transform;
		sm_sharedMusic[index].sound.Stop();
		DontDestroyOnLoad(sm_sharedMusic[index].sound);
	}
	
	/// <summary>
    /// Fades the music in/out
    /// </summary>
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
					0.0f, music.sound.volume), music.maxVolume);
				
				if(fadeIn && music.sound.volume >= music.maxVolume)
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
	
	/// <summary>
    /// Plays sound effects
    /// Post tick required as effect can be requested in any order
    /// </summary>
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
					Debug.LogError("Could not find instance for sound " + sound.name);
				}
			}
		}
	}
	
	/// <summary>
    /// Plays a music track
    /// </summary>
	public void PlayMusic(MusicID ID)
	{
		if(!sm_sharedMusic[(int)ID].sound.isPlaying)
		{
			sm_sharedMusic[(int)ID].fadeState = FadeState.FADE_IN;
			sm_sharedMusic[(int)ID].sound.Play();
		}
	}
	
	/// <summary>
    /// Stops a music track
    /// </summary>
	public void StopMusic(MusicID ID)
	{
		sm_sharedMusic[(int)ID].fadeState = FadeState.FADE_OUT;
	}
	
	/// <summary>
    /// Plays a sound effect
    /// </summary>
	public void PlaySound(SoundID ID)
	{
		sm_sharedSound[(int)ID].shouldPlaySound = true;
	}
	
	/// <summary>
    /// Gets the SoundManager from the scene
    /// </summary>
	public static SoundManager Get()
	{
		if(sm_soundManager == null)
		{
			sm_soundManager = FindObjectOfType<SoundManager>();
			if(sm_soundManager == null)
			{
				Debug.LogError("Could not find SoundManager");
			}
		}
		return sm_soundManager;
	}
}
