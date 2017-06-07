using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireHydrant : MonoBehaviour
{

    public GameObject spoutWater = null;
    public GameObject baseWater = null;
    public Rigidbody hydrantRB = null;
    public GameObject hydrantBody = null;
    public int numberOfHits = 0;

    // Use this for initialization
    void Start()
    {
        spoutWater = transform.GetChild(1).transform.GetChild(0).transform.GetChild(0).gameObject;
        baseWater = transform.GetChild(0).transform.GetChild(0).gameObject;
        hydrantRB = transform.GetChild(1).GetComponent<Rigidbody>();
        hydrantBody = transform.GetChild(1).transform.GetChild(2).gameObject;
    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player")
        {
            if (numberOfHits < 2)
            {
                numberOfHits++;
            }
        }
        // first hit turn spout on
        if (numberOfHits == 1)
        {
            spoutWater.SetActive(true);
        }
        // second hit turn off spout and turn on base water
        if (numberOfHits == 2)
        {
            spoutWater.SetActive(false);
            baseWater.SetActive(true);
            hydrantBody.GetComponent<CapsuleCollider>().enabled = true;
            hydrantRB.isKinematic = false;
        }
    }
}
