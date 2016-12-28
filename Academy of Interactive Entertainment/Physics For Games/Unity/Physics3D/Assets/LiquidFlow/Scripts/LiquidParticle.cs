using UnityEngine;
using System.Collections;

/*
 *<summary>
 *  LiquidParticle. 
 *  Class contains a circle with multiple states to represent different liquid types.
 *  Particles scale in size over time with scaling effecting overall velocity of the sprite.
 *</summary>
 *<Author> James Dannatt </Author>
 *<Credits to> Rodrigo Diaz </Credits to>
 */


public class LiquidParticle : MonoBehaviour
{

	public enum LiquidStates
	{
		Water,
		Lava
	};

	LiquidStates currentState = LiquidStates.Water;
	public GameObject currentImage;
	public GameObject waterImage, lavaImage;
	float m_timer = 0.0f;
	float m_particleLifeTime = 0.0f;

    const float MAX_SIZE = 2.0f;
    const float LAVA_MASS = 2.0f;
    const float WATER_MASS = 1.0f;

    /*
     *<summary>
     *  Pseudo constructor
     *</summary>
     */
    void Awake ()
	{
        m_timer = m_particleLifeTime;
        SetState(currentState);
    }


	/*
   *<summary>
   *  Update loop called each tick as part of monobehavior
   *</summary>
   */
	void Update ()
	{
        m_timer -= Time.deltaTime;
        MovementAnimation();
        ScaleDown();
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
        var rb = go.GetComponent<Rigidbody>();
        currentImage.SetActive(false);

        switch (a_newState)
        {
            case LiquidStates.Lava:
                currentImage = lavaImage;
                rb.mass = LAVA_MASS;
                break;
            case LiquidStates.Water:
                currentImage = waterImage;
                rb.mass = WATER_MASS;
                break;
        }

        rb.velocity = new Vector3(0, 0, 0);
        rb.angularVelocity = new Vector3(0, 0, 0);
        m_timer = m_particleLifeTime;

        currentState = a_newState;
        currentImage.SetActive(true);
    }


	/*
    *<summary>
    * Scales the particle based on its velocity.
    * This takes load of potential physics calculations.
    *</summary>
    */
	void MovementAnimation ()
	{
        var rb = currentImage.transform.parent.GetComponent<Rigidbody>();
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
	void ScaleDown ()
	{
        float ratio = Mathf.Min(1.0f, Mathf.Max(0.0f, m_timer / m_particleLifeTime));
        SetScale(GetScale() * ratio);

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
        currentImage.gameObject.transform.localScale = new Vector3(scale, scale, scale);
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
     *  This is where we would handle collisions between particles and call functions like our setState to change
     *  partcle types. Or we could just flat out destroy them etc..
     *</summary>
     *<param name="a_otherParticle"> The collision with another particle. Obviously not limited to particles so do a check in the method </param>
     */
	void OnCollisionEnter (Collision a_otherParticle)
	{
		

	}
	
}
