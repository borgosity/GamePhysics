using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[RequireComponent(typeof(CharacterController))]
//[RequireComponent(typeof(Animator))]
public class PlayerController : MonoBehaviour {
    CharacterController controller = null;
    //Animator animator = null;
    // text
    TextMesh speedoText = null;
    TextMesh zombieText = null;
    // particles
    ParticleSystem exhaustA = null;
    ParticleSystem exhaustB = null;
    // light materials
    Material brakeLightL = null;
    Material brakeLightR = null;
    Material turnLightL = null;
    Material turnLightR = null;
    Material reverseLightL = null;
    Material reverseLightR = null;


    public float speed = 80.0f;
    public float pushPower = 2.0f;

    public float boostPower = 0.0f;
    public float currentSpeed = 0.0f;

    private GameController gameController = null;
    public int zombieCount = 0;

    public bool stopped = false;

    // explosion settings
    public float radius = 30.0f;
    public float power = 80.0f;
    public float heightOffset = 0.0f;

    // Use this for initialization
    void Start()
    {
        // controllers
        controller = GetComponent<CharacterController>();
        gameController = GameObject.FindObjectOfType<GameController>();
        // particle systems
        exhaustA = transform.GetChild(1).GetChild(0).GetComponent<ParticleSystem>();
        exhaustB = transform.GetChild(2).GetChild(0).GetComponent<ParticleSystem>();
        // light materials
        brakeLightL = transform.GetChild(3).GetChild(0).GetComponent<Renderer>().material;
        brakeLightR = transform.GetChild(3).GetChild(1).GetComponent<Renderer>().material;
        turnLightL = transform.GetChild(3).GetChild(2).GetComponent<Renderer>().material;
        turnLightR = transform.GetChild(3).GetChild(3).GetComponent<Renderer>().material;
        reverseLightL = transform.GetChild(3).GetChild(4).GetComponent<Renderer>().material;
        reverseLightR = transform.GetChild(3).GetChild(5).GetComponent<Renderer>().material;
        // in game text
        speedoText = transform.GetChild(0).GetComponent<TextMesh>();
        zombieText = transform.GetChild(4).GetChild(0).GetComponent<TextMesh>();

        boostPower = 200.0f;
        // animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        float vertical = Input.GetAxis("Vertical");
        float horizontal = Input.GetAxis("Horizontal");
        // update boosters
        BoostUpdate(vertical, horizontal);
        // update tail lights
        LightsUpdate(vertical, horizontal);
        // movement
        currentSpeed = vertical * (speed + (vertical > 0 ? boostPower : 0.0f)) * Time.deltaTime;
        speedoText.text = (currentSpeed * 10).ToString("n0");
        controller.SimpleMove(transform.forward * currentSpeed);
        //controller.SimpleMove(transform.up * Time.deltaTime);
        transform.Rotate(transform.up, horizontal * (speed * 0.33f) * Time.deltaTime);
        //animator.SetFloat("Speed", vertical * speed * Time.deltaTime);

        // zombie text update
        ZombieTextUpdate();
    }

    void OnControllerColliderHit(ControllerColliderHit hit)
    {
        Rigidbody body = hit.collider.attachedRigidbody;
        if (body == null)
            return;


        if (body.isKinematic)
        {
            if (body.tag == "LightPole")
            {
                hit.gameObject.GetComponent<StreetLight>().PlayerHitPole();
            }
            return;
        }

        if (hit.moveDirection.y < -0.3F)
            return;

        Vector3 pushDir = new Vector3(hit.moveDirection.x, 0, hit.moveDirection.z);
        body.velocity = pushDir * pushPower;
    }

    void BoostUpdate(float vertical, float horizontal)
    {
        if (boostPower > 0.0f && vertical > 0.0f)
        {
            // reduce boost over time
            boostPower -= 0.5f;
            //start exhaust particles
            if (!exhaustA.isPlaying || !exhaustB.isPlaying)
            {
                exhaustA.Play();
                exhaustB.Play();
            }
        }
        else
        {
            // stop exhaust particles
            exhaustA.Stop();
            exhaustB.Stop();
        }
        // keep boost in check
        if (boostPower <= 0.0f)
        {
            boostPower = 0.0f;
        }
    }
    // update zombie text
    void ZombieTextUpdate()
    {
        zombieCount = gameController.zombieCount;
        if (zombieCount > 0)
        {
            zombieText.text = zombieCount + " Left";
        }
        else
        {
            zombieText.text = "Clear!";
        }
    }

    void LightsUpdate(float vertical, float horizontal)
    {
        // turning right
        if (horizontal > 0)
        {
            Color flash = turnLightR.color * Mathf.LinearToGammaSpace(0.5f);
            turnLightR.SetColor("_EmissionColor", flash);
        }
        else
        {
            Color flash = turnLightR.color * Mathf.LinearToGammaSpace(0.0f);
            turnLightR.SetColor("_EmissionColor", flash);
        }
        // turning left
        if (horizontal < 0)
        {
            Color flash = turnLightL.color * Mathf.LinearToGammaSpace(0.5f);
            turnLightL.SetColor("_EmissionColor", flash);
        }
        else
        {
            Color flash = turnLightL.color * Mathf.LinearToGammaSpace(0.0f);
            turnLightL.SetColor("_EmissionColor", flash);
        }
        // braking
        if (vertical < 1.0f && !stopped)
        {
            Color flash = brakeLightL.color * Mathf.LinearToGammaSpace(0.5f);
            brakeLightL.SetColor("_EmissionColor", flash);
            brakeLightR.SetColor("_EmissionColor", flash);
            if (vertical <= 0.0f)
            {
                stopped = true;
            }
        }
        else if (vertical == 0.0f)
        {
            Color flash = brakeLightL.color * Mathf.LinearToGammaSpace(0.5f);
            brakeLightL.SetColor("_EmissionColor", flash);
            brakeLightR.SetColor("_EmissionColor", flash);
        }
        else
        {
            Color flash = brakeLightL.color * Mathf.LinearToGammaSpace(0.1f);
            brakeLightL.SetColor("_EmissionColor", flash);
            brakeLightR.SetColor("_EmissionColor", flash);
            if (vertical == 1.0f)
            {
                stopped = false;
            }
        }
        // reversing
        if (vertical < 0.0f)
        {
            Color flash = reverseLightL.color * Mathf.LinearToGammaSpace(0.5f);
            reverseLightL.SetColor("_EmissionColor", flash);
            reverseLightR.SetColor("_EmissionColor", flash);
        }
        else
        {
            Color flash = reverseLightL.color * Mathf.LinearToGammaSpace(0.0f);
            reverseLightL.SetColor("_EmissionColor", flash);
            reverseLightR.SetColor("_EmissionColor", flash);
        }
    }

    void OnTriggerEnter(Collider other)
    {
        Ragdoll ragdoll = other.gameObject.GetComponentInParent<Ragdoll>();
        if (ragdoll != null && !ragdoll.dead)
        {
            ragdoll.RagdollOn = true;
            ZombieHit();
        }
    }

    void ZombieHit()
    {
        Vector3 explosionPos = transform.position;
        // get all colliders in the radius of the bomb
        Collider[] colliders = Physics.OverlapSphere(explosionPos, radius * 0.5f);
        // apply explosion to rigidbodies
        foreach (Collider hit in colliders)
        {
            // skip boxes
            if (hit.tag == "Box")
                continue;

            Rigidbody rb = hit.GetComponent<Rigidbody>();
            if (rb != null)
            {
                // add explosive force
                rb.AddExplosionForce(power, explosionPos, radius, heightOffset);
            }
        }
    }

}
