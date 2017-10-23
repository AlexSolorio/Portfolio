using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class ButtonManager : MonoBehaviour {

	public GameObject mainMenuPanel;
	public GameObject instructionsPanel;

	public MenuAudioScript audioScript;
	public InstructionsController instructionsController;

	// Use this for initialization
	void Start () 
	{
		if (mainMenuPanel != null && instructionsPanel != null)
		{
			mainMenuPanel.SetActive (true);
			instructionsPanel.SetActive (false);
		}
	}

	public void switchToScene(string sceneName)
	{
		audioScript.playClickNoise ();
		SceneManager.LoadScene (sceneName);
	}

	public void ExitGameButton()
	{
		audioScript.playClickNoise ();
		Application.Quit();
	}

	public void InstructionsButton()
	{
		audioScript.playClickNoise ();
		mainMenuPanel.SetActive (false);
		instructionsPanel.SetActive (true);
	}

	public void BackButton()
	{
		audioScript.playClickNoise ();
		mainMenuPanel.SetActive (true);
		instructionsPanel.SetActive (false);
		instructionsController.setInstructionPageNumber (1);
	}
}
