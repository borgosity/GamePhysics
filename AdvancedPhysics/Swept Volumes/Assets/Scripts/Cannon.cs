using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cannon : MonoBehaviour {

    public GameObject cannonBallPrefab = null;
    public Transform spawnPoint = null;
    public float forceOnFire = 1200;
    public float cooldownTime = 0.2f;

    float cooldownTimer = 0;
    
    // Update is called once per frame
    void Update () {
		if(Input.anyKeyDown && cooldownTimer <= 0)
        {
            GameObject go = Instantiate(cannonBallPrefab, spawnPoint.position, spawnPoint.rotation);

            if (go == null)
                return;

            Rigidbody rb = go.GetComponent<Rigidbody>();
            if (rb == null)
                return;
                        
            rb.AddForce(go.transform.forward * forceOnFire);            
            cooldownTimer = cooldownTime;
        }
        if(cooldownTimer > 0)
            cooldownTimer -= Time.deltaTime;
    }   
}
