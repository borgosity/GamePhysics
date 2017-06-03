using UnityEngine;
using System.Collections;

/// LiquidParticle. 
/// Class contains a circle with multiple states to represent different liquid types.
/// Particles will need to scale in size over time with scaling effecting overall velocity of the sprite.
/// Code has been provided to get you started. You will need to fill in the missing information from each function.

[RequireComponent (typeof(Rigidbody2D))]  
public class LiquidParticle : MonoBehaviour
{

    public enum LiquidStates
    {
	    Water,
	    Lava	//2 States
    };

    //Different liquid types
    LiquidStates currentState = LiquidStates.Water;
    
    public Material waterMaterial;
    public Material lavaMaterial;

    float startTime = 0.0f;
    float particleLifeTime = 3.0f;

    const float WATER_GRAVITYSCALE = 1.0f;
    const float LAVA_GRAVITYSCALE = 0.3f;

    private Rigidbody2D _rigidbody = null;

    
    void Awake ()
    {
        _rigidbody = GetComponent<Rigidbody2D>();

        startTime = 0.0f;
	    SetState (currentState);
    }
    
    void Update ()
    {
    }


    /// SetState
    /// Change an existing particle to a new type (eg water to lava)
    /// a_newState: The new particle type to be passed in eg. LiquidStates.Lava 
    public void SetState (LiquidStates newState)
    {
    }


    /// MovementAnimation
    /// Scales the particle based on its velocity.
    void MovementAnimation ()
    {
    }


    /// ScaleDown
    /// Scales the size of the particle based on how long it has been alive. 
    /// Gives the impression of a dying particle.
    void ScaleDown ()
    {
    }


    /// SetLifeTime
    /// Function allows for the external changing of the particles lifetime.
    /// a_newLifetime: The new time the particle should live for. (eg. 4.0f seconds) 
    public void SetLifeTime (float newLifetime)
    {
        particleLifeTime = newLifetime;
    }


    /// OnCollisionEnter2D
    /// This is where we would handle collisions between particles and call functions like our setState to change
    /// partcle types. Or we could just flat out destroy them etc..
    /// a_otherParticle: The collision with another particle. Obviously not limited to particles so do a check in the method 
    void OnCollisionEnter2D (Collision2D a_otherParticle)
    {
		

    }
	
}
