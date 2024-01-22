package com.example.sustainablechoices.model;

public class ChoicesModel {
    
    private int score;

    public ChoicesModel() {
        this.score = 0;
    }

    public int getScore() {
        return score;
    }

    public void calculateScore(String travelChoice, String foodChoice, String purchaseChoice) {
        score = 0;

        switch (travelChoice) {
            case "kävellen tai pyörällä":
                score += 4;
                break;
            case "julkisilla":
                score += 3;
                break;
            case "kimppakyydillä":
                score += 2;
                break;
            case "omalla autolla":
                score += 1;
                break;
        }

        switch (foodChoice) {
            case "kasvisruokavalion mukaan":
            case "vegaanisesti":
                score += 3;
                break;
            case "sekaruokavalion mukaan":
                score += 2;
                break;
        }

        switch (purchaseChoice) {
            case "tuotteita tarpeeseen":
            case "en mitään":
                score += 3;
                break;
            case "tuotteita huvinvuoksi":
                score += 2;
                break;
        }
    }
}
