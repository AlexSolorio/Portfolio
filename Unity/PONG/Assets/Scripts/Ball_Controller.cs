using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball_Controller : MonoBehaviour {

	Rigidbody rb;
	public float xDirectionSpeed;
	int count;

	// Use this for initialization
	//Ball chooses a direction and flies that direction
	void Start () {
		
		rb = GetComponent<Rigidbody> ();
		StartCoroutine ("Pause");
	}
	
	// Update is called once per frame
	void Update () {
		if (transform.position.x < -25)
		{
			//Give player2 a point
			ScoreBoard_Controller.instance.givePlayer2aPoint();
			transform.position = Vector3.zero;
			rb.velocity = new Vector3 (0f, 0f, 0f);
			StartCoroutine ("Pause");
		}

		if (transform.position.x > 25)
		{
			//Give player1 a point
			ScoreBoard_Controller.instance.givePlayer1aPoint();
			transform.position = Vector3.zero;
			rb.velocity = new Vector3 (0f, 0f, 0f);
			StartCoroutine ("Pause");
		}
	}

	IEnumerator Pause()
	{
		transform.position = Vector3.zero;
		yield return new WaitForSeconds (2.5f);
		LaunchBall ();
	}

	void LaunchBall(){
		xDirectionSpeed = 10;

		int xDirection = Random.Range (0, 2);
		int yDirection = Random.Range (0, 3);

		Vector3 launchDirection = new Vector3 ();
		if (xDirection == 0)
			launchDirection.x = -xDirectionSpeed;
		if (xDirection == 1)
			launchDirection.x = xDirectionSpeed;
		if (yDirection == 0)
			launchDirection.y = -8f;
		if (yDirection == 1)
			launchDirection.y = 8f;

		rb.velocity = launchDirection;
		count = 0;
	}

	//when we hit something else...
	void OnCollisionEnter(Collision hit)
	{
		//if it was the top or bottom of the screen
		if (hit.gameObject.tag == "Boundaries" && count < 0)
		{
			float newXDirectionSpeed = 0f;
			if (rb.velocity.x > 0f)
				newXDirectionSpeed = xDirectionSpeed;
			if (rb.velocity.x < 0f)
				newXDirectionSpeed = -xDirectionSpeed;
			rb.velocity = new Vector3 (newXDirectionSpeed, rb.velocity.y * -1f, 0f);
			count = 10;
		}
		else{
			count--;
		}



		//if it was one of the bats
		if (hit.gameObject.name == "Left_Bat")
		{
			rb.velocity = new Vector3 (xDirectionSpeed, 0f, 0f);
			if(transform.position.y - hit.gameObject.transform.position.y < -2)
			{
				rb.velocity = new Vector3 (xDirectionSpeed, -8f, 0f);
			}
			if(transform.position.y - hit.gameObject.transform.position.y > 2)
			{
				rb.velocity = new Vector3 (xDirectionSpeed, 8f, 0f);
			}
		}

		if (hit.gameObject.name == "Right_Bat")
		{
			rb.velocity = new Vector3 (-xDirectionSpeed, 0f, 0f);
			if(transform.position.y - hit.gameObject.transform.position.y < -2)
			{
				rb.velocity = new Vector3 (-xDirectionSpeed, -8f, 0f);
			}
			if(transform.position.y - hit.gameObject.transform.position.y > 2)
			{
				rb.velocity = new Vector3 (-xDirectionSpeed, 8f, 0f);
			}
		}
	}
}
