using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZombieMovement : MonoBehaviour {

	public Animator animator;
	public bool isInRangeOfPlayer = false;
	public bool isGrounded = false;
	public GameObject player;
	public float zombieSpeed = 0.07f;
	public SpriteRenderer spriteRenderer;
	public Rigidbody2D rigidBody;
	public AudioScript audioScript;
	public PlayerHealth playerHealth;
	public GameObject playerBloodParticleSystem;
	protected GameObject createdPlayerBloodParticleSystem;
	public int dealDamageCounter;

	// Use this for initialization
	void Start () {
		player = GameObject.FindGameObjectWithTag ("Player");
		audioScript = GameObject.FindGameObjectWithTag ("Audio Controller").GetComponent<AudioScript> ();
		playerHealth = player.GetComponent<PlayerHealth> ();
		rigidBody = GetComponent<Rigidbody2D> ();
		animator.SetBool ("isDying", false);
		animator.SetBool ("isAttacking", false);
		animator.SetBool ("isWalking", false);
		dealDamageCounter = 0;
	}
	
	// Update is called once per frame
	void Update () {
		checkIfNearPlayer ();
		walkTowardPlayer ();
		dealDamageCounter++;
		if (dealDamageCounter > 20)
			dealDamageCounter = 0;
	}

	void checkIfNearPlayer ()
	{
		if (Mathf.Abs(transform.position.x - player.transform.position.x) < 6 && !isInRangeOfPlayer)
		{
			isInRangeOfPlayer = true;
			audioScript.playZombieMoanSound ();
		}
	}

	void walkTowardPlayer()
	{
		float distanceToPlayer = Vector3.Distance (transform.position, player.transform.position);
		if (animator.GetBool("isDying") || distanceToPlayer < .7f)
			return;

		if (!isInRangeOfPlayer)
			zombieSpeed = 0.02f;
		else
			zombieSpeed = 0.07f;

		if (transform.position.x < player.transform.position.x)
		{
			animator.SetBool ("isWalking", true);
			Vector3 position = transform.position;
			position.x += zombieSpeed;
			transform.position = position;
			if (!spriteRenderer.flipX)
				spriteRenderer.flipX = true;
		}
		else if (transform.position.x > player.transform.position.x)
		{
			animator.SetBool ("isWalking", true);
			Vector3 position = transform.position;
			position.x -= zombieSpeed;
			transform.position = position;
			if (spriteRenderer.flipX)
				spriteRenderer.flipX = false;
		}

		float differenceInX = Mathf.Abs (player.transform.position.x - transform.position.x);

		//jump
		if (transform.position.y + 1 < player.transform.position.y && isGrounded && differenceInX < 1)
		{
			jump ();
		}
	}

	void jump()
	{
		rigidBody.velocity = new Vector2 (0, 6.0f);
		isGrounded = false;
	}

	void OnTriggerEnter2D (Collider2D coll)
	{
		GameObject thingHit = coll.gameObject;
		if (thingHit.tag == "Player")
		{
			animator.SetBool ("isWalking", false);
			animator.SetBool ("isAttacking", true);
		}
		else if (thingHit.tag == "Ground")
		{
			jump ();
		}
	}

	void OnTriggerStay2D (Collider2D coll)
	{
		GameObject thingHit = coll.gameObject;
		if (thingHit.tag == "Player" && dealDamageCounter == 20)
		{
			thingHit.GetComponent<PlayerHealth> ().takeDamage (6f);
			createdPlayerBloodParticleSystem = Instantiate<GameObject> (playerBloodParticleSystem);
			createdPlayerBloodParticleSystem.transform.position = thingHit.transform.position;
			Destroy (createdPlayerBloodParticleSystem, 1.0f);
		}
		else if (thingHit.tag == "Ground")
		{
			jump ();
		}
	}

	void OnTriggerExit2D (Collider2D coll)
	{
		GameObject thingHit = coll.gameObject;
		if (thingHit.tag == "Player")
		{
			animator.SetBool ("isAttacking", false);
			animator.SetBool ("isWalking", true);
		}
	}

	void OnCollisionEnter2D(Collision2D coll)
	{
		if (coll.gameObject.tag == "Ground" && !Input.GetMouseButton(0))
			isGrounded = true;
	}

	void OnCollisionStay2D(Collision2D coll)
	{
		if (coll.gameObject.tag == "Ground" && !Input.GetMouseButton(0))
			isGrounded = true;
	}

	void OnCollisionExit2D(Collision2D coll)
	{
		if (coll.gameObject.tag == "Ground")
			isGrounded = false;
	}
}
