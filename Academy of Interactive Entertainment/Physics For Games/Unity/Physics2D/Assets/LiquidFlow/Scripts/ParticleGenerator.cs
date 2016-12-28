using UnityEngine;
using System.Collections;

/*
 *<summary>
 *  Particle Generator. 
 *  Constantly generates liquid particles.
 *</summary>
 *<Author> James Dannatt </Author>
 *<Credits to> Rodrigo Diaz </Credits to>
 */

public class ParticleGenerator : MonoBehaviour {

    public float particleSizeMultiplier = 1.0f;
    public float particleLifetime = 3.0f;
    //Is there a initial force the particles should have?
    public Vector3 particleForce = Vector3.zero;
    //If no state is set the default particle is water. 
    public LiquidParticle.LiquidStates particlesState = LiquidParticle.LiquidStates.Water;
    // How much time until the next particle spawns
    private const float SPAWN_INTERVAL=0.025f;
    //Container to keep all the particles in a single object within the unity heirarchy
    private Transform m_sceneParticleHolder;
    private float m_spawnTimer = 0.0f;

    /*
     *<summary>
     *  Pseudo constructor
     *</summary>
     */
    void Awake() {
        //Reset spawn timer
        m_spawnTimer = 0.0f;
        m_sceneParticleHolder = new GameObject("ParticleHolder").transform;
    }

    /*
     *<summary>
     * Creates particles given an area
     *</summary>
     */
    public void Fill(float x, float y, float w, float h)
    {
        float offset = 0.25f;
        float startX = x - (w * 0.5f) + offset;
        float startY = y - (h * 0.5f) + offset;

        for (float i = startX; i < startX + w - offset; i += offset)
        {
            for (float j = startY; j < startY + h - offset; j += offset)
            {
                GameObject newLiquidParticle = (GameObject)Instantiate(Resources.Load("LiquidPhysics/LiquidParticle"));
                newLiquidParticle.transform.position = new Vector2(i, j);

                LiquidParticle particleScript = newLiquidParticle.GetComponent<LiquidParticle>();
                particleScript.SetImmortal();
                particleScript.SetMaxSize();
                particleScript.SetState(particlesState);
                particleScript.particleSizeMultiplier = particleSizeMultiplier;
                newLiquidParticle.transform.SetParent(m_sceneParticleHolder);
            }
        }
    }

    /*
     *<summary>
     *  Update loop called each tick as part of monobehavior
     *</summary>
     */
    void Update() {
        m_spawnTimer += Time.deltaTime;
		if (m_spawnTimer >= SPAWN_INTERVAL ){ 
            //Create a new particle
			GameObject newLiquidParticle=(GameObject)Instantiate(Resources.Load("LiquidPhysics/LiquidParticle")); //Spawn a particle
            newLiquidParticle.transform.position = transform.position;// Relocate to the spawner position
            newLiquidParticle.GetComponent<Rigidbody2D>().AddForce(particleForce); 
            //Configure the new particle
			LiquidParticle particleScript = newLiquidParticle.GetComponent<LiquidParticle>(); // Get the particle script
			particleScript.SetLifeTime(particleLifetime); //Set each particle lifetime
			particleScript.SetState(particlesState); //Set the particle State		
            particleScript.particleSizeMultiplier = particleSizeMultiplier;
            particleScript.SetMaxSize();
            //Keep the scene tidy and add the particle to the container transform
            newLiquidParticle.transform.SetParent(m_sceneParticleHolder);
            //Reset spawn timer
            m_spawnTimer = 0.0f;	
		}		
	}
}
