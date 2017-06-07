using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class CannonBall : MonoBehaviour
{
    public float forceOnFire = 300;
    bool fire = false;
    bool canFire = true;
    Rigidbody _rigidbody = null;
    // Use this for initialization 
    void Start() { }

    private void Awake()
    {
        _rigidbody = GetComponent<Rigidbody>();
        _rigidbody.isKinematic = true;
    }
    // Update is called once per frame 
    void Update()
    {
        if (Input.anyKeyDown && canFire)
        {
            _rigidbody.isKinematic = false;
            _rigidbody.AddForce(-transform.forward * forceOnFire);
            canFire = false;
        }
    }
}
