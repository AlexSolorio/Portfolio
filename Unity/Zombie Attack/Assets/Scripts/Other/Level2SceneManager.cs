using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class Level2SceneManager : MonoBehaviour {

	public KillsIndicatorController killsIndicatorController;
	public PlayerHealth playerHealth;
	public int killsNeededForWin;

	// Use this for initialization
	void Start () {

	}

	// Update is called once per frame
	void Update () {
		if (killsIndicatorController.zombiesKilled >= killsNeededForWin) 
		{
			SceneManager.LoadScene("Player Win");
		} 
		else if (playerHealth.currentHealth <= 0)
		{
			SceneManager.LoadScene ("Player Death");
		}
	}
}
