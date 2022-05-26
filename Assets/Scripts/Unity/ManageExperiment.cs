﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ManageExperiment : MonoBehaviour
{
    private int mode;
    public ManageSlider slider;
    public GameObject sliderUI;
    public ManageTraining training;
    public GameObject trainingUI;
    public ManagePrelim prelim;
    public GameObject prelimUI;
    public bool finished = false;
    public GameObject finishedUI;
    public List<DataStruct> fullData;
    public DataStruct currentData_left;
    public DataStruct currentData_right;
    public CsvWriter csvWriter;
    public int index;
    public List<float> variableList;
    // public float weightAmplitude;
    // public float weightFrequency;
    public List<TrialParameters> allParameters;
    public TrialParameters train1;
    public TrialParameters train2;
    public TrialParameters train3;
    public TrialParameters trial1;
    public TrialParameters trial2;
    public TrialParameters trial3;
    public TrialParameters trial4;
    public TrialParameters trial5;
    public TrialParameters trial6;
    public bool newStart = false;

    void Awake(){
        Debug.Log($"Initialized");
    }

    void Start()
    {
        mode = 0;
    }

    // Update is called once per frame
    void Update()
    {        
        convertAllRoughness();
        allParameters = new List<TrialParameters>(){train1,train2,train3,trial1,trial2,trial3,trial4,trial5,trial6};

        if(newStart){
            foreach(TrialParameters trial in allParameters){
                variableList.Add(trial.frequency_left);
                variableList.Add(trial.frequency_right);
            }
            csvWriter.initIndvCSVs(variableList, "frequencies");
            newStart = false;
        }

        if(mode == 0){
            training.enabled = true;
            trainingUI.SetActive(true);

            slider.enabled = false;
            sliderUI.SetActive(false);

            prelim.enabled = false;
            prelimUI.SetActive(false);
        } 
        else if(mode == 1){
            training.enabled = false;
            trainingUI.SetActive(false);

            slider.enabled = true;
            sliderUI.SetActive(true);

            prelim.enabled = false;
            prelimUI.SetActive(false);
        }
        else if(mode == 2){
            training.enabled = false;
            trainingUI.SetActive(false);

            slider.enabled = true;      // prelim uses slider script as well
            sliderUI.SetActive(false);

            prelim.enabled = true;
            prelimUI.SetActive(true);
        }

        if(finished){
            saveFullData();
            finishedUI.SetActive(true);
        }
    }
    public void changeModes(){
        if(mode < 2){
            mode += 2;
        } else{
            mode = 0;
        }
    }
    public void initData(){
        fullData = new List<DataStruct>();
    }
    public void saveFullData(){
        csvWriter.storeParticipantCSV(fullData, index);
    }
    public void saveSlider(DataStruct data){
        csvWriter.addToCSV(data);
    }
    public float convertToRoughness(float amplitude, float frequency){
        // somehow based on the current grid parameters, categorise into roughness
          
        // float roughness = (weightAmplitude * amplitude) /(weightFrequency * frequency);
        float roughness = -(20f + 4f*(frequency - 30f));
        
        // return higher number = rougher
        return roughness;
    }

    public void convertAllRoughness(){
        foreach(TrialParameters trial in allParameters){
            trial.roughness_left = convertToRoughness(trial.amplitude_left, trial.frequency_left);
            trial.roughness_right = convertToRoughness(trial.amplitude_right, trial.frequency_right);
        }
    }
}
