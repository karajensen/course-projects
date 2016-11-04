using UnityEngine;
using System.Collections;

/*
 *<summary>
 *  LiquidParticle. 
 *  Class contains a circle with multiple states to represent different liquid types.
 *  Particles will need to scale in size over time with scaling effecting overall velocity of the sprite.
 *  Code has been provided to get you started. You will need to fill in the missing information from each function.
 *</summary>
 *<Author> AIE </Author>
 */


public class LiquidParticle : MonoBehaviour
{

	public enum LiquidStates
	{
		Water,
		Lava,
        Steam
	};
	
	LiquidStates currentState = LiquidStates.Water;
	public GameObject currentImage;
    public GameObject waterImage, lavaImage, steamImage;
	float m_startTime = 0.0f;
	float m_particleLifeTime = 0.0f;
    float m_downScaler = 1.0f;

	const float WATER_GRAVITYSCALE = 1.0f;
	const float LAVA_GRAVITYSCALE = 0.75f;
    const float STEAM_GRAVITYSCALE = -0.1f;

	/*
     *<summary>
     *  Pseudo constructor
     *</summary>
     */
	void Awake ()
	{
		m_startTime = 0.0f;
		SetState (currentState);
	}


	/*
   *<summary>
   *  Update loop called each tick as part of monobehavior
   *</summary>
   */
	void Update ()
	{
        MovementAnimation();
        ScaleDown();
    }

    /*
   *<summary>
   * return the state
   *</summary>
   */
   public LiquidStates GetState()
   {
        return currentState;
   }

    /*
   *<summary>
   *  Change an existing particle to a new type (eg water to lava)
   *</summary>
   *<param name="a_newState"> The new particle type to be passed in eg. LiquidStates.Lava </param>
   */
    public void SetState (LiquidStates a_newState)
	{
        var rb = currentImage.transform.parent.GetComponent<Rigidbody2D>();
        currentImage.SetActive (false);

		switch (a_newState)
        {
            case LiquidStates.Lava:
                currentImage = lavaImage;
                rb.gravityScale = LAVA_GRAVITYSCALE;
                break;
            case LiquidStates.Water:
                currentImage = waterImage;
                rb.gravityScale = WATER_GRAVITYSCALE;
                break;
            case LiquidStates.Steam:
                currentImage = steamImage;
                rb.gravityScale = STEAM_GRAVITYSCALE;
                break;
        }
		currentState = a_newState;   
		currentImage.SetActive (true);
	}


	/*
    *<summary>
    * Scales the particle based on its velocity.
    * This takes load of potential physics calculations.
    *</summary>
    */
	void MovementAnimation ()
	{
        var rb = currentImage.transform.parent.GetComponent<Rigidbody2D>();
        var speed = rb.velocity.magnitude;
        var scale = 1.0f + 0.085f * speed;
        currentImage.gameObject.transform.localScale = new Vector3(scale, scale, 1.0f);
    }


	/*
    *<summary>
    * Scales the size of the particle based on how long it has been alive. 
    * Gives the impression of a dying particle.
    *</summary>
    */
	void ScaleDown ()
	{
        m_downScaler -= 0.002f;

        var obj = currentImage.gameObject.transform;
        obj.localScale = new Vector3(
            obj.localScale.x * m_downScaler,
            obj.localScale.y * m_downScaler,
            1.0f);

        float minSize = currentState == LiquidStates.Steam ? 0.1f : 0.75f;
        if (obj.localScale.x <= minSize || obj.localScale.y <= minSize)
        {
            GameObject.Destroy(currentImage.transform.parent.gameObject);
        }
    }


	/*
     *<summary>
     *  Function allows for the external changing of the particles lifetime.
     *</summary>
     *<param name="a_newLifetime"> The new time the particle should live for. (eg. 4.0f seconds) </param>
     */
	public void SetLifeTime (float a_newLifetime)
	{
		
	}


	/*
     *<summary>
     *  This is where we would handle collisions between particles and call functions like our setState to change
     *  partcle types. Or we could just flat out destroy them etc..
     *</summary>
     *<param name="a_otherParticle"> The collision with another particle. Obviously not limited to particles so do a check in the method </param>
     */
	void OnCollisionEnter2D (Collision2D a_otherParticle)
	{
        var particle = a_otherParticle.gameObject.GetComponent<LiquidParticle>();
        if(particle)
        {
            if((particle.GetState() == LiquidStates.Lava && currentState == LiquidStates.Water) ||
               (particle.GetState() == LiquidStates.Water && currentState == LiquidStates.Lava))
            {
                particle.SetState(LiquidStates.Steam);
                SetState(LiquidStates.Steam);
            }
        }
        else if(currentState == LiquidStates.Steam)
        {
            GameObject.Destroy(currentImage.transform.parent.gameObject);
        }
	}
}
