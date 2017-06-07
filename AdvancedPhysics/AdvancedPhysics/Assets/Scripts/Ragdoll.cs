using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour {

    private Animator animator = null;
    public List<Rigidbody> rigidbodies = new List<Rigidbody>();
    private GameController gameController = null;
    public bool dead = false;


    public bool RagdollOn {
        get { return !animator.enabled; }
        set
        {
            if (!dead)
            {
                animator.enabled = !value;
                foreach (Rigidbody r in rigidbodies)
                {
                    r.isKinematic = !value; 
                }
                // remove zobie from count
                gameController.ZombieDied();
                dead = true;
            }
        }
    }

	// Use this for initialization
	void Start () {
        gameController = GameObject.FindObjectOfType<GameController>();
        animator = GetComponent<Animator>();
        foreach (Rigidbody r in rigidbodies)
            r.isKinematic = true;
	}
	
	// Update is called once per frame
	void Update () {

	}
        
}
