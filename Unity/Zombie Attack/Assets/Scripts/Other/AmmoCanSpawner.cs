using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoCanSpawner : MonoBehaviour {

	public int numberOfAmmoCans;
	public int maxNumberOfAmmoCans;
	public float spawnDeltaTime;
	public float spawnInitialDelay;
	public float spawnYPosition;
	public GameObject ammoCan;
	protected GameObject spawnedAmmoCan;

	// Use this for initialization
	void Start () {
		InvokeRepeating ("Spawn", spawnInitialDelay, spawnDeltaTime);
	}

	// Update is called once per frame
	void Update () {

	}

	public void Spawn()
	{
		if (numberOfAmmoCans >= maxNumberOfAmmoCans)
			return;
		spawnedAmmoCan = Instantiate<GameObject> (ammoCan);
		float xValue = getCorrectXValue ();
		Debug.Log ("ammo can spawned at " + xValue.ToString());
		spawnedAmmoCan.transform.position = new Vector3 (xValue, spawnYPosition, 0);
		numberOfAmmoCans++;
	}

	float getCorrectXValue()
	{
		float xValue = Random.Range (-20.0f, 75.0f);

		//if ammo can will spawn in current camera frame
		if (Mathf.Abs(xValue - Camera.main.transform.position.x) < 10)
			return getCorrectXValue ();
		return xValue;
	}
}
