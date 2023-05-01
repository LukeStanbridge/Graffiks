using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GrapplingRope : MonoBehaviour
{
    private Spring spring;
    private Vector3 currentGrapplePosition;
    private LineRenderer lr;
    public Swinging swinging;
    public int quality;
    public float damper;
    public float strength;
    public float velocity;
    public float waveCount;
    public float waveHeight;
    public AnimationCurve affectCurve;

    private void Awake()
    {
        lr = GetComponent<LineRenderer>();
        spring = new Spring();
        spring.SetTarget(0);
    }

    private void LateUpdate()
    {
        DrawRope();
    }

    //draw fancy rope
    private void DrawRope()
    {
        // if not grappling, don't draw rope
        if (!swinging.joint)
        {
            currentGrapplePosition = swinging.gunTip.position;
            spring.Reset();
            if (lr.positionCount > 0) lr.positionCount = 0;
            return;
        }

        if (lr.positionCount == 0)
        {
            spring.SetVelocity(velocity);
            lr.positionCount = quality + 1;
        }

        spring.SetDamper(damper);
        spring.SetStrength(strength);
        spring.Update();

        var grapplePoint = swinging.swingPoint;
        var gunTipPosition = swinging.gunTip.position;
        var up = Quaternion.LookRotation((grapplePoint - gunTipPosition).normalized) * Vector3.up;

        currentGrapplePosition = Vector3.Lerp(currentGrapplePosition, swinging.swingPoint, Time.deltaTime * 8f);

        for(var i =0; i < quality + 1; i++)
        {
            var delta = i / (float) quality;
            var offset = up * waveHeight * Mathf.Sin(delta * waveCount * Mathf.PI * spring.Value * affectCurve.Evaluate(delta));

            lr.SetPosition(i, Vector3.Lerp(gunTipPosition, currentGrapplePosition, delta) + offset);
        }

        //lr.SetPosition(0, swinging.gunTip.position);
        //lr.SetPosition(1, currentGrapplePosition);
    }
}
