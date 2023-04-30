using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class DancingAI : MonoBehaviour
{
    //assignables
    public Ragdoll ragdoll;
    public Button whatDoing;
    public Button stopDancing;
    public Button forReal;
    public Button weirdo;
    public TextMeshProUGUI problem;
    public TextMeshProUGUI goal;
    private Vector3 PosReset;
    public bool startGame = false;

    //patrolling
    private Animator anim;

    private void Awake()
    {
        anim = gameObject.GetComponent<Animator>();
        anim.Play("Hip Hop Dancing");
        PosReset = transform.position;
    }

    private void Update()
    {
    }

    public void Ragdoll()
    {
        ragdoll.ToggleRagdoll(true);

        DestroyEnemy();
    }

    private void DestroyEnemy()
    {
        Destroy(gameObject, 4f);
    }
    
    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, 20);
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == LayerMask.NameToLayer("WhatIsPlayer"))
        {
            Cursor.lockState = CursorLockMode.None;
            Cursor.visible = true;
            stopDancing.gameObject.SetActive(true);
            whatDoing.gameObject.SetActive(true);
            forReal.gameObject.SetActive(true);
            weirdo.gameObject.SetActive(true);
            whatDoing.onClick.AddListener(MainQuest);
            stopDancing.onClick.AddListener(StopDancing);
            forReal.onClick.AddListener(ForReal);
            weirdo.onClick.AddListener(Weirdo);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.layer == LayerMask.NameToLayer("WhatIsPlayer"))
        {
            Cursor.lockState = CursorLockMode.Locked;
            Cursor.visible = false;
            stopDancing.gameObject.SetActive(false);
            whatDoing.gameObject.SetActive(false);
            forReal.gameObject.SetActive(false);
            weirdo.gameObject.SetActive(false);
            problem.gameObject.SetActive(false);
            goal.gameObject.SetActive(false);
        }
    }

    private void MainQuest()
    {
        problem.gameObject.SetActive(true);
        goal.gameObject.SetActive(true);
        startGame = true;
    }

    private void StopDancing()
    {
        transform.position = PosReset;
        anim.CrossFadeInFixedTime("Dancing", 0.4f);
    }
    private void ForReal()
    {
        transform.position = PosReset;
        anim.CrossFadeInFixedTime("Dancing Twerk", 0.4f);
    }
    private void Weirdo()
    {
        transform.position = PosReset;
        anim.CrossFadeInFixedTime("Silly Dancing", 0.4f);
    }
}
