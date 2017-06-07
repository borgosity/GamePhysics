using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameController : MonoBehaviour {

    public int zombieCount = 0;
    public GameObject[] zombies;
    
    // Use this for initialization
	void Start () {
        zombies = GameObject.FindGameObjectsWithTag("Zombie");
        zombieCount = zombies.Length;
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKey("escape"))
        {
            Application.Quit();
        }
	}

    public void ZombieDied()
    {
        zombieCount--;
    }
}
