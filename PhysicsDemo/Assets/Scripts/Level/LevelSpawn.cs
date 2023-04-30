using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelSpawn : MonoBehaviour
{
    [SerializeField] private Transform spawnPoint;
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == LayerMask.NameToLayer("WhatIsPlayer"))
        {
            other.transform.position = spawnPoint.position;
        }
    }
}
