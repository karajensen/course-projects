using UnityEngine;
using System.Collections;

public class SpriteToParticles : MonoBehaviour
{
    public ParticleGenerator particleSource = null;

	void Start ()
    {
        var renderer = gameObject.GetComponent<SpriteRenderer>();

        particleSource.Fill(
            renderer.bounds.center.x,
            renderer.bounds.center.y,
            renderer.bounds.size.x,
            renderer.bounds.size.y);

        gameObject.GetComponent<SpriteRenderer>().enabled = false;
    }
}
