using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour {

    private Animator animator = null;
    public List<Rigidbody> rigidbodies = new List<Rigidbody>();
    private GameController gameController = null;
    private GameObject sightSprite = null;
    public GameObject zombieEyeL = null;
    public GameObject zombieEyeR = null;

    // particles
    public ParticleSystem blood = null;

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
                // blood effect
                blood.Play();
                // remove zobie from count
                gameController.ZombieDied();
                sightSprite.SetActive(false);
                zombieEyeL.SetActive(false);
                zombieEyeR.SetActive(false);
                dead = true;
            }
        }
    }

	// Use this for initialization
	void Start () {
        // get game controller
        gameController = GameObject.FindObjectOfType<GameController>();
        // get particles
        GetParticles();
        // get eyes
        GetEyes();
        // get sight sprite
        sightSprite = transform.GetChild(2).gameObject;
        // get animator
        animator = GetComponent<Animator>();
        // disable rigid bodies
        foreach (Rigidbody r in rigidbodies)
            r.isKinematic = true;
	}
	
	// Update is called once per frame
	void Update () {

	}
      
    void GetEyes()
    {
        zombieEyeL = transform.GetChild(1). // root
                                    GetChild(0). // pelvis 
                                        GetChild(0). // chest
                                            GetChild(1). // lower head
                                                GetChild(0).gameObject;
        zombieEyeR = transform.GetChild(1). // root
                                    GetChild(0). // pelvis 
                                        GetChild(0). // chest
                                            GetChild(1). // lower head
                                                GetChild(1).gameObject;
    }
    void GetParticles()
    {
        blood = transform.GetChild(1). // root
                                    GetChild(1).GetComponent<ParticleSystem>();
    }
}
