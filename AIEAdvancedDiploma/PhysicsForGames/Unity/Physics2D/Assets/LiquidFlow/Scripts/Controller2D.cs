using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class Controller2D : MonoBehaviour
{
    public GameObject waterGun;
    private ParticleGenerator _waterGunScript;
    private SpriteRenderer _renderer;
    float _gravity = 0.04f;
    bool _grounded = false;
    float _minDistanceToWall = 0.1f;
    float _minDistanceForSlope = 0.1f;  
    float _minDistanceToGround = 0.1f;
    float _movementAmount = 0.01f;
    float _waterGunOffset = 2.0f;
    float _waterGunPower = 100.0f;
    bool _jumping = false;
    float _jumpTimer = 0.0f;
    int _mask = 0;

    bool IsGrounded() { return _grounded;  }

    void Start()
    {
        _mask = LayerMask.GetMask("Default");
        _waterGunScript = waterGun.GetComponent<ParticleGenerator>();
        _renderer = gameObject.GetComponent<SpriteRenderer>();
        moveHorizontal(false);
    }

    bool isValidCollider(Collider2D collider)
    {
        return collider != null;
    }

    void moveHorizontal(bool movingLeft)
    {
        const float castOffset = 0.01f;
        float width = _renderer.bounds.size.x;
        float halfHeight = _renderer.bounds.size.y * 0.5f;
        float halfWidth = width * 0.5f;

        float xOffset = movingLeft ? -halfWidth - castOffset : halfWidth + castOffset;
        Vector2 pt = new Vector2(
            _renderer.bounds.center.x + xOffset,
            _renderer.bounds.center.y);
        Vector2 direction = movingLeft ? Vector2.left : Vector2.right;

        RaycastHit2D hit = Physics2D.Raycast(pt, direction, Mathf.Infinity, _mask);
        if (hit.collider == null || (isValidCollider(hit.collider) && hit.distance >= _minDistanceToWall))
        {
            transform.localPosition = new Vector3(
                transform.localPosition.x + (movingLeft ? -_movementAmount : _movementAmount),
                transform.localPosition.y,
                transform.localPosition.z);
        }

        bool isGunActive = true;
        if (isValidCollider(hit.collider))
        {
            isGunActive = !(hit.distance < halfWidth);

            if(hit.distance < _minDistanceForSlope)
            {
                // Determine movement for slopes
                float dot = Vector2.Dot(hit.normal.normalized, direction);
                const float maxSlopeCanTravel = 0.9f;
                const float slopeAdjustment = 0.2f;

                if (Math.Abs(dot) < maxSlopeCanTravel)
                {
                    transform.localPosition = new Vector3(
                        transform.localPosition.x,
                        transform.localPosition.y + (-dot * slopeAdjustment),
                        transform.localPosition.z);
                }
                else
                {
                    // Ensure the slope isn't a small ledge
                    pt.y += halfHeight;
                    RaycastHit2D hitSlope = Physics2D.Raycast(pt, direction, Mathf.Infinity, _mask);
                    if (isValidCollider(hitSlope.collider))
                    {
                        dot = Vector2.Dot(hitSlope.normal.normalized, direction);
                        if (Math.Abs(dot) < maxSlopeCanTravel)
                        {
                            transform.localPosition = new Vector3(
                                transform.localPosition.x,
                                transform.localPosition.y + (-dot * slopeAdjustment),
                                transform.localPosition.z);
                        }
                    }
                }
            }
        }

        waterGun.SetActive(isGunActive);
        _waterGunScript.particleForce = new Vector3(
            movingLeft ? -_waterGunPower : _waterGunPower, 0.0f, 0.0f);
        waterGun.transform.localPosition = new Vector3(
            movingLeft ? -_waterGunOffset : _waterGunOffset, 0.0f, 0.0f);
    }

    void moveVertical()
    {
        float halfHeight = _renderer.bounds.size.y * 0.5f;
        float halfWidth = _renderer.bounds.size.x * 0.5f;
        const float castOffset = 0.01f;

        // Determine whether colliding with the ground/roof
        Vector2 downLeft = new Vector2(
            _renderer.bounds.center.x - halfWidth,
            _renderer.bounds.center.y - halfHeight - castOffset);

        Vector2 downRight = new Vector2(
            _renderer.bounds.center.x + halfWidth,
            _renderer.bounds.center.y - halfHeight - castOffset);

        Vector2 topLeft = new Vector2(
            _renderer.bounds.center.x - halfWidth,
            _renderer.bounds.center.y + halfHeight + castOffset);

        Vector2 topRight = new Vector2(
            _renderer.bounds.center.x + halfWidth,
            _renderer.bounds.center.y + halfHeight + castOffset);

        RaycastHit2D groundHit = Physics2D.Raycast(downLeft, Vector2.down, Mathf.Infinity, _mask);
        float groundDistance = isValidCollider(groundHit.collider) ? groundHit.distance : Mathf.Infinity;
        groundHit = Physics2D.Raycast(downRight, Vector2.down, Mathf.Infinity, _mask);
        groundDistance = Math.Min(groundDistance, isValidCollider(groundHit.collider) ? groundHit.distance : Mathf.Infinity);
        _grounded = groundDistance < _minDistanceToGround;

        RaycastHit2D roofHit = Physics2D.Raycast(topLeft, Vector2.up, Mathf.Infinity, _mask);
        float roofDistance = isValidCollider(roofHit.collider) ? roofHit.distance : Mathf.Infinity;
        roofHit = Physics2D.Raycast(topRight, Vector2.up, Mathf.Infinity, _mask);
        roofDistance = Math.Min(roofDistance, isValidCollider(roofHit.collider) ? roofHit.distance : Mathf.Infinity);

        if (_jumping)
        {
            _jumpTimer += Time.deltaTime;
            const float jumpDuration = 0.15f;
            if(_jumpTimer > jumpDuration || roofDistance < _minDistanceToGround)
            {
                _jumping = false;
            }
            else
            {
                transform.localPosition = new Vector3(
                    transform.localPosition.x,
                    transform.localPosition.y + _gravity,
                    transform.localPosition.z);
            }
        }
        else if (!_grounded)
        {
            // Apply gravity to character
            transform.localPosition = new Vector3(
                transform.localPosition.x,
                transform.localPosition.y - Math.Min(groundDistance, _gravity),
                transform.localPosition.z);
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

        if (Input.GetKey(KeyCode.Space) && _grounded)
        {
            _jumping = true;
            _jumpTimer = 0.0f;
        }
    }
}
