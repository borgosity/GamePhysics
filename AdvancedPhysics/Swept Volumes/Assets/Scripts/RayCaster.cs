using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RayCaster : MonoBehaviour {
    public Text output;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

        RaycastHit hitInfo;
        if(Physics.Raycast(ray, out hitInfo, 500) == true)
        {
            output.text = hitInfo.transform.gameObject.name;             
        }        
	}
}
