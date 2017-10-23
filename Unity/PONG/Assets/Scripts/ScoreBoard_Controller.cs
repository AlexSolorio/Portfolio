using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScoreBoard_Controller : MonoBehaviour {

	public Text player1ScoreText;
	public Text player2ScoreText;
	public static ScoreBoard_Controller instance;
	public int player1Score;
	public int player2Score;

	// Use this for initialization
	void Start () {
		instance = this;
		player1Score = player2Score = 0;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void givePlayer2aPoint()
	{
		player2Score++;
		player2ScoreText.text = player2Score.ToString ();
	}

	public void givePlayer1aPoint()
	{
		player1Score++;
		player1ScoreText.text = player1Score.ToString ();
	}
}
