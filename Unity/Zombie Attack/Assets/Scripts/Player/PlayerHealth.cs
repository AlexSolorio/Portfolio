using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHealth : MonoBehaviour {

	public float maxHealth = 100;
	public float currentHealth;


	// Use this for initialization
	void Start () {
		currentHealth = maxHealth;
		StartCoroutine (slowlyIncreaseHealth ());
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void takeDamage(float amountOfDamage)
	{
		currentHealth -= amountOfDamage;

		checkIfDead ();
	}

	public void checkIfDead()
	{
		if (currentHealth <= 0)
		{
			Debug.Log ("Player has died.");
		}
	}

	IEnumerator slowlyIncreaseHealth()
	{
		while (true)
		{
			if (currentHealth < maxHealth)
				currentHealth+= .1f;
			yield return new WaitForSeconds (.1f);
		}
	}
}
