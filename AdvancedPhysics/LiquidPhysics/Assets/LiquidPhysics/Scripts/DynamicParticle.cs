using UnityEngine;
using System.Collections;
/// <summary>
/// Dynamic particle.
/// 
/// The dynamic particle is the backbone of the liquids effect. Its a circle with physics with 3 states, each state change its physic properties and its sprite color ( so the shader can separate wich particle is it to draw)
/// The particles scale down and die, and have a scale  effect towards their velocity.
/// 
/// Visit: www.codeartist.mx for more stuff. Thanks for checking out this example.
/// Credit: Rodrigo Fernandez Diaz
/// Contact: q_layer@hotmail.com
/// </summary>

public class DynamicParticle : MonoBehaviour {	
	public enum STATES{WATER,GAS,LAVA,NONE}; //The 3 states of the particle
	STATES currentState=STATES.NONE; //Defines the currentstate of the particle, default is water
	public GameObject currentImage; //The image is for the metaball shader for the effect, it is onle seen by the liquids camera.
	public GameObject[] particleImages; //We need multiple particle images to reduce drawcalls
	float GAS_FLOATABILITY=7.0f; //How fast does the gas goes up?
	float particleLifeTime=3.0f,startTime;//How much time before the particle scalesdown and dies	

	void Awake(){ 
		if (currentState == STATES.NONE)
			SetState (STATES.WATER);
	}

	//The definitios to each state
	public void SetState(STATES newState){
		if(newState!=currentState){ //Only change to a different state
			switch(newState){
				case STATES.WATER:													
					GetComponent<Rigidbody2D>().gravityScale=1.0f; // To simulate Water density
				break;
				case STATES.GAS:		
					particleLifeTime=particleLifeTime/2.0f;	// Gas lives the time the other particles
					GetComponent<Rigidbody2D>().gravityScale=0.0f;// To simulate Gas density
					gameObject.layer=LayerMask.NameToLayer("Gas");// To have a different collision layer than the other particles (so gas doesnt rises up the lava but still collides with the wolrd)
				break;					
				case STATES.LAVA:
					GetComponent<Rigidbody2D>().gravityScale=0.3f; // To simulate the lava density
				break;	
				case STATES.NONE:
					Destroy(gameObject);
				break;
			}
			if(newState!=STATES.NONE){
				currentState=newState;
				startTime=Time.time;//Reset the life of the particle on a state change
				GetComponent<Rigidbody2D>().velocity=new Vector2();	// Reset the particle velocity	
				currentImage.SetActive(false);
				currentImage=particleImages[(int)currentState];
				currentImage.SetActive(true);
			}
		}		
	}
	void Update () {
		switch(currentState){
			case STATES.WATER: //Water and lava got the same behaviour
				MovementAnimation(); 
				ScaleDown();
			break;
			case STATES.LAVA:
				MovementAnimation();
				ScaleDown();
			break;
			case STATES.GAS:
				if(GetComponent<Rigidbody2D>().velocity.y<50){ //Limits the speed in Y to avoid reaching mach 7 in speed
					GetComponent<Rigidbody2D>().AddForce (new Vector2(0,GAS_FLOATABILITY)); // Gas always goes upwards
				}
				ScaleDown();
			break;

		}	
	}
	// This scales the particle image acording to its velocity, so it looks like its deformable... but its not ;)
	void MovementAnimation(){
		Vector3 movementScale=new Vector3(1.0f,1.0f,1.0f);//Tamaño de textura no de metaball			
		movementScale.x+=Mathf.Abs(GetComponent<Rigidbody2D>().velocity.x)/30.0f;
		movementScale.z+=Mathf.Abs(GetComponent<Rigidbody2D>().velocity.y)/30.0f;
		movementScale.y=1.0f;		
		currentImage.gameObject.transform.localScale=movementScale;
	}
	// The effect for the particle to seem to fade away
	void ScaleDown(){ 
		float scaleValue = 1.0f-((Time.time-startTime)/particleLifeTime);
		Vector2 particleScale=Vector2.one;
		if (scaleValue <= 0) {
						Destroy (gameObject);
		} else{
			particleScale.x=scaleValue;
			particleScale.y=scaleValue;
			transform.localScale=particleScale;
		}
	}

	// To change particles lifetime externally (like the particle generator)
	public void SetLifeTime(float time){
		particleLifeTime=time;	
	}
	// Here we handle the collision events with another particles, in this example water+lava= water-> gas
	void OnCollisionEnter2D(Collision2D other){
		if(currentState==STATES.WATER && other.gameObject.tag=="DynamicParticle"){ 
			if(other.collider.GetComponent<DynamicParticle>().currentState==STATES.LAVA){
				SetState(STATES.GAS);
			}
		}

	}
	
}
