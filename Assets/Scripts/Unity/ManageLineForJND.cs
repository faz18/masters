﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using MathNet.Numerics;
// using static System.Math;

public class ManageLineForJND : MonoBehaviour
{
    
    [System.Serializable]
    public struct LineParameters{
        public float haptic_frequency;
        public int side;    // left = -1, right = +1
        public float visual_frequency;
        public LineRenderer lineRenderer;
        public Vector3[] positions;

        public LineParameters(float h, int s, float vf, float o, LineRenderer l, Vector3[] pos){
            haptic_frequency = h;
            side = s;
            visual_frequency = vf;
            lineRenderer = l;
            positions = pos;

        }
    }
    public LineParameters leftLine;
    public LineParameters rightLine;
    public int samplingRate;
    
    
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

        leftLine.positions = parametersToPositions(leftLine);
        rightLine.positions = parametersToPositions(rightLine);
        
        updateLine();

    }

    public void updateParameters(float left_frequency, float left_roughness, float right_frequency, float right_roughness){
        leftLine.visual_frequency = left_frequency;
        leftLine.haptic_frequency = left_roughness;

        rightLine.visual_frequency = right_frequency;
        rightLine.haptic_frequency = right_roughness;

    }

    public void updateLine(){
        leftLine.lineRenderer.positionCount = leftLine.positions.Length;
        leftLine.lineRenderer.SetPositions(leftLine.positions);

        rightLine.lineRenderer.positionCount = rightLine.positions.Length;
        rightLine.lineRenderer.SetPositions(rightLine.positions);
    }
    public Vector3[] parametersToPositions(LineParameters panel){
        Vector3[] positions = new Vector3[samplingRate];

        double[] x = Generate.LinearSpaced(samplingRate, 0, (10*panel.side));
        float offset = Random.Range(0.0f, 1.0f);

        for(int i = 0; i < x.Length; i++){
            float y = (0.05f * Mathf.Sin((panel.visual_frequency * (float)x[i]) + offset));
            Vector3 coord = new Vector3((float)x[i], y, 0);

            positions[i] = coord;
        }

        return positions;
    }
}
