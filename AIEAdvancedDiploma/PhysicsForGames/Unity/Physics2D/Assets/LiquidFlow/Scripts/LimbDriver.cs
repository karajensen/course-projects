using UnityEngine;
using System.Collections;

public class LimbDriver : MonoBehaviour
{
    public Rigidbody2D leftLeg = null;
    public Rigidbody2D rightLeg = null;
    public Rigidbody2D leftArm = null;
    public Rigidbody2D rightArm = null;
    public Color normalColor;
    public Color selectedColor;

    private SpriteRenderer leftLegRenderer = null;
    private SpriteRenderer rightLegRenderer = null;
    private SpriteRenderer rightArmRenderer = null;
    private SpriteRenderer leftArmRenderer = null;

    public Rigidbody2D body = null;
    private float forceUpMultiplier = 5.0f;
    private float forceSideMultiplier = 2.5f;

    void Start()
    {
        leftLegRenderer = leftLeg.gameObject.GetComponent<SpriteRenderer>();
        rightLegRenderer = rightLeg.gameObject.GetComponent<SpriteRenderer>();
        leftArmRenderer = leftArm.gameObject.GetComponent<SpriteRenderer>();
        rightArmRenderer = rightArm.gameObject.GetComponent<SpriteRenderer>();
    }

	void Update ()
    {
        leftLegRenderer.color = normalColor;
        rightLegRenderer.color = normalColor;
        leftArmRenderer.color = normalColor;
        rightArmRenderer.color = normalColor;

        if (Input.GetKey("q"))
        {
            Vector2 force = Vector2.up * forceUpMultiplier + Vector2.left * forceSideMultiplier;
            leftArm.AddForce(force);
            body.AddForce(force);
            leftArmRenderer.color = selectedColor;
        }
        if (Input.GetKey("w"))
        {
            Vector2 force = Vector2.up * forceUpMultiplier + Vector2.right * forceSideMultiplier;
            rightArm.AddForce(force);
            body.AddForce(force);
            rightArmRenderer.color = selectedColor;
        }
        if (Input.GetKey("o"))
        {
            Vector2 force = Vector2.up * forceUpMultiplier + Vector2.left * forceSideMultiplier;
            leftLeg.AddForce(force);
            body.AddForce(force);
            leftLegRenderer.color = selectedColor;
        }
        if (Input.GetKey("p"))
        {
            Vector2 force = Vector2.up * forceUpMultiplier + Vector2.right * forceSideMultiplier;
            rightLeg.AddForce(force);
            body.AddForce(force);
            rightLegRenderer.color = selectedColor;
        }
    }
}
