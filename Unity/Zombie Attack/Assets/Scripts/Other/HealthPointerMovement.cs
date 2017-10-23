using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthPointerMovement : MonoBehaviour {

	public GameObject player;
	public PlayerHealth playerHealth;
	public float playerMaxHealth;

	// Use this for initialization
	void Start () {
		player = GameObject.FindGameObjectWithTag ("Player");
		playerHealth = player.GetComponent<PlayerHealth> ();
		playerMaxHealth = playerHealth.maxHealth;
	}
	
	// Update is called once per frame
	void Update () {
		float playerCurrentHealth = playerHealth.currentHealth;
		Vector3 position = transform.localPosition;
		position.x = -4.33f + (8.66f * playerCurrentHealth / playerMaxHealth);
		if (position.x < -4.33f)
			position.x = -4.33f;
		transform.localPosition = position;
	}
}
