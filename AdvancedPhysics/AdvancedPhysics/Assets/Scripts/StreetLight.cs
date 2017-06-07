using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StreetLight : MonoBehaviour {

    public CapsuleCollider poleBody = null;
    public Rigidbody lightPostRB = null;
    public Light spotLight = null;
    public int numberOfHits = 0;

    // Use this for initialization
    void Start () {
        poleBody = transform.GetChild(0).GetComponent<CapsuleCollider>();
        lightPostRB = GetComponent<Rigidbody>();
        spotLight = transform.GetChild(1).GetComponent<Light>();

    }

    // Update is called once per frame
    void Update () {
		
	}

    public void PlayerHitPole()
    {
        if (numberOfHits < 2)
        {
            numberOfHits++;
        }
        // first hit turn spout on
        if (numberOfHits == 1)
        {
            // turn off spot light
            spotLight.enabled = false;
            return;
        }
        // second hit turn off spout and turn on base water
        if (numberOfHits == 2)
        {
            poleBody.GetComponent<CapsuleCollider>().enabled = true;
            lightPostRB.isKinematic = false;
        }
    }
}
