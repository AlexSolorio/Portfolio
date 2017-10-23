using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZombieHealth : MonoBehaviour {

	public const float maxHealth = 100;
	public float zombieHitReaction;
	public float currentHealth;
	public ZombieSpawner zombieSpawner;
	private int reactingCounter;
	public Animator animator;
	public KillsIndicatorController killsIndicatorController;
	public GameObject playerBloodParticleSystem;
	protected GameObject createdPlayerBloodParticleSystem;

	// Use this for initialization
	void Start () {
		currentHealth = maxHealth;
		killsIndicatorController = GameObject.FindGameObjectWithTag ("Kills Indicator").GetComponent<KillsIndicatorController> ();
		zombieSpawner = GameObject.FindGameObjectWithTag ("Zombie Controller").GetComponent<ZombieSpawner> ();
	}

	// Update is called once per frame
	void Update () {

	}

	void OnTriggerEnter2D (Collider2D coll)
	{
		GameObject thingHit = coll.gameObject;
		if (thingHit.tag == "Bullet")
		{
			takeDamage (50.0f, thingHit.GetComponent<bulletMovement>().bulletMovingRight);
			Destroy (thingHit);
			createdPlayerBloodParticleSystem = Instantiate<GameObject> (playerBloodParticleSystem);
			createdPlayerBloodParticleSystem.transform.position = transform.position;
			Destroy (createdPlayerBloodParticleSystem, 1.0f);
		}
	}

	void takeDamage(float amountOfDamage, bool bulletMovingRight)
	{
		currentHealth -= amountOfDamage;
		StartCoroutine("reactToBullet", bulletMovingRight);
		checkIfDead ();
	}

	IEnumerator reactToBullet(bool bulletMovingRight)
	{
		for (int i = 0; i < 6; i++)
		{
			if (bulletMovingRight)
			{
				Vector3 position = transform.position;
				position.x += zombieHitReaction;
				transform.position = position;
			}
			else
			{
				Vector3 position = transform.position;
				position.x -= zombieHitReaction;
				transform.position = position;
			}
			yield return new WaitForSeconds (.01f);
		}
	}

	void checkIfDead()
	{
		if (currentHealth <= 0)
		{
			killsIndicatorController.zombiesKilled++;
			animator.SetBool ("isDying", true);
			Destroy (this.GetComponent<BoxCollider2D> ());
			zombieSpawner.numberOfZombies--;
			zombieSpawner.zombieList.Remove (this.gameObject);
			Destroy (this.gameObject, 1.5f);
		}
	}
}
