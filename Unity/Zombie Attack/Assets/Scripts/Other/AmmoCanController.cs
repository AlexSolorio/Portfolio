using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoCanController : MonoBehaviour {

	public AmmoCanSpawner ammoCanSpawner;
	public bulletGenerator bulletGenerator;
	public AudioScript audioScript;

	// Use this for initialization
	void Start () {
		ammoCanSpawner = GameObject.FindGameObjectWithTag ("Ammo Can Controller").GetComponent<AmmoCanSpawner> ();
		bulletGenerator = GameObject.FindGameObjectWithTag ("Player").GetComponent<bulletGenerator> ();
		audioScript = GameObject.FindGameObjectWithTag ("Audio Controller").GetComponent<AudioScript> ();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void OnTriggerEnter2D (Collider2D coll)
	{
		GameObject thingHit = coll.gameObject;
		if (thingHit.tag == "Player")
		{
			audioScript.playPickUpAmmoSound ();
			bulletGenerator.bulletsOutOfClip += 15;
			ammoCanSpawner.numberOfAmmoCans--;
			Destroy (this.gameObject);
		}
	}
		
}
