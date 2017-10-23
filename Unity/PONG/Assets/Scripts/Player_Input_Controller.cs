using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_Input_Controller : MonoBehaviour {


	//Script that handles input from two players
	//Player 1 => Controls left bat with W/S keys
	//Player 2 => Controls right bat with arrow keys

	public GameObject leftBat;
	public GameObject rightBat;
	public float batSpeeds;

	// Use this for initialization
	void Start () {
		batSpeeds = 15f;
	}
	
	// Update is called once per frame
	void Update () {

		leftBat.GetComponent<Rigidbody>().velocity = new Vector3(0f, 0f, 0f);
		rightBat.GetComponent<Rigidbody>().velocity = new Vector3(0f, 0f, 0f);

		if(Input.GetKey(KeyCode.W) && !Input.GetKey(KeyCode.S)){
			//Move the left bat up
			leftBat.GetComponent<Rigidbody>().velocity = new Vector3(0f, batSpeeds, 0f);
//			Vector3 position = leftBat.transform.position;
//			position.y += .1f;
//			leftBat.transform.position = position;
		}

		if(Input.GetKey(KeyCode.S) && !Input.GetKey(KeyCode.W)){
			//Move the left bat down
			leftBat.GetComponent<Rigidbody>().velocity = new Vector3(0f, -batSpeeds, 0f);
//			Vector3 position = leftBat.transform.position;
//			position.y -= .1f;
//			leftBat.transform.position = position;
		}

		if(Input.GetKey(KeyCode.UpArrow) && !Input.GetKey(KeyCode.DownArrow)){
			//Move the right bat up
			rightBat.GetComponent<Rigidbody>().velocity = new Vector3(0f, batSpeeds, 0f);
//			Vector3 position = rightBat.transform.position;
//			position.y += .1f;
//			rightBat.transform.position = position;
		}

		if(Input.GetKey(KeyCode.DownArrow) && !Input.GetKey(KeyCode.UpArrow)){
			//Move the right bat down
			rightBat.GetComponent<Rigidbody>().velocity = new Vector3(0f, -batSpeeds, 0f);
//			Vector3 position = rightBat.transform.position;
//			position.y -= .1f;
//			rightBat.transform.position = position;
		}

	}
}
