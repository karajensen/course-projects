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
     *  Update loop called each tick as part of monobehavior
     *</summary>
     */
    void Update() {
        m_spawnTimer += Time.deltaTime;
		if (m_spawnTimer >= SPAWN_INTERVAL ){ 
            //Create a new particle
			GameObject newLiquidParticle=(GameObject)Instantiate(Resources.Load("LiquidPhysics/LiquidParticle")); //Spawn a particle
			Vector3 startPos = new Vector3(transform.position.x + Random.Range(-0.25f,0.25f),transform.position.y + Random.Range(-0.25f,0.25f),transform.position.z);

            newLiquidParticle.transform.position = startPos;// Relocate to the spawner position
            newLiquidParticle.GetComponent<Rigidbody>().AddForce(particleForce);
            newLiquidParticle.GetComponent<SphereCollider>().radius = 0.125f;

            //Configure the new particle
            LiquidParticle particleScript = newLiquidParticle.GetComponent<LiquidParticle>(); // Get the particle script
			particleScript.SetLifeTime(particleLifetime); //Set each particle lifetime
			particleScript.SetState(particlesState); //Set the particle State		
            //Keep the scene tidy and add the particle to the container transform
            newLiquidParticle.transform.SetParent(m_sceneParticleHolder);
            //Reset spawn timer
            m_spawnTimer = 0.0f;	
		}		
	}
}
