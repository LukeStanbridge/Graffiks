using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Respawn : MonoBehaviour
{
    public GameObject player;
    public Transform respawn;

    public void OnCollisionEnter(Collision collision)
    {
        if (collision.collider == player.gameObject.GetComponent<Collider>())
        {
            player.transform.position = respawn.transform.position; 
        }
    }
}
