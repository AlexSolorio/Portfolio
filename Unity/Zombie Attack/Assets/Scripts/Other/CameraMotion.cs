using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMotion : MonoBehaviour {

	public GameObject player;

	// Use this for initialization
	void Start () {
		player = GameObject.FindGameObjectWithTag ("Player");
	}
	
	// Update is called once per frame
	void Update () {
		float horizontalInput = Input.GetAxis ("Horizontal");
		bool scrollLeft = (horizontalInput < 0 && (player.transform.position.x <= Camera.main.transform.position.x - 2.5f));
		bool scrollRight = (horizontalInput > 0 && (player.transform.position.x >= Camera.main.transform.position.x + 2.5f));
		if (scrollLeft)
		{
			Vector3 cameraPosition = Camera.main.transform.position;
			cameraPosition.x -= 0.1f;
			Camera.main.transform.position = cameraPosition;
		}
		if (scrollRight)
		{
			Vector3 cameraPosition = Camera.main.transform.position;
			cameraPosition.x += 0.1f;
			Camera.main.transform.position = cameraPosition;
		}
	}
}
