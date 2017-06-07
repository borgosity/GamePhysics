using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpeedBoostScript : MonoBehaviour {

    public float boostBonus = 200.0f;
    TextMesh tileText = null;
    Transform cameraTransform = null;

    // Use this for initialization
    void Start () {
        tileText = transform.GetChild(0).GetComponent<TextMesh>();
        cameraTransform = Camera.main.transform;
    }
	
	// Update is called once per frame
	void Update () {

        // keep text facing camera
        tileText.transform.rotation = Quaternion.LookRotation(tileText.transform.position - cameraTransform.position, Vector3.up);

    }

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player")
        {
            PlayerController playerScript = other.gameObject.GetComponentInParent<PlayerController>();
            if (playerScript != null)
            {
                playerScript.boostPower = boostBonus;
            }
        }
    }
}
