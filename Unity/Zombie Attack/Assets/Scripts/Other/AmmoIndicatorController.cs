using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AmmoIndicatorController : MonoBehaviour {

	public Sprite ammoOut;
	public Sprite ammo1;
	public Sprite ammo2;
	public Sprite ammo3;
	public Sprite ammo4;
	public Sprite ammo5;

	public SpriteRenderer spriteRenderer;
	public GameObject player;
	public bulletGenerator bulletGenerator;
	public Text bulletsOutOfClipText;

	// Use this for initialization
	void Start () {
		bulletGenerator = player.GetComponent<bulletGenerator> ();
	}
	
	// Update is called once per frame
	void Update () {
		int bulletsInClip = bulletGenerator.bulletsInClip;
		if (bulletsInClip == 5)
			spriteRenderer.sprite = ammo5;
		else if (bulletsInClip == 4)
			spriteRenderer.sprite = ammo4;
		else if (bulletsInClip == 3)
			spriteRenderer.sprite = ammo3;
		else if (bulletsInClip == 2)
			spriteRenderer.sprite = ammo2;
		else if (bulletsInClip == 1)
			spriteRenderer.sprite = ammo1;
		else
			spriteRenderer.sprite = ammoOut;

		setBulletText ();
	}

	void setBulletText()
	{
		bulletsOutOfClipText.text = "+" + bulletGenerator.bulletsOutOfClip.ToString();
	}
}
