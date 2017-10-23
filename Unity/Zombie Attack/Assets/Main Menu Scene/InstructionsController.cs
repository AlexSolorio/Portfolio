using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class InstructionsController : MonoBehaviour {

	public string InstructionPage1Text;
	public string InstructionPage2Text;
	public string InstructionPage3Text;

	public Text instructionText;
	public int instructionPageNumber;
	public bool textHasChanged;
	public MenuAudioScript audioScript;
	public GameObject backwardButtonObj;
	public GameObject forwardButtonObj;
	public Button backwardButton;
	public Button forwardButton;

	// Use this for initialization
	void Start () {
		textHasChanged = true;
		InstructionPage1Text = 
			"\nZombie Attack is a game of survival." +
			"\n\nArmed only with a gun and a few bullets to start," +
			"\nyou must survive the abandoned, dreary streets of" +
			"\nDenver, doing what you must to protect yourself" +
			"\nfrom the bloodthirsty zombies.";
		InstructionPage2Text = 
			"It is vital that you keep on the move, as staying put " +
			"\nwill only result in you running out of bullets and " +
			"\nreaching your ultimate demise." +
			"\n\nPay close attention to both your health bar and ammo:" +
			"\nfailure to do so could cost your life." +
			"\n\nYou need 15 kills to beat each level.";
		InstructionPage3Text = 
			"\tControls:" +
			"\n\n\tMovement and Jumping: \t\tWASD" +
			"\n\tAiming and Shooting: \t\t\tMouse/Mouse Click" +
			"\n\tReload: \t\t\t\t\t\t\t\tR" +
			"\n\tAmmo Pickup: \t\t\t\t\t\tAutomatic";
	}
	
	// Update is called once per frame
	void Update () {
		if (textHasChanged)
		{
			switch (instructionPageNumber)
			{
			case 1:
				instructionText.text = InstructionPage1Text;
				instructionText.alignment = TextAnchor.UpperCenter;
				backwardButtonObj.SetActive(false);
				forwardButtonObj.SetActive(true);
				break;
			case 2:
				instructionText.text = InstructionPage2Text;
				instructionText.alignment = TextAnchor.UpperCenter;
				backwardButtonObj.SetActive(true);
				forwardButtonObj.SetActive(true);
				break;
			case 3:
				instructionText.text = InstructionPage3Text;
				instructionText.alignment = TextAnchor.UpperLeft;
				forwardButtonObj.SetActive(false);
				backwardButtonObj.SetActive(true);
				break;
			default:
				Debug.Log ("Instruction number out of bounds.");
				break;
			}
			textHasChanged = false;
		}
	}

	public void increaseInstructionPageNumber()
	{
		audioScript.playClickNoise ();
		textHasChanged = true;
		instructionPageNumber++;
	}

	public void decreaseInstructionPageNumber()
	{
		audioScript.playClickNoise ();
		textHasChanged = true;
		instructionPageNumber--;
	}

	public void setInstructionPageNumber(int i)
	{
		instructionPageNumber = i;
		textHasChanged = true;
	}
}
