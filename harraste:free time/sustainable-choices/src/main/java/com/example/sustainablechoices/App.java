package com.example.sustainablechoices;

import java.io.IOException;

import com.example.sustainablechoices.controller.ChoicesController;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;


public class App extends Application
{
    public ChoicesController controller;

    public static void main( String[] args )
    {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader loader = new FXMLLoader(App.class.getResource("root.fxml"));
        Parent root = loader.load();

        controller = loader.getController();
        controller.showChoicesScene();

        Scene scene = new Scene(root, 340, 540);
        stage.setScene(scene);
        stage.setTitle("Sustainable Lifestyle");
        stage.show();
    }
}
