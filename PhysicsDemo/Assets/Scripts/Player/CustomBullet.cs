using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomBullet : MonoBehaviour
{
    //assignables
    public Rigidbody rb;
    public GameObject explosion;
    public LayerMask whatIsEnemies;
    public GameObject ignore;

    //stats
    [Range(0f, 1f)]
    public float bounciness;
    public bool useGravity;

    //damage
    public int explosionDamage;
    public float explosionRange;
    public float explosionForce;
    public float explosionUpwards;

    //lifetime
    public int maxCollisions;
    public float maxLifetime;
    public bool explodeOnTouch = true;
    //public bool explodeOnImpact = true;

    public int collisons;
    private PhysicMaterial physicsMat;

    private void Start()
    {
        Setup();
    }

    private void Update()
    {
        ////when to explode
        //if (collisons > maxCollisions) Explode();

        //count down lifetime when not in slow mo
        if (!Input.GetKey(KeyCode.LeftShift))
        {
            //maxLifetime = maxLifetime * 10f;
            maxLifetime -= Time.deltaTime;
            if (maxLifetime <= 0f) Explode();
        }
    }

    private void Explode()
    {
        //instantiate explosion
        if (explosion != null) Instantiate(explosion, transform.position, Quaternion.identity);

        //Disable visuals on impact
        this.GetComponent<MeshRenderer>().enabled = false;
        this.GetComponent<TrailRenderer>().enabled = false;

        //check for enemies
        Collider[] enemies = Physics.OverlapSphere(transform.position, explosionRange, whatIsEnemies);
        for (int i = 0; i < enemies.Length; i++)
        {
            //Get component of enemy and call Take Damage
            if (enemies[i].GetComponent<TurretAI>()) enemies[i].GetComponent<TurretAI>().TakeDamage(explosionDamage); // damage turrets
            if (enemies[i].GetComponent<PlayerController>()) enemies[i].GetComponent<PlayerController>().TakeDamage(explosionDamage); // damage player

            //add explosion force to dancing enemies
            if (enemies[i].GetComponent<DancingAI>())
            {
                enemies[i].GetComponent<Rigidbody>().AddExplosionForce(explosionForce, transform.position, explosionRange, explosionUpwards);
                enemies[i].GetComponent<Ragdoll>().ToggleRagdoll(true);
            }
        }

        //add a little delay, just to make verything works okay
        Invoke("Delay", 0.5f);
    }

    private void Delay()
    {
        Destroy(gameObject);
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.collider.CompareTag("Bullet"))
        {
            return;
        }

        if (collisons < 1) Explode();
        collisons++;
    }

    private void Setup()
    {
        //create new physics material
        physicsMat = new PhysicMaterial();
        physicsMat.bounciness = bounciness;
        physicsMat.frictionCombine = PhysicMaterialCombine.Minimum;
        physicsMat.bounceCombine = PhysicMaterialCombine.Maximum;

        //assign material to collider
        GetComponent<SphereCollider>().material = physicsMat;
        //set gravity
        rb.useGravity = useGravity;
    }

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, explosionRange);
    }
}
