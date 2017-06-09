using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;


public class GameController : MonoBehaviour {

    public int zombieCount = 0;
    public GameObject[] zombies;
    public GameObject startCanvas = null;
    public GameObject finshedCanvas = null;

    public bool started = false;
    public bool finished = false;

    public float timer = 0.0f;
    public float finshedTimer = 10.0f;
    
    // Use this for initialization
	void Start () {
        zombies = GameObject.FindGameObjectsWithTag("Zombie");
        zombieCount = zombies.Length;
        startCanvas = GameObject.FindGameObjectWithTag("MainCanvas");
        //finshedCanvas = GameObject.FindGameObjectWithTag("FinishedCanvas");
    }

    // Update is called once per frame
    void Update () {
		if (Input.GetKey("escape"))
        {
            Application.Quit();
        }

        // paused
        if (!started)
        {
            Time.timeScale = 0;
            // start game
            if (Input.GetKey("space"))
            {
                startCanvas.SetActive(false);
                Time.timeScale = 1;
                started = true;
            }
        }

        // increase timer
        if (started && !finished)
        {
            timer += Time.deltaTime;
        }
        // if game finished
        if (finished)
        {
            if (!finshedCanvas.activeSelf)
            {
                finshedCanvas.SetActive(true);
                //finshedCanvas.transform.GetChild(2).gameObject.GetComponent<Text>().text = timer.ToString("n2");

                System.TimeSpan timeSpan = System.TimeSpan.FromSeconds(timer);
                string timeText = string.Format("{0:D2}:{1:D2}:{2:D2}", timeSpan.Minutes, timeSpan.Seconds, timeSpan.Milliseconds);
                finshedCanvas.transform.GetChild(2).gameObject.GetComponent<Text>().text = timeText;

            }
            finshedTimer -= Time.deltaTime;
            if (finshedTimer <= 0.0f)
            {
                SceneManager.LoadScene("MainScene");
            }
        }
	}

    public void ZombieDied()
    {
        zombieCount--;
        if (zombieCount == 0)
        {
            finished = true;
        }
    }
}
