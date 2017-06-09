using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZombieRayCast : MonoBehaviour {

    public float sightDistance = 2.6f;
    public float manicLifeTime = 2.5f;
    public float manicTimer = 0.0f;
    public bool goManic = false;

    // materials
    public Material zombieEyeL = null;
    public Material zombieEyeR = null;
    public SpriteRenderer sightSprite = null;
    public SpriteRenderer mouthSprite = null;


    // colours
    public Color32 detected = new Color32(255, 0, 0, 84);
    public Color32 normalColor = new Color32(238, 255, 4, 84);
    public Color manicEyes = new Color(0.6392157f, 0, 0, 1);
    public Color normalEyes = new Color(0.5490196f, 0.6392157f, 0, 1);

    // particles
    public ParticleSystem vomit = null;
    
    // animations
    private Animator animator = null;

    // ragdoll
    private Ragdoll ragdoll = null;

    // raycasts
    Vector3[] rayCastVectors = new Vector3[3];


    // Use this for initialization
    void Start () {
        animator = GetComponent<Animator>();
        ragdoll = GetComponent<Ragdoll>();
        GetEyes();
        GetMouth();
        GetParticles();
        sightSprite = transform.GetChild(2).gameObject.GetComponent<SpriteRenderer>();

    }

    // Update is called once per frame
    void Update () {
		if (goManic)
        {
            if (manicTimer > 0.0f)
            {
                manicTimer -= Time.deltaTime;
            }
            else
            {
                manicTimer = 0.0f;
                goManic = false;
                Relax();
            }
        }

        // adjust height if below 0
        if (transform.position.y < 0.0f)
        {
            Vector3 reset = new Vector3(transform.position.x, 0.05f, transform.position.z);
            transform.position = reset;
        }
        // keep zombie in bounds
        StayInBounds();
    }

    void FixedUpdate()
    {
        // skip updates if dead or manic
        if (goManic || ragdoll.dead)
            return;
        // update raycat vectors
        rayCastVectors[0] = transform.TransformDirection(Vector3.forward) * sightDistance;
        rayCastVectors[1] = Quaternion.Euler(0, 20, 0) * transform.forward * sightDistance;
        rayCastVectors[2] = Quaternion.Euler(0, -20, 0) * transform.forward * sightDistance;
        // update start position for debug
        Vector3 rayStartPos = new Vector3(transform.position.x, transform.position.y + 0.5f, transform.position.z);
        // loop through raycast to check for collisions
        for (int i = 0; i < 3; i++)
        {
            // draw debug rays
            Debug.DrawRay(rayStartPos, rayCastVectors[i], Color.green);
            // new raycast hit
            RaycastHit hit;
            // check for collision
            if (Physics.Raycast(rayStartPos, rayCastVectors[i], out hit, sightDistance))
            {
                if (hit.transform.gameObject.tag == "Player")
                {
                    manicTimer = manicLifeTime;
                    goManic = true;
                    GoManic();
                }
            }
        }

    }

    void GoManic()
    {
        zombieEyeL.SetColor("_Color", manicEyes);
        zombieEyeR.SetColor("_Color", manicEyes);
        zombieEyeL.SetColor("_EmissionColor", manicEyes);
        zombieEyeR.SetColor("_EmissionColor", manicEyes);
        sightSprite.color = detected;
        mouthSprite.enabled = true;
        animator.SetBool("Vomit", true);
        vomit.Play();
    }

    void Relax()
    {
        zombieEyeL.SetColor("_Color", normalEyes);
        zombieEyeR.SetColor("_Color", normalEyes);
        zombieEyeL.SetColor("_EmissionColor", normalEyes);
        zombieEyeR.SetColor("_EmissionColor", normalEyes);
        sightSprite.color =  normalColor;
        animator.SetBool("Vomit", false);
        vomit.Stop();
        mouthSprite.enabled = false;
        // square up rotation
        if (!ragdoll.dead)
        {
            transform.rotation = Quaternion.LookRotation(Vector3.back);
        }
    }

    void StayInBounds()
    {
        if (transform.position.z > 50)
        {
            transform.rotation = Quaternion.LookRotation(Vector3.back);
        }
        if (transform.position.z < -50)
        {
            transform.rotation = Quaternion.LookRotation(Vector3.forward);
        }
        if (transform.position.x > 50)
        {
            transform.rotation = Quaternion.LookRotation(Vector3.left);
        }
        if (transform.position.x < -50)
        {
            transform.rotation = Quaternion.LookRotation(Vector3.right);
        }
    }

    void GetEyes()
    {
        zombieEyeL = transform.GetChild(1). // root
                                    GetChild(0). // pelvis 
                                        GetChild(0). // chest
                                            GetChild(1). // lower head
                                                GetChild(0).gameObject.GetComponent<Renderer>().material;
        zombieEyeR = transform.GetChild(1). // root
                                    GetChild(0). // pelvis 
                                        GetChild(0). // chest
                                            GetChild(1). // lower head
                                                GetChild(1).gameObject.GetComponent<Renderer>().material;

    }

    void GetParticles()
    {
        vomit = transform.GetChild(1). // root
                                    GetChild(0). // pelvis 
                                        GetChild(0). // chest
                                            GetChild(1). // lower head
                                                GetChild(3).gameObject.GetComponent<ParticleSystem>();
    }

    void GetMouth()
    {
        mouthSprite = transform.GetChild(1). // root
                                    GetChild(0). // pelvis 
                                        GetChild(0). // chest
                                            GetChild(1). // lower head
                                                GetChild(4).gameObject.GetComponent<SpriteRenderer>();
    }
}
