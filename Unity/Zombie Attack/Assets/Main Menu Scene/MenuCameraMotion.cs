using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuCameraMotion : MonoBehaviour {

	public float scrollSpeed;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		Vector3 position = transform.position;
		if (position.x >= 17.778)
		{
			transform.position = new Vector3 (0, -0.00206f, -10);
		}
		else
		{
			position.x += scrollSpeed;
			transform.position = position;
		}
	}
}
