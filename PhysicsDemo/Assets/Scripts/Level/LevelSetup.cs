using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class LevelSetup : MonoBehaviour
{
    [SerializeField] private Transform level1;
    [SerializeField] private Transform level2;
    [SerializeField] private Transform level3;
    [SerializeField] private Transform level4;
    [SerializeField] private Transform returnPortal1;
    [SerializeField] private ParticleSystem openLevel;
    [SerializeField] private ParticleSystem closedLevel;
    [SerializeField] private GameObject level1Text;
    [SerializeField] private GameObject returnPortal1Text;
    private ParticleSystem level1Particles;
    private ParticleSystem level2Particles;
    private ParticleSystem level3Particles;
    private ParticleSystem level4Particles;

    private bool level1Open = false;
    private bool level2Open = false;
    private bool level3Open = false;
    private bool level4Open = false;
    public bool level1Complete = false;
    public bool level2Complete = false;
    public bool level3Complete = false;
    public bool level4Complete = false;

    [SerializeField] DancingAI dancingNPC;

    // Start is called before the first frame update
    void Start()
    {
        level1Particles = Instantiate(closedLevel, level1.transform.position, closedLevel.transform.rotation);
        level2Particles = Instantiate(closedLevel, level2.transform.position, closedLevel.transform.rotation);
        level3Particles = Instantiate(closedLevel, level3.transform.position, closedLevel.transform.rotation);
        level4Particles = Instantiate(closedLevel, level4.transform.position, closedLevel.transform.rotation);
    }

    // Update is called once per frame
    void Update()
    {
        if (dancingNPC.startGame)
        {
            level1Open = true;
            if (level1Open)
            {
                level1.GetComponent<BoxCollider>().enabled = true;
                if (level1Particles != null) Destroy(level1Particles.gameObject);
                level1Particles = changeParticles(openLevel, level1);
                dancingNPC.startGame = false;
            }
        }
        else if (level1Complete)
        {
            level2Open = true;
            if (level2Open)
            {
                returnPortal1.GetComponent<BoxCollider>().enabled = true;
                if (level1Particles != null) Destroy(level1Particles.gameObject);
                level1Particles = changeParticles(closedLevel, level1);
                level1.GetComponent<BoxCollider>().enabled = false;
                level2.GetComponent<BoxCollider>().enabled = true;
                level1Complete = false;
            }
        }

        //display level text
        if (level1Open) level1Text.gameObject.SetActive(true);
        if (level2Open) returnPortal1Text.gameObject.SetActive(true);
    }

    ParticleSystem changeParticles(ParticleSystem particles, Transform levelPosition)
    {
        return Instantiate(particles, levelPosition.transform.position, openLevel.transform.rotation);
    }

    public void StartParticles()
    {
        Instantiate(openLevel, returnPortal1.transform.position, openLevel.transform.rotation);
    }
}
