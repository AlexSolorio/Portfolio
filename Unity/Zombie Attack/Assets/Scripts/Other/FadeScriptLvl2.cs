using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class FadeScriptLvl2 : MonoBehaviour {

	public PlayerHealth playerHealth;
	public SpriteRenderer spriteRenderer;
	public bool fadingToBlack;
	public bool fadingToRed;
	public int killsNeededForWin;
	public KillsIndicatorController killsIndicatorController;
	public GameObject ammoText;
	public GameObject killsText;

	// Use this for initialization
	void Start () {
		fadingToBlack = false;
		fadingToRed = false;
		Color tmp = spriteRenderer.color;
		tmp.a = 0;
		spriteRenderer.color = tmp;
	}

	// Update is called once per frame
	void Update () {
		if (!fadingToRed && !fadingToBlack)
		{
			if (killsIndicatorController.zombiesKilled >= killsNeededForWin) 
			{
				StartCoroutine (fadeAndLoadPlayerWin ());
				fadingToBlack = true;
			} 
			else if (playerHealth.currentHealth <= 0)
			{
				StartCoroutine (fadeAndLoadPlayerDeath ());
				fadingToRed = true;
			}
		}
		else
		{
			ammoText.SetActive (false);
			killsText.SetActive (false);
		}
	}

	IEnumerator fadeAndLoadPlayerWin()
	{
		spriteRenderer.color = Color.black;
		Color tmp = spriteRenderer.color;
		tmp.a = 0;
		spriteRenderer.color = tmp;
		for (int i = 0; i < 40; i++)
		{
			Color tmp2 = spriteRenderer.color;
			tmp.a += (1.0f - tmp.a) * 0.1f;
			spriteRenderer.color = tmp;
			yield return new WaitForSeconds (0.05f);
		}
		SceneManager.LoadScene("Player Win");
	}

	IEnumerator fadeAndLoadPlayerDeath()
	{
		spriteRenderer.color = Color.red;
		Color tmp = spriteRenderer.color;
		tmp.a = 0;
		spriteRenderer.color = tmp;
		for (int i = 0; i < 40; i++)
		{
			Color tmp2 = spriteRenderer.color;
			tmp.a += (1.0f - tmp.a) * 0.1f;
			spriteRenderer.color = tmp;
			yield return new WaitForSeconds (0.05f);
		}
		SceneManager.LoadScene ("Player Death");
	}
}
