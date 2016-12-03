using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class Controller2D : MonoBehaviour
{
    public new Transform transform;
    public BoxCollider2D boxCollider;
    public Rigidbody2D rigidBody2D;
    private SpriteRenderer _renderer;
    float _gravity = 0.1f;
    bool _grounded = false;

    bool IsGrounded() { return _grounded;  }

    void Start()
    {
        _renderer = gameObject.GetComponent<SpriteRenderer>();
    }

    void moveHorizontal(bool movingLeft)
    {
        // Check to make sure no colliding, if so, allow movement
        const float movementAmount = 0.01f;
        transform.localPosition = new Vector3(
            transform.localPosition.x + (movingLeft ? -movementAmount : movementAmount),
            transform.localPosition.y,
            transform.localPosition.z);

        // TODO: Slopes
        
        
    }

    void moveVertical()
    {
        // Determine whether colliding with the ground
        _grounded = false;
        Vector2 bottom = new Vector2(
            _renderer.bounds.center.x,
            _renderer.bounds.center.y - _renderer.bounds.size.y);

        RaycastHit2D hit = Physics2D.Raycast(bottom, Vector2.down);
        if(hit.collider != null)
        {
            const float minDistanceGrounded = 0.01f;
            _grounded = hit.distance < minDistanceGrounded;
            if(!_grounded)
            {
                // Apply gravity to character
                transform.localPosition = new Vector3(
                    transform.localPosition.x,
                    transform.localPosition.y - _gravity,
                    transform.localPosition.z);
            }
        }
    }
       
    void Update()
    {
        moveVertical();

        // Determine movememnt
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            moveHorizontal(true);
        }
        if (Input.GetKey(KeyCode.RightArrow))
        {
            moveHorizontal(false);
        }
    }
}
