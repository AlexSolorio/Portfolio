using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour {

	public bool legsFacingLeft;
	public bool isGrounded;
	public bool movingLeft;
	public bool movingRight;

	public Sprite right;
	public Sprite rightUp;
	public Sprite rightUpUp;

	public float horizontalInput;
	public float verticalInput;
	public float rotationInput;
	public float playerSpeed;

	public Animator legAnimator;
	public Rigidbody2D thisRigidbody;
	public SpriteRenderer spriteRenderer;
	public GameObject legs;

	// Use this for initialization
	void Start () 
	{

	}

	void Update()
	{
		pointTorsoInCorrectDirection ();
	}

	// Update is called once per frame
	void FixedUpdate () 
	{
		horizontalInput = Input.GetAxis ("Horizontal");
		verticalInput = Input.GetAxis ("Vertical");
		bool movingLeft = (horizontalInput < 0); 
		bool movingRight = (horizontalInput > 0);
		bool jump = (verticalInput > 0);

		if (movingLeft && !movingRight)
		{
			Vector3 position = transform.position;
			position.x -= playerSpeed;
			transform.position = position;
			if (legsFacingLeft)
				legs.GetComponent<SpriteRenderer>().flipX = false;
			legsFacingLeft = true;
		}
		else if (movingRight && !movingLeft)
		{
			Vector3 position = transform.position;
			position.x += playerSpeed;
			transform.position = position;
			if (legsFacingLeft)
				legs.GetComponent<SpriteRenderer>().flipX = true;
			legsFacingLeft = false;
		}

		if ((movingLeft || movingRight) && !((movingLeft && movingRight)))
			legAnimator.SetBool ("isRunning", true);
		else
			legAnimator.SetBool ("isRunning", false);
		
		if (isGrounded && jump)
			thisRigidbody.velocity = new Vector2 (thisRigidbody.velocity.x, 8.0f);
	}

	//changes the torso direction and sprite image depending on where on screen the mouse position is relative to the torso
	void pointTorsoInCorrectDirection ()
	{
		Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
		Vector3 gunToMouseVector = mousePosition - transform.position;

		if (Mathf.Abs((gunToMouseVector.y / gunToMouseVector.x)) > 3.0f)
			spriteRenderer.sprite = rightUpUp;
		
		else if (Mathf.Abs((gunToMouseVector.y / gunToMouseVector.x)) > 0.5f)
			spriteRenderer.sprite = rightUp;
		
		else
			spriteRenderer.sprite = right;

		if (gunToMouseVector.y < 0)
			spriteRenderer.sprite = right;

		if (mousePosition.x < transform.position.x)
			spriteRenderer.flipX = true;
		else
			spriteRenderer.flipX = false;
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
