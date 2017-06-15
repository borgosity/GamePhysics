using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RayCaster : MonoBehaviour {
    public Text output;
    public GameObject crosshair;
    public ParticleController particleController;

    public Texture2D cursorTexture;
    public CursorMode cursorMode = CursorMode.Auto;
    public Vector2 hotSpot;

    void Start()
    {
        hotSpot = new Vector2(cursorTexture.width * 0.5f, cursorTexture.height * 0.5f);
        Cursor.SetCursor(cursorTexture, hotSpot, cursorMode);
    }
    // Update is called once per frame 
    void Update ()
    {
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit hitInfo;
        if (Physics.Raycast(ray, out hitInfo, 500) == true)
        {
            // update crosshair
            //Vector3 crosshairPos = new Vector3(hitInfo.point.x, hitInfo.point.y, crosshair.transform.position.z);
           // crosshair.transform.position = crosshairPos;

            Vector3 newPos = new Vector3(hitInfo.point.x, hitInfo.point.y, hitInfo.transform.position.z);
            output.text = hitInfo.transform.gameObject.name + "- " + newPos.x + " ," + newPos.y;
            // check for hits
            if (Input.GetAxis("Fire1") > 0.0f)
            {
                if (hitInfo.transform.gameObject.tag == "ExplodingBox")
                {
                    particleController.PlayExplosion(hitInfo.transform.position);
                    Destroy(hitInfo.transform.gameObject);
                    //hitInfo.transform.position = newPos;
                }
                if (hitInfo.transform.gameObject.tag == "Balloon")
                {
                    particleController.PlayBalloonPop(hitInfo.transform.position);
                    hitInfo.transform.gameObject.GetComponent<Cloth>().enabled = true;
                }
            }
        }
    }
}
