using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.Serialization;

[Serializable]
public class SaveGame : ISerializable {

	public float Health { get; set; }
	public int bulletsInClip;
	public int bulletsOutOfClip;
	public int zombiesKilled;
	public Vector3 playerPosition;
	public Vector3 cameraPosition;
	public int numberOfZombies;
	public GameObject[] zombies;

	public void StoreData(GameModel model)
	{
		Health = model.player.GetComponent<PlayerHealth> ().currentHealth;
		bulletsInClip = model.player.GetComponent<bulletGenerator> ().bulletsInClip;
		bulletsOutOfClip = model.player.GetComponent<bulletGenerator> ().bulletsOutOfClip;
		zombiesKilled = model.killsIndicator.GetComponent<KillsIndicatorController> ().zombiesKilled;
		playerPosition = model.player.gameObject.transform.position;
		cameraPosition = model.mainCamera.gameObject.transform.position;

		numberOfZombies = model.zombieList.Count;
		Debug.Log ("saved zombies number: " + numberOfZombies);
		zombies = new GameObject[numberOfZombies];
		int i = 0;
		foreach(GameObject obj in model.zombieList)
		{
			zombies [i] = obj;
			i++;
		}
	}

	public void LoadData(GameModel model)
	{
		model.player.GetComponent<PlayerHealth> ().currentHealth = Health;
		model.player.GetComponent<bulletGenerator> ().bulletsInClip = bulletsInClip;
		model.player.GetComponent<bulletGenerator> ().bulletsOutOfClip = bulletsOutOfClip;
		model.killsIndicator.GetComponent<KillsIndicatorController> ().zombiesKilled = zombiesKilled;
		model.player.gameObject.transform.position = playerPosition;
		model.mainCamera.gameObject.transform.position = cameraPosition;
		Debug.Log ("loaded zombies number: " + zombies.Length);

		model.zombieList = new List<GameObject> ();
		for (int i = 0; i < zombies.Length; i++)
		{
			model.zombieList.Add (zombies [i]);
		}
	}

	public void GetObjectData(SerializationInfo info, StreamingContext context)
	{
		info.AddValue ("posx", playerPosition.x);
		info.AddValue ("posy", playerPosition.y);
		info.AddValue ("posz", playerPosition.z);

		info.AddValue ("cameraX", cameraPosition.x);
		info.AddValue ("cameraY", cameraPosition.y);
		info.AddValue ("cameraZ", cameraPosition.z);

		info.AddValue ("health", Health);
		info.AddValue ("bulletsInClip", bulletsInClip);
		info.AddValue ("bulletsOutOfClip", bulletsOutOfClip);
		info.AddValue ("zombiesKilled", zombiesKilled);
		info.AddValue ("numberOfZombies", numberOfZombies);

		for (int i = 0; i < zombies.Length; i++)
		{
			info.AddValue ("zombieX" + i.ToString(), zombies[i].transform.position.x);
			info.AddValue ("zombieY" + i.ToString(), zombies[i].transform.position.y);
			info.AddValue ("zombieZ" + i.ToString(), zombies[i].transform.position.z);
		}
	}

	public SaveGame() {}

	public SaveGame(SerializationInfo info, StreamingContext context)
	{

		playerPosition = new Vector3 (
			info.GetSingle ("posx"),
			info.GetSingle ("posy"),
			info.GetSingle ("posz"));
		cameraPosition = new Vector3 (
			info.GetSingle ("cameraX"),
			info.GetSingle ("cameraY"),
			info.GetSingle ("cameraZ"));
		Health = info.GetSingle ("health");
		bulletsInClip = info.GetInt32 ("bulletsInClip");
		bulletsOutOfClip = info.GetInt32 ("bulletsOutOfClip");
		zombiesKilled = info.GetInt32 ("zombiesKilled");
		numberOfZombies = info.GetInt32 ("numberOfZombies");
		zombies = new GameObject[numberOfZombies];

		for (int i = 0; i < numberOfZombies; i++)
		{
			zombies [i] = new GameObject ();
			zombies[i].transform.position = new Vector3 (
				info.GetSingle ("zombieX" + i.ToString()),
				info.GetSingle ("zombieY" + i.ToString()),
				info.GetSingle ("zombieZ" + i.ToString()));
		}
	}
}
