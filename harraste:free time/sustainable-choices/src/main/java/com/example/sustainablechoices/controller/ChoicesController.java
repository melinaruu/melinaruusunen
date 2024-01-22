package com.example.sustainablechoices.controller;

import java.io.IOException;

import com.example.sustainablechoices.App;
import com.example.sustainablechoices.model.ChoicesModel;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.Toggle;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.AnchorPane;

public class ChoicesController {

    private ChoicesModel model;

    @FXML
    private AnchorPane questionnairePane;
    @FXML
    private RadioButton carRadiobutton;
    @FXML
    private RadioButton walkOrBikeRadiobutton;
    @FXML
    private RadioButton publictransRadiobutton;
    @FXML
    private RadioButton carpoolRadiobutton;
    @FXML
    private ToggleGroup travel;
    @FXML
    private RadioButton mixedRadiobutton;
    @FXML
    private RadioButton veggieRadiobutton;
    @FXML
    private RadioButton veganRadiobutton;
    @FXML
    private ToggleGroup food;
    @FXML
    private RadioButton forfunRadiobutton;
    @FXML
    private RadioButton forneedsRadiobutton;
    @FXML
    private RadioButton nothingRadiobutton;
    @FXML
    private ToggleGroup purchase;
    @FXML
    private Button resultsButton;
    @FXML
    private Label scoreLabel;
    @FXML
    private Button againButton;

    @FXML
    private void initialize() {
        model = new ChoicesModel();
    }

    public void showChoicesScene() {
        FXMLLoader loader = new FXMLLoader(App.class.getResource("choices.fxml"));
        loader.setController(this);

        try {
            AnchorPane choicesPane = loader.load();
            questionnairePane.getChildren().add(choicesPane);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    private void handleAgainClick() {
        FXMLLoader loader = new FXMLLoader(App.class.getResource("choices.fxml"));
        loader.setController(this);

        try {
            AnchorPane choicesPane = loader.load();
            questionnairePane.getChildren().clear();
            questionnairePane.getChildren().add(choicesPane);

            travel.selectToggle(null);
            food.selectToggle(null);
            purchase.selectToggle(null);
            scoreLabel.setText("");
        } catch(IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    private void handleButtonClick() {
        String travelChoice = getSelectedToggleText(travel);
        String foodChoice = getSelectedToggleText(food);
        String purchaseChoice = getSelectedToggleText(purchase);


        if (travelChoice!=null && foodChoice != null && purchaseChoice != null)  {
            model.calculateScore(travelChoice, foodChoice, purchaseChoice);
            showResults();
        } else {
            showError("Valitse vaihtoehto kaikkiin kysymyksiin!");
        }
    }

    @FXML
    private void showResults() {
        int score = model.getScore();

        FXMLLoader loader = new FXMLLoader(App.class.getResource("results.fxml"));
        loader.setController(this);

        try {
            AnchorPane resultsPane = loader.load();
            questionnairePane.getChildren().clear();
            questionnairePane.getChildren().add(resultsPane);

            scoreLabel.setText("Your score: " + score + " out of 10 points!");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String getSelectedToggleText(ToggleGroup toggleGroup) {
        Toggle selectedToggle = toggleGroup.getSelectedToggle();
        return (selectedToggle != null) ? ((RadioButton) selectedToggle).getText() : null;
    }

    private void showError(String msg) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error");
        alert.setHeaderText(null);
        alert.setContentText(msg);
        alert.showAndWait();
    }
}
