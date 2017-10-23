using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ZombieSpawner : MonoBehaviour {

	public int numberOfZombies = 0;
	public int maxNumberOfZombies = 10;
	public float spawnDeltaTime;
	public float spawnInitialDelay;
	public float zombieSpawnHeight = 2;
	public List<GameObject> zombieList;
	public GameObject zombie;
	protected GameObject spawnedZombie;
	public static GameObject SaveLoadManagerObject;
	public GameModel model;
	public bool hasSpawnedLoadedZombies;

	// Use this for initialization
	void Start () {
		InvokeRepeating ("Spawn", spawnInitialDelay, spawnDeltaTime);
		hasSpawnedLoadedZombies = false;

		SaveLoadManagerObject = GameObject.FindGameObjectWithTag ("saveload");
		model = SaveLoadManagerObject.GetComponent<GameModel> ();
	}
	
	// Update is called once per frame
	void Update () {
		if (model.LoadedGame)
		{
			if (model.levelHasLoaded && !hasSpawnedLoadedZombies && SceneManager.GetActiveScene().name.Equals("Level 1"))
			{
				List<GameObject> loadedZombieList = model.zombieList;
				foreach(GameObject obj in loadedZombieList)
				{
					SpawnZombieAtLocation (obj.transform.position.x, obj.transform.position.y, obj.transform.position.z);
				}
				hasSpawnedLoadedZombies = true;
			}
		}
	}

	void Spawn()
	{
		if (numberOfZombies >= maxNumberOfZombies)
			return;
		spawnedZombie = Instantiate<GameObject> (zombie);
		zombieList.Add (spawnedZombie);
		float xValue = getCorrectXValue ();
		Debug.Log ("zombie spawned at " + xValue.ToString());
		spawnedZombie.transform.position = new Vector3 (xValue, zombieSpawnHeight, 0);
		numberOfZombies++;
	}

	void SpawnZombieAtLocation(float x, float y, float z)
	{
		Debug.Log ("spawned loaded zombie at" + x + y + z);
		spawnedZombie = Instantiate<GameObject> (zombie);
		zombieList.Add (spawnedZombie);
		spawnedZombie.transform.position = new Vector3 (x, y, z);
		numberOfZombies++;
	}

	float getCorrectXValue()
	{
		float xValue = Random.Range (-20.0f, 75.0f);

		//if zombie will spawn in current camera frame
		if (Mathf.Abs(xValue - Camera.main.transform.position.x) < 10)
			return getCorrectXValue ();
		return xValue;
	}
}
