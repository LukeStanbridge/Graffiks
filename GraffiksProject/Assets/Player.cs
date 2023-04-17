using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Player : MonoBehaviour
{
    private Animator animator;
    public Button danceButton;

    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
        danceButton = GetComponent<Button>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void DancePlayer()
    {
        animator.Play("Step Hip Hop Dance");
    }

    public void IdlePlayer()
    {

    }
}
