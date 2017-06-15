using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParticleController : MonoBehaviour {

    public GameObject explosion;
    public GameObject balloonPop;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void PlayExplosion(Vector3 position)
    {
       GameObject temp = GameObject.Instantiate(explosion);
       temp.transform.position = position;   
    }
    public void PlayBalloonPop(Vector3 position)
    {
        GameObject temp = GameObject.Instantiate(balloonPop);
        temp.transform.position = position;
    }
}
