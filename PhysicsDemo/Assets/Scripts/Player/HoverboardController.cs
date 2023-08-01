using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HoverboardController : MonoBehaviour
{
    private Rigidbody hoverboardRb;
    [SerializeField] private float multiplier;
    [SerializeField] private float moveForce, turnTorque;
    public Transform[] anchors = new Transform[4];
    private RaycastHit[] hits = new RaycastHit[4];

    private void Start()
    {
        hoverboardRb = GetComponent<Rigidbody>();
    }

    private void FixedUpdate()
    {
        for (int i = 0; i < 4; i++)
        {
            ApplyForce(anchors[i], hits[i]);
        }

        hoverboardRb.AddForce(Input.GetAxis("Vertical") * moveForce * transform.forward);
        hoverboardRb.AddTorque(Input.GetAxis("Horizontal") * turnTorque * transform.up);

        if (Input.GetKey(KeyCode.Space))
        {
            multiplier += 2;
        }

        if (Input.GetKey(KeyCode.LeftShift) && multiplier >= 2.4f) 
        {
            multiplier -= 2;
            if (multiplier < 2.4f) multiplier = 2.4f;
        }
    }

    private void ApplyForce(Transform anchor, RaycastHit hit)
    {
        if (Physics.Raycast(anchor.position, -anchor.up, out hit))
        {
            float force = 0;
            force = Mathf.Abs(1 / (hit.point.y - anchor.position.y));
            hoverboardRb.AddForceAtPosition(transform.up * force * multiplier, anchor.position, ForceMode.Acceleration);
        }
    }
}
