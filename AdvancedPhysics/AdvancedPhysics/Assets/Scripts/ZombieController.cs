using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZombieController : MonoBehaviour {

    public GameObject zombiePrefab = null;
    public Transform zombieList = null;
    public int numberOfZombies = 10;
    public int maxZombies = 25;

    public Vector3[] positions;

    private float gridSize = 8.5f;
    private float innerGrid = 17.0f;
    private float outerGrid = 34.0f;


    void Awake()
    {
        // set positions
        Positions();
        // create zombies parent
        zombieList = new GameObject("ZombieList").transform;
        // create zombies
        for (int i = 0; i < numberOfZombies; i++)
        {
            GameObject zombie = GameObject.Instantiate(zombiePrefab);
            zombie.transform.position = positions[i];
            zombie.transform.SetParent(zombieList);
        }
    }

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    // set zombie positions
    void Positions()
    {
        // TODO: generat positions based on grid using nested for loops

        float posY = 0.05f;
        positions = new Vector3[maxZombies];
        positions[0] = new Vector3(0.0f, posY, 2.5f);
        positions[1] = new Vector3(innerGrid, posY, innerGrid);
        positions[2] = new Vector3(-innerGrid, posY, innerGrid);
        positions[3] = new Vector3(-innerGrid, posY, -innerGrid);
        positions[4] = new Vector3(innerGrid, posY, -innerGrid);
        positions[5] = new Vector3(outerGrid, posY, outerGrid);
        positions[6] = new Vector3(-outerGrid, posY, outerGrid);
        positions[7] = new Vector3(-outerGrid, posY, -outerGrid);
        positions[8] = new Vector3(outerGrid, posY, -outerGrid);
        positions[9] = new Vector3(0.0f, posY, outerGrid);
        positions[10] = new Vector3(0.0f, posY, -outerGrid);
        positions[11] = new Vector3(outerGrid, posY, 0.0f);
        positions[12] = new Vector3(-outerGrid, posY, 0.0f);
        positions[13] = new Vector3(0.0f, posY, innerGrid);
        positions[14] = new Vector3(0.0f, posY, -innerGrid);
        positions[15] = new Vector3(innerGrid, posY, 0.0f);
        positions[16] = new Vector3(-innerGrid, posY, 0.0f);


    }
}
