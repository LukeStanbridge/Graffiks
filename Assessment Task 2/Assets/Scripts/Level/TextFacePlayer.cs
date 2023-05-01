using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class TextFacePlayer : MonoBehaviour
{
    private void Start()
    {
        transform.DOMoveY(6, 1.5f).SetLoops(-1, LoopType.Yoyo).SetEase(Ease.InOutSine);
    }
    void Update()
    {
        transform.LookAt(Camera.main.transform.position);
        transform.Rotate(new Vector3(0, 180, 0));
    }
}
