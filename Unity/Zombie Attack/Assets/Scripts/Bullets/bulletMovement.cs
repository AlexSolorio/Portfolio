using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bulletMovement : MonoBehaviour {

	public Vector3 fireDirection;
	public SpriteRenderer spriteRenderer;
	public bool bulletMovingRight;

	// Use this for initialization
	void Start () {
		//find the vector between start position and mouse position
		Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
		Vector3 newMousePosition = new Vector3 (mousePosition.x, mousePosition.y, 0);
		fireDirection = newMousePosition - transform.position;

		//finds normalized vector
		fireDirection /= fireDirection.magnitude;
		fireDirection *= 0.5f;

		if (fireDirection.x > 0)
			bulletMovingRight = true;
		else
			bulletMovingRight = false;
	}

	// Update is called once per frame
	void Update () {
		goForward ();
		if (!spriteRenderer.isVisible)
			Destroy (this.gameObject);
	}

	void goForward()
	{
		Vector3 position = transform.position;
		position += fireDirection;
		transform.position = position;
	}

	void OnCollisionEnter2D (Collision2D coll)
	{
		Destroy (this.gameObject);
	}
}
