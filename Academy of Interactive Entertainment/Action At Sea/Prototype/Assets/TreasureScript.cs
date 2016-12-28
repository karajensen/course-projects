using UnityEngine;
using System.Collections;

public class TreasureScript : MonoBehaviour {

	private SharedSoundHandler menuMusicHandler;
	
	void Start () 
	{
		menuMusicHandler = FindObjectOfType<SharedSoundHandler> ();
		if(!menuMusicHandler)
		{
			Debug.Log("MenuMusicHandler could not be found in scene.");
		}		
	}

	public void BackButton ()
	{
		menuMusicHandler.PlayButtonClick ();
		Application.LoadLevel (4);
	}
	public void BackToMenuButton ()
	{
		menuMusicHandler.PlayButtonClick ();
		Application.LoadLevel (0);
	}
}
