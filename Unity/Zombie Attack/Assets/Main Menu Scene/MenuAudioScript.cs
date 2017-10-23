using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuAudioScript : MonoBehaviour {

	public AudioSource menuBackgroundMusicSource;
	public AudioSource click;

	// Use this for initialization
	void Start () {
		menuBackgroundMusicSource.Play ();
	}

	// Update is called once per frame
	void Update () {

	}

	public void playClickNoise()
	{
		click.Play ();
	}
}
