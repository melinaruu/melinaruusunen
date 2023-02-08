package fi.tuni.comp_se_110.weatherapp;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

/**
 * This is the root class of the Application.
 * It's responsible for keeping track of global variables and creating the UI.
 */
public class App extends Application {
    
    private Map<PreferenceType, Map<String, Map<String, String>>> preferences = new HashMap();
    public RootController rootViewController;
    
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(App.class.getResource("root.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 800, 480);
        rootViewController = fxmlLoader.getController();
        rootViewController.initializeRoot(this);
        stage.setTitle("WeatherApp");
        stage.setScene(scene);
        stage.show();
        loadPreferences();
    }
    
    @Override
    public void stop() {
        savePreferences();
    }
    
    private void savePreferences() {
        File file = new File("preferences.json");
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter(file));
            bw.write(JsonConverter.toJSON(preferences));
            bw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void loadPreferences() {
        String content = null;
        File file = new File("preferences.json");
        try {
            content = new String(Files.readAllBytes(Paths.get(file.toURI())));
        } catch(IOException e) {
            e.printStackTrace();
        }
        Map<String, Map<String, Map<String, String>>> stringPreferences = new HashMap();
        if(content != null) {
            stringPreferences = JsonConverter.getFromJSON(content, Map.class);
        }
        if(stringPreferences.get("WEATHER") != null) {
            preferences.put(PreferenceType.WEATHER, stringPreferences.get("WEATHER"));
        } 
        if(stringPreferences.get("ROAD") != null) {
            preferences.put(PreferenceType.ROAD, stringPreferences.get("ROAD"));
        }
        if(stringPreferences.get("COMBINED") != null) {
            preferences.put(PreferenceType.COMBINED, stringPreferences.get("COMBINED"));
        }
    }
    
    public void addNewPreference(PreferenceType type, String name, Map<String, String> data) {
        if(preferences.get(type) == null) {
            preferences.put(type, new HashMap());
        }
        preferences.get(type).put(name, data);
    }
    
    public String[] getPreferenceNames(PreferenceType type) {
        if(!preferences.containsKey(type)) {
            return null;
        }
        Map<String, Map<String, String>> selected = preferences.get(type);
        String[] preferenceNames = selected.keySet().toArray(new String[selected.size()]);
        return preferenceNames;
    }
    
    public Map<String, String> getPreference(PreferenceType type, String name) {
        return preferences.get(type).get(name);
    }

    public static void main(String[] args) {
        launch();
    }

}