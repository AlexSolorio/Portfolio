using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bulletGenerator : MonoBehaviour {

	public GameObject bullet;
	public GameObject fireParticleSystem;

	public int bulletsInClip;
	public int bulletsOutOfClip;
	protected GameObject createdBullet;
	protected GameObject createdFireParticleSystem;

	public AudioScript audioScript;

	// Use this for initialization
	void Start () {
		bulletsInClip = 5;
		bulletsOutOfClip = 25;
	}

	// Update is called once per frame
	void Update () {
		
		//fire if possible
		if (Input.GetMouseButtonDown(0))
		{
			if (bulletsInClip > 0)
			{
				fireBullet ();
				bulletsInClip--; 
			}
			else
			{
				audioScript.playClipEmptySound ();
			}
		}

		//reload if possible
		if (Input.GetAxis("Reload") > 0 && bulletsInClip < 5 && bulletsOutOfClip > 0)
		{
			int bulletsToReload = 5 - bulletsInClip;
			if (bulletsToReload > bulletsOutOfClip)
				bulletsToReload = bulletsOutOfClip;
			bulletsOutOfClip -= bulletsToReload;
			bulletsInClip += bulletsToReload;
			audioScript.playReloadSound ();
		}
	}

	void fireBullet()
	{
		audioScript.playShotFiredSound ();
		createdBullet = Instantiate<GameObject> (bullet);
		createdFireParticleSystem = Instantiate<GameObject> (fireParticleSystem);
		SpriteRenderer sr = GetComponent<SpriteRenderer> ();
		bool isFlipped = GetComponent<SpriteRenderer> ().flipX;
		if (sr.sprite.name == "torso sprite sheet_1")
		{
			if (!isFlipped)
			{
				Vector3 bulletPosition = transform.position;
				bulletPosition += new Vector3 (1f, -0.1f, 0);
				createdBullet.transform.position = bulletPosition;
				createdFireParticleSystem.transform.position = bulletPosition;
			}
			else
			{
				Vector3 bulletPosition = transform.position;
				bulletPosition += new Vector3 (-1f, -0.1f, 0);
				createdBullet.transform.position = bulletPosition;
				createdFireParticleSystem.transform.position = bulletPosition;
			}
		}
		else if (sr.sprite.name == "torso sprite sheet_4")
		{
			if (!isFlipped)
			{
				Vector3 bulletPosition = transform.position;
				bulletPosition += new Vector3 (0.8f, 0.65f, 0);
				createdBullet.transform.position = bulletPosition;
				createdFireParticleSystem.transform.position = bulletPosition;
			}
			else
			{
				Vector3 bulletPosition = transform.position;
				bulletPosition += new Vector3 (-0.8f, 0.65f, 0);
				createdBullet.transform.position = bulletPosition;
				createdFireParticleSystem.transform.position = bulletPosition;
			}
		}
		else if (sr.sprite.name == "torso sprite sheet_3")
		{
			if (!isFlipped)
			{
				Vector3 bulletPosition = transform.position;
				bulletPosition += new Vector3 (-0.1f, 1, 0);
				createdBullet.transform.position = bulletPosition;
				createdFireParticleSystem.transform.position = bulletPosition;
			}
			else
			{
				Vector3 bulletPosition = transform.position;
				bulletPosition += new Vector3 (0.1f, 1, 0);
				createdBullet.transform.position = bulletPosition;
				createdFireParticleSystem.transform.position = bulletPosition;
			}
		}
		Destroy (createdFireParticleSystem, 1.0f);
	}
}
