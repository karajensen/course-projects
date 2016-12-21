using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class Controller2D : MonoBehaviour
{
    public class RayCastData
    {
        public RayCastData(RaycastHit2D r, Vector2 s, Vector2 d)
        {
            ray = r;
            start = s;
            direction = d;
        }

        public RaycastHit2D ray;
        public Vector2 start;
        public Vector2 direction;
    };

    ParticleGenerator _waterGunScript;
    SpriteRenderer _renderer;
    float _minDistanceToWall = 0.1f;
    float _minDistanceForSlope = 0.1f;
    float _minDistanceToGround = 0.1f;
    float _slopeAdjustment = 0.1f;
    bool _jumping = false;
    bool _jumpReleased = false;
    float _jumpTimer = 0.0f;
    int _mask = 0;

    static int _minHorizontalRays = 1;
    static int _minVerticalRays = 2;
    static int _maxRays = 6;

    public GameObject waterGun;
    public List<RayCastData> _raycastsThisFrame = new List<RayCastData>();
    public int _totalHorizontalRays = _minHorizontalRays;
    public int _totalVerticalRays = _minVerticalRays;
    public bool _grounded = false;
    public float _gravity = 0.04f;
    public float _waterGunOffset = 2.0f;
    public float _waterGunPower = 100.0f;
    public float _movementAmount = 0.01f;
    public float _slopeLimit = 0.9f;
    public float _jumpDuration = 0.2f;

    void Start()
    {
        _mask = LayerMask.GetMask("Default");
        _waterGunScript = waterGun.GetComponent<ParticleGenerator>();
        _renderer = gameObject.GetComponent<SpriteRenderer>();
        moveHorizontal(false);
    }

    bool IsGrounded()
    {
        return _grounded;
    }

    void DrawDiagnostics()
    {
        foreach (var data in _raycastsThisFrame)
        {
            if (data.ray.collider != null)
            {
                Debug.DrawLine(data.start,
                    data.start + (data.direction * data.ray.distance), 
                    Color.green);

                float size = 0.025f;
                Vector2 topLeft = data.ray.point + (Vector2.up * size) + (Vector2.left * size);
                Vector2 topRight = data.ray.point + (Vector2.up * size) + (Vector2.right * size);
                Vector2 botLeft = data.ray.point + (Vector2.down * size) + (Vector2.left * size);
                Vector2 botRight = data.ray.point + (Vector2.down * size) + (Vector2.right * size);

                Debug.DrawLine(topLeft, topRight, Color.green);
                Debug.DrawLine(topRight, botRight, Color.green);
                Debug.DrawLine(botRight, botLeft, Color.green);
                Debug.DrawLine(botLeft, topLeft, Color.green);
            }
            else
            {
                const float length = 5.0f;
                Debug.DrawLine(data.start,
                    data.start + (data.direction * length), 
                    Color.red);
            }
        }
    }

    void moveHorizontal(bool movingLeft)
    {
        bool canMove = false;
        bool isGunActive = true;
        bool canSlopeAdjust = false;
        bool slopAdjustDown = false;

        const float castOffset = 0.01f;
        float width = _renderer.bounds.size.x;
        float height = _renderer.bounds.size.y;
        float halfHeight = _renderer.bounds.size.y * 0.5f;
        float halfWidth = width * 0.5f;

        float xOffset = movingLeft ? -halfWidth - castOffset : halfWidth + castOffset;
        float yOffset = _totalHorizontalRays == 2 ? height : height / _totalHorizontalRays;
        Vector2 direction = movingLeft ? Vector2.left : Vector2.right;
        Vector2 start = new Vector2(
            _renderer.bounds.center.x + xOffset,
            _renderer.bounds.center.y - (_totalHorizontalRays > 1 ? halfHeight : 0));
        
        for (int i = 0; i < _totalHorizontalRays; ++i)
        {
            Vector2 pt = new Vector2(start.x, start.y + (yOffset * i));

            RaycastHit2D hit = Physics2D.Raycast(pt, direction, Mathf.Infinity, _mask);
            _raycastsThisFrame.Add(new RayCastData(hit, pt, direction));

            if (hit.collider == null || (hit.collider != null && hit.distance >= _minDistanceToWall))
            {
                canMove = true;
            }

            if (hit.collider != null)
            {
                isGunActive = !(hit.distance < halfWidth);
            }

            if (hit.collider != null && hit.distance < _minDistanceForSlope)
            {
                // Determine movement for slopes
                float dot = Vector2.Dot(hit.normal.normalized, direction);
                if (Math.Abs(dot) < _slopeLimit)
                {
                    canSlopeAdjust = true;
                    slopAdjustDown = dot > 0.0f;
                }
            }
        }

        if (canMove)
        {
            transform.localPosition = new Vector3(
                transform.localPosition.x + (movingLeft ? -_movementAmount : _movementAmount),
                transform.localPosition.y,
                transform.localPosition.z);
        }

        if (canSlopeAdjust)
        {
            transform.localPosition = new Vector3(
                transform.localPosition.x,
                transform.localPosition.y + (slopAdjustDown ? -_slopeAdjustment : _slopeAdjustment),
                transform.localPosition.z);
        }

        waterGun.SetActive(isGunActive);
        _waterGunScript.particleForce = new Vector3(
            movingLeft ? -_waterGunPower : _waterGunPower, 0.0f, 0.0f);
        waterGun.transform.localPosition = new Vector3(
            movingLeft ? -_waterGunOffset : _waterGunOffset, 0.0f, 0.0f);
    }

    void moveVertical()
    {
        float width = _renderer.bounds.size.x;
        float height = _renderer.bounds.size.y;
        float halfHeight = height * 0.5f;
        float halfWidth = width * 0.5f;
        const float castOffset = 0.01f;
        float xOffset = _totalVerticalRays == 2 ? width : width / _totalVerticalRays;
        float roofDistance = Mathf.Infinity;
        float groundDistance = Mathf.Infinity;

        Vector2 startDown = new Vector2(
            _renderer.bounds.center.x - halfWidth,
            _renderer.bounds.center.y - halfHeight - castOffset);

        Vector2 startUp = new Vector2(
            _renderer.bounds.center.x - halfWidth,
            _renderer.bounds.center.y + halfHeight + castOffset);

        for (int i = 0; i < _totalVerticalRays; ++i)
        {
            Vector2 down = new Vector2(startDown.x + (xOffset * i), startDown.y);
            Vector2 up = new Vector2(startUp.x + (xOffset * i), startUp.y);

            RaycastHit2D groundHit = Physics2D.Raycast(down, Vector2.down, Mathf.Infinity, _mask);
            groundDistance = Math.Min(groundDistance, groundHit.collider != null ? groundHit.distance : Mathf.Infinity);
            _raycastsThisFrame.Add(new RayCastData(groundHit, down, Vector2.down));

            RaycastHit2D roofHit = Physics2D.Raycast(up, Vector2.up, Mathf.Infinity, _mask);
            roofDistance = Math.Min(roofDistance, roofHit.collider != null ? roofHit.distance : Mathf.Infinity);
            _raycastsThisFrame.Add(new RayCastData(roofHit, up, Vector2.up));

            _grounded = groundDistance < _minDistanceToGround;
        }

        if (_jumping)
        {
            // Apply jump to character
            _jumpTimer += Time.deltaTime;
            if(_jumpTimer > _jumpDuration || roofDistance < _minDistanceToGround)
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
        _totalVerticalRays = Math.Min(Math.Max(
            _minVerticalRays, _totalVerticalRays), _maxRays);

        _totalHorizontalRays = Math.Min(Math.Max(
            _minHorizontalRays, _totalHorizontalRays), _maxRays);

        _raycastsThisFrame.Clear();

        moveVertical();

        if (Input.GetKey(KeyCode.LeftArrow))
        {
            moveHorizontal(true);
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            moveHorizontal(false);
        }

        if (!_jumpReleased)
        {
            _jumpReleased = !Input.GetKey(KeyCode.Space);
        }
        else if (_jumpReleased && Input.GetKey(KeyCode.Space) && _grounded)
        {
            _jumpReleased = false;
            _jumping = true;
            _jumpTimer = 0.0f;
        }

        DrawDiagnostics();
    }
}
