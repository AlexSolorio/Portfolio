using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioScript : MonoBehaviour {

	public AudioSource reloadSource;
	public AudioSource shotFiredSource;
	public AudioSource clipEmptySource;
	public AudioSource zombieMoan1Source;
	public AudioSource zombieMoan2Source;
	public AudioSource pickUpAmmoSource;
	public AudioSource backgroundMusicSource;

	// Use this for initialization
	void Start () {
		backgroundMusicSource.Play ();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void playReloadSound()
	{
		reloadSource.Play ();
	}

	public void playShotFiredSound()
	{
		shotFiredSource.Play ();
	}

	public void playClipEmptySound()
	{
		clipEmptySource.Play ();
	}

	public void playPickUpAmmoSound()
	{
		pickUpAmmoSource.Play ();
	}

	public void playZombieMoanSound()
	{
		if (Random.Range (0.0f, 1.0f) > 0.5f)
			zombieMoan1Source.Play ();
		else
			zombieMoan2Source.Play ();
	}
}
