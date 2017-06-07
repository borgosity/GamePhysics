using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RagdollTrigger : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnTriggerEnter(Collider other)
    {
        Ragdoll ragdoll = other.gameObject.GetComponentInParent<Ragdoll>();
        if (ragdoll != null)
            ragdoll.RagdollOn = true;
    }
}
