using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Swinging : MonoBehaviour
{
    [Header("References")]
    public Transform gunTip, cam, player;
    public LayerMask whatIsGrappleable;
    public PlayerController pm;

    [Header("Swinging")]
    private float maxSwingDistance = 25f;
    public Vector3 swingPoint;
    public SpringJoint joint;
    public float swingCooldown;
    public float swingCooldownReset;
    public bool canSwing = true;

    [Header("OdmGear")]
    public Transform orientation;
    public Rigidbody rb;
    public float horizontalThrustForce;
    public float forwardThrustForce;
    public float extendCableSpeed;

    [Header("Prediction")]
    public RaycastHit predictionHit;
    public float predictionSphereCastRadius;
    public Transform predictionPoint;

    [Header("Input")]
    public KeyCode swingKey = KeyCode.Mouse0;


    private void Update()
    {
        if (!Input.GetKey(KeyCode.LeftShift))
        {
            if (Input.GetKeyDown(swingKey) && canSwing) StartSwing();
            if (Input.GetKeyUp(swingKey))
            {
                swingCooldown = swingCooldownReset;
                canSwing = false;
                StopSwing(); 
            }

            if (!canSwing)
            {
                swingCooldown -= Time.deltaTime;
                if (swingCooldown <= 0)
                {
                    canSwing = true;
                }
            }
        }

        CheckForSwingPoints();

        if (joint != null) OdmGearMovement();
    }

    private void CheckForSwingPoints()
    {
        if (joint != null) return;

        RaycastHit sphereCastHit;
        Physics.SphereCast(cam.position, predictionSphereCastRadius, cam.forward,
                            out sphereCastHit, maxSwingDistance, whatIsGrappleable);

        RaycastHit raycastHit;
        Physics.Raycast(cam.position, cam.forward,
                            out raycastHit, maxSwingDistance, whatIsGrappleable);

        Vector3 realHitPoint;

        // Option 1 - Direct Hit
        if (raycastHit.point != Vector3.zero)
            realHitPoint = raycastHit.point;

        // Option 2 - Indirect (predicted) Hit
        else if (sphereCastHit.point != Vector3.zero)
            realHitPoint = sphereCastHit.point;

        // Option 3 - Miss
        else
            realHitPoint = Vector3.zero;

        // realHitPoint found
        if (realHitPoint != Vector3.zero)
        {
            predictionPoint.gameObject.SetActive(true);
            predictionPoint.position = realHitPoint;
        }
        // realHitPoint not found
        else
        {
            predictionPoint.gameObject.SetActive(false);
        }

        predictionHit = raycastHit.point == Vector3.zero ? sphereCastHit : raycastHit;
    }


    private void StartSwing()
    {
        // return if predictionHit not found
        if (predictionHit.point == Vector3.zero) return;
        if (predictionHit.point.y < pm.transform.position.y) return;

        pm.ResetRestrictions();

        pm.swinging = true;

        swingPoint = predictionHit.point;
        joint = player.gameObject.AddComponent<SpringJoint>();
        joint.autoConfigureConnectedAnchor = false;
        joint.connectedAnchor = swingPoint;

        float distanceFromPoint = Vector3.Distance(player.position, swingPoint);

        // the distance grapple will try to keep from grapple point. 
        joint.maxDistance = distanceFromPoint * 0.8f;
        joint.minDistance = distanceFromPoint * 0.25f;

        // customize values as you like
        joint.spring = 4.5f;
        joint.damper = 7f;
        joint.massScale = 4.5f;
    }

    public void StopSwing()
    {
        pm.swinging = false;

        Destroy(joint);
    }

    private void OdmGearMovement()
    {
        if (Input.GetKey(KeyCode.LeftShift)) StopSwing();

        if (predictionHit.point.y < pm.transform.position.y) StopSwing();
        // right
        if (Input.GetKey(KeyCode.D)) rb.AddForce(orientation.right * horizontalThrustForce * Time.deltaTime);
        // left
        if (Input.GetKey(KeyCode.A)) rb.AddForce(-orientation.right * horizontalThrustForce * Time.deltaTime);

        // forward
        if (Input.GetKey(KeyCode.W)) rb.AddForce(orientation.forward * horizontalThrustForce * Time.deltaTime);

        // shorten cable
        if (Input.GetKey(KeyCode.Space))
        {
            Vector3 directionToPoint = swingPoint - transform.position;
            rb.AddForce(directionToPoint.normalized * forwardThrustForce * Time.deltaTime);

            float distanceFromPoint = Vector3.Distance(transform.position, swingPoint);

            joint.maxDistance = distanceFromPoint * 0.8f;
            joint.minDistance = distanceFromPoint * 0.25f;
        }
        // extend cable
        if (Input.GetKey(KeyCode.S))
        {
            float extendedDistanceFromPoint = Vector3.Distance(transform.position, swingPoint) + extendCableSpeed;

            joint.maxDistance = extendedDistanceFromPoint * 0.8f;
            joint.minDistance = extendedDistanceFromPoint * 0.25f;
        }
    }
}
