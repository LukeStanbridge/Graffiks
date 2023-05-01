using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using UnityEngine;
using UnityEngine.UI;

public class TurretAI : MonoBehaviour
{
    public Transform player;
    public Transform AttackPoint;
    public Slider healthBar;
    public LevelSetup levelSetup;

    public LayerMask whatIsGround, whatIsPlayer;

    public float health;

    //Attacking
    public float timeBetweenAttacks;
    bool alreadyAttacked;
    public GameObject projectile;
    public GameObject cannon;
    public GameObject explode;

    //States
    public float attackForce;
    public float attackRange;
    public bool playerInAttackRange;

    private void Awake()
    {
        player = GameObject.Find("PlayerObj").transform;
        healthBar.value = health;
    }

    private void Update()
    {
        //Check for sight and attack range
        playerInAttackRange = Physics.CheckSphere(transform.position, attackRange, whatIsPlayer);
        if (playerInAttackRange && player.transform.position.y > (transform.position.y - 2.5)) AttackPlayer();
        healthBar.transform.LookAt(player.position);
    }

    private void AttackPlayer()
    {
        //Look at player
        cannon.transform.LookAt(player);

        if (!alreadyAttacked)
        {
            ///Attack code here
            Rigidbody rb = Instantiate(projectile, AttackPoint.position, Quaternion.identity).GetComponent<Rigidbody>();
            rb.AddForce(AttackPoint.transform.forward * attackForce, ForceMode.Impulse);

            alreadyAttacked = true;
            Invoke(nameof(ResetAttack), timeBetweenAttacks);
        }
    }
    private void ResetAttack()
    {
        alreadyAttacked = false;
    }

    public void TakeDamage(int damage)
    {
        health -= damage;
        healthBar.value -= damage;

        if (health <= 0)
        {
            levelSetup.level1Complete = true;
            levelSetup.StartParticles();
            Invoke(nameof(DestroyEnemy), 0.2f);
        }
    }
    private void DestroyEnemy()
    {
        Instantiate(explode, transform.position, Quaternion.identity);
        Destroy(gameObject);
    }

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, attackRange);
    }
}
