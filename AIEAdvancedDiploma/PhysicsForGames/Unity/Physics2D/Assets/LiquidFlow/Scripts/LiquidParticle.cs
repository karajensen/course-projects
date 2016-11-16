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
        Steam,
        Vapour
	};
	
	LiquidStates currentState = LiquidStates.Water;
	public GameObject currentImage;
    public GameObject waterImage, lavaImage, steamImage, vapourImage;
    public float m_timer = 0.0f;
    public float m_particleLifeTime = 0.0f;
    public bool m_immortal = false;

    const float MAX_SIZE = 2.0f;
    const float WATER_GRAVITYSCALE = 1.0f;
	const float LAVA_GRAVITYSCALE = 0.5f;
    const float STEAM_GRAVITYSCALE = -0.1f;
    const float VAPOUR_GRAVITYSCALE = -0.01f;

	/*
     *<summary>
     *  Pseudo constructor
     *</summary>
     */
	void Awake ()
	{
        m_timer = m_particleLifeTime;
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

        if (!m_immortal)
        {
            m_timer -= Time.deltaTime;
            ScaleDown();
        }
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
        var go = currentImage.transform.parent.gameObject;
        var rb = go.GetComponent<Rigidbody2D>();
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
            case LiquidStates.Vapour:
                currentImage = vapourImage;
                rb.gravityScale = VAPOUR_GRAVITYSCALE;
                break;
        }

        rb.velocity = new Vector2(0, 0);
        rb.angularVelocity = 0.0f;
        m_timer = m_particleLifeTime;

        currentState = a_newState;   
		currentImage.SetActive (true);
        go.layer = currentImage.gameObject.layer;
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
        var speedMultiplier = 0.01f;
        SetScale(MAX_SIZE - speed * speedMultiplier);
    }
    
	/*
    *<summary>
    * Scales the size of the particle based on how long it has been alive. 
    * Gives the impression of a dying particle.
    *</summary>
    */
	void ScaleDown()
	{
        if(currentState != LiquidStates.Vapour)
        {
            float ratio = Mathf.Min(1.0f, Mathf.Max(0.0f, m_timer / m_particleLifeTime));
            SetScale(GetScale() * ratio);
        }

        float minSize = 0.25f;
        if (GetScale() <= minSize)
        {
            GameObject.Destroy(currentImage.transform.parent.gameObject);
        }
    }

    /*
     *<summary>
     * Sets the scale of the particle
     *</summary>
     */
     private void SetScale(float scale)
     {
        currentImage.gameObject.transform.localScale = new Vector3(scale, scale, 1.0f);
     }

    /*
     *<summary>
     * Gets the scale of the particle
     *</summary>
     */
    private float GetScale()
    {
        return currentImage.gameObject.transform.localScale.x;
    }

    /*
     *<summary>
     *  Function allows for the external changing of the particles lifetime.
     *</summary>
     *<param name="a_newLifetime"> The new time the particle should live for. (eg. 4.0f seconds) </param>
     */
    public void SetLifeTime (float a_newLifetime)
	{
        m_particleLifeTime = a_newLifetime;
        m_timer = m_particleLifeTime;
    }

    /*
     *<summary>
     *  Function allows for a particle to live forever
     *</summary>
     *<param name="a_newLifetime"> The new time the particle should live for. (eg. 4.0f seconds) </param>
     */
    public void SetImmortal()
    {
        m_immortal = true;
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
            SetState(LiquidStates.Vapour);
            SetScale(MAX_SIZE);
        }
        else if(currentState == LiquidStates.Vapour)
        {
            if(a_otherParticle.gameObject.layer == LayerMask.NameToLayer("Barrier"))
            {
                GameObject.Destroy(currentImage.transform.parent.gameObject);
            }
        }
	}
}
