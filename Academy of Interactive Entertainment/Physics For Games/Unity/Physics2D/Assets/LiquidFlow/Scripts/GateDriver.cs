using UnityEngine;
using System.Collections;

public class GateDriver : MonoBehaviour
{
    public GameObject cog = null;
    private SliderJoint2D joint = null;
    private float velocityMultiplier = 0.01f;
    private float gateShutSpeed = -2.0f;
    private float minOpenSpeed = 1.0f;

    void Start()
    {
        joint = gameObject.GetComponent<SliderJoint2D>();
    }

	void Update ()
    {
        float velocity = cog.GetComponent<Rigidbody2D>().angularVelocity;

        JointMotor2D motor = new JointMotor2D();
        motor.maxMotorTorque = joint.motor.maxMotorTorque;
        motor.motorSpeed = velocity < minOpenSpeed ? gateShutSpeed : velocity * velocityMultiplier;

        joint.motor = motor;
    }
}
