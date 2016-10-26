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
		Lava	//2 States
	};
	//Different liquid types
	LiquidStates currentState = LiquidStates.Water;
	public GameObject currentImage;
	//The image is for the metaball shader for the effect.
	public GameObject waterImage, lavaImage;
	float m_startTime = 0.0f;
	float m_particleLifeTime = 0.0f;

	const float WATER_GRAVITYSCALE = 0.0f;
	const float LAVA_GRAVITYSCALE = 0.0f;


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
		
	}


	/*
   *<summary>
   *  Change an existing particle to a new type (eg water to lava)
   *</summary>
   *<param name="a_newState"> The new particle type to be passed in eg. LiquidStates.Lava </param>
   */
	public void SetState (LiquidStates a_newState)
	{
		currentImage.SetActive (false);
		switch (a_newState) {
		default:
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
		
	}


	/*
    *<summary>
    * Scales the size of the particle based on how long it has been alive. 
    * Gives the impression of a dying particle.
    *</summary>
    */
	void ScaleDown ()
	{ 
		
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
		

	}
	
}
