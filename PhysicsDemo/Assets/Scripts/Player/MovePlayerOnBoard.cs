using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePlayerOnBoard : MonoBehaviour
{
    public Transform playerPosition;

    private void Update()
    {
        transform.position = playerPosition.position;
    }
}
