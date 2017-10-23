using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

using System;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;


public class GameModel : MonoBehaviour {

	private static GameModel mSingleton;

	public static GameModel Instance { get { return mSingleton;} }
	public bool levelHasLoaded;
	public bool LoadedGame { get; set; }

	public GameObject player;
	public GameObject killsIndicator;
	public List<GameObject> zombieList;
	public Camera mainCamera;
	public string saveFileName;

	void Awake()
	{
		levelHasLoaded = false;
		LoadedGame = false;
		saveFileName = "last save";
		if (mSingleton == null)
		{
			mSingleton = this;
			DontDestroyOnLoad (gameObject);
		}
		else
			Destroy (gameObject);
	}

	void Update()
	{
		if (!levelHasLoaded)
		{
			if (SceneManager.GetActiveScene().name.Equals("Level 1"))
			{
				player = GameObject.FindGameObjectWithTag ("Player");
				killsIndicator = GameObject.FindGameObjectWithTag ("Kills Indicator");
				zombieList = GameObject.FindGameObjectWithTag ("Zombie Controller").GetComponent<ZombieSpawner> ().zombieList;
				mainCamera = Camera.main; 
				if (LoadedGame)
				{
					OnLoadClick ();
				}
				levelHasLoaded = true;
			}
		}
		if (Input.GetKeyDown("v") && SceneManager.GetActiveScene().name.Equals("Level 1"))
		{
			OnSaveClick ();
		}
	}

	public void OnSaveClick()
	{
		zombieList = GameObject.FindGameObjectWithTag ("Zombie Controller").GetComponent<ZombieSpawner> ().zombieList;
		SaveGame saveGame = new SaveGame ();

		//write to file
		SaveGameModel (saveGame, saveFileName);
	}

	public void OnLoadClick()
	{
		LoadGame (saveFileName);
	}

	public void SaveGameModel(SaveGame save, string filename)
	{
		BinaryFormatter bf = new BinaryFormatter ();
		FileStream fs = File.OpenWrite (Application.persistentDataPath + "/" + filename + ".dat");
		save.StoreData (this);
		bf.Serialize (fs, save);
		fs.Close ();
	}

	public void LoadGame(string filename)
	{
		BinaryFormatter bf = new BinaryFormatter ();
		FileStream fs = File.OpenRead (Application.persistentDataPath + "/" + filename + ".dat");
		SaveGame saveGame = (SaveGame)bf.Deserialize (fs);
		saveGame.LoadData (this);
		fs.Close ();
	}

}
