
package fi.tuni.comp_se_110.weatherapp;

/**
 * 
 * @author Melina Ruusunen
 */

import java.time.LocalDate;
import javafx.fxml.FXML;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.LineChart;
import javafx.scene.control.*;
import java.net.URL;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.layout.AnchorPane;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;

public class RoadConditionController implements Initializable{
    @FXML
    private AnchorPane chart;
    @FXML
    private ComboBox<String> streetBox;
    @FXML
    private ComboBox<String> infoOptsBox;
    @FXML
    private ToggleGroup road;
    @FXML
    private CheckBox messagesCheckbox;
    @FXML
    private Label trafficMessage;
    @FXML
    private DatePicker sctDayDatePicker;
    @FXML
    private TextField xMinVal;
    @FXML
    private TextField xMaxVal;
    @FXML
    private TextField yMinVal;
    @FXML
    private TextField yMaxVal;
    
    
    private ObservableList<String> infoOpts;
    ArrayList<String> streets = new ArrayList<>();  
    ObservableList<String> streetIDs;
    
    private RootController parentController;
    
    private HashMap<String, ArrayList<RoadConditionForecastPoint>> roadConditionData = new HashMap<>();
    
    public ArrayList<Double> getCoordinates() {
        ArrayList<Double> coords = new ArrayList<>();
        if(!xMinVal.getText().isEmpty() && !xMaxVal.getText().isEmpty() 
            && !yMinVal.getText().isEmpty() && !yMaxVal.getText().isEmpty()
            && xMinVal.getText().matches("\\d+(\\.\\d+)?") 
            && xMaxVal.getText().matches("\\d+(\\.\\d+)?")
            && yMinVal.getText().matches("\\d+(\\.\\d+)?") 
            && yMaxVal.getText().matches("\\d+(\\.\\d+)?")) {
            
            double xmin = Double.parseDouble(xMinVal.getText());
            double xmax = Double.parseDouble(xMaxVal.getText());
            double ymin = Double.parseDouble(yMinVal.getText());
            double ymax = Double.parseDouble(yMaxVal.getText());
        
            if (xmin != xmax && ymin != ymax) {
                if ((xmin >= 20 && xmin <= 32) && (xmax >= 20 && xmax <= 32) && 
                    (ymin >= 59 && ymin <= 72) && (ymax >= 59 && ymax <= 72)) {
                    coords.add(xmin);
                    coords.add(ymin);
                    coords.add(xmax);
                    coords.add(ymax);
                    return coords;
                }
            }
        }
        return null; 
    }
    
    public void setGraphContent() {
        ArrayList<Double> coords = null;
        if (getCoordinates() != null) {
            coords = getCoordinates();
            RadioButton selectedRadioButton = (RadioButton) road.getSelectedToggle();
            
            if(selectedRadioButton == null) {
                // alert to choose radiobutton
                Alert alert = new Alert(AlertType.ERROR);
                alert.setTitle("Error");
                alert.setHeaderText("Choose a radiobutton!");
                alert.showAndWait();
            } else {
                String selectedValue = selectedRadioButton.getText();

                if (selectedValue.equals("maintenance")) {
                    LocalDate date = sctDayDatePicker.getValue();
                    
                    if(date == null) {
                        // alert to choose a date
                        Alert alert = new Alert(AlertType.ERROR);
                        alert.setTitle("Error");
                        alert.setHeaderText("Choose a date!");
                        alert.showAndWait();
                    } else {

                        try {
                            chart.getChildren().clear();
                            BarChart barChart = RoadConditionModel.drawBars(date, coords.get(0), coords.get(1), coords.get(2), coords.get(3));
                            if(barChart == null) {
                                Label infoMessage = new Label("No maintenance tasks in your selected area /"
                                        + " on your selected date!"); 
                                chart.getChildren().add(infoMessage);
                            } else{
                                chart.getChildren().add(barChart);
                            }
                        } catch (Exception ex) {
                            ex.printStackTrace();
                        }
                    }

                } else if (selectedValue.equals("road condition forecast")) {
                    String info = infoOptsBox.getValue();
                    String id = streetBox.getValue();
                    
                    if(info==null && id==null){
                        // alert about both missing
                        Alert alert = new Alert(Alert.AlertType.ERROR);
                        alert.setTitle("Error");
                        alert.setHeaderText("Choose a street ID and the info to show on graph!");
                        alert.showAndWait();
                    }
                    else if(info==null) {
                        // alert about no info
                        Alert alert = new Alert(Alert.AlertType.ERROR);
                        alert.setTitle("Error");
                        alert.setHeaderText("Choose the info to show on graph!");
                        alert.showAndWait();
                    } else if(id==null) {
                        // alert about no street id
                        Alert alert = new Alert(Alert.AlertType.ERROR);
                        alert.setTitle("Error");
                        alert.setHeaderText("Choose a street ID!");
                        alert.showAndWait();
                    } else {
                        try {
                            chart.getChildren().clear();
                            LineChart lineChart = RoadConditionModel.drawChart(coords.get(0), coords.get(1), coords.get(2), coords.get(3), id, info);
                            lineChart.setTitle("Road condition forecast for the next 12h");
                            chart.getChildren().add(lineChart);
                        } catch (Exception ex) {
                            ex.printStackTrace();
                        }
                    }
                }
            }
        } else {
            // alert to choose all the necessary info
            Alert alert = new Alert(AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText("Choose all the needed info!");
            alert.showAndWait();
        } 
    }
    
    public void showTrafficMessages(ActionEvent e) {
        ArrayList<Double> coords = null;
        if (getCoordinates() != null) {
            coords = getCoordinates();
            if (messagesCheckbox.isSelected()) {
            String msgToPrint = RoadConditionModel.getTrafficMsgInfo(coords.get(0), coords.get(1), coords.get(2), coords.get(3));
            trafficMessage.setText(msgToPrint);
            } else {
                trafficMessage.setText("");
            }
        } else {
            // alert about wrong coordinates
            Alert alert = new Alert(AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText("Check coordinates!");
            alert.showAndWait();
        }
    }
    
    public void setStreetIDOptions(ActionEvent e) {
        ArrayList<Double> coords = null;
        if (getCoordinates() != null) {
            streets.clear();
            streetBox.getItems().clear();
            
            coords = getCoordinates();

            roadConditionData = DigiTrafficApi.getRoadConditionsForecast(coords.get(0), coords.get(1), coords.get(2), coords.get(3));
            for (Map.Entry<String, ArrayList<RoadConditionForecastPoint>> set :
                roadConditionData.entrySet()) {
                streets.add(set.getKey());
            }
            
            if(streets.isEmpty()) {
                // alert to use wider coordinates
                Alert alert = new Alert(AlertType.ERROR);
                alert.setTitle("Error");
                alert.setHeaderText("Choose a wider coordinate area!");
                alert.showAndWait();
            } else {
                streetIDs = FXCollections.observableArrayList(streets);
                streetBox.setItems(streetIDs);
            }
        }
        else {
            // alert about wrong coordinates
            Alert alert = new Alert(AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText("Check coordinates!");
            alert.showAndWait();
        }
    }
    
    @Override
    public void initialize(URL url, ResourceBundle rb) {    
        infoOpts = FXCollections.observableArrayList("Precipitation", "Slipperiness", "Overall road condition");
        infoOptsBox.setItems(infoOpts);
    }
    
    public void initializeParentController(RootController parentController) {
        this.parentController = parentController;
    }
    
    public HashMap<String, String> getOptions() {
        HashMap<String, String> newOptions = new HashMap();
        newOptions.put("xMin", xMinVal.getText());
        newOptions.put("xMax", xMaxVal.getText());
        newOptions.put("yMin", yMinVal.getText());
        newOptions.put("yMax", yMaxVal.getText());
        newOptions.put("date", sctDayDatePicker.getValue().toString());
        newOptions.put("selected", ((RadioButton)road.getSelectedToggle()).getText());
        if(messagesCheckbox.isSelected()) {
            newOptions.put("messages", "True");
        } else {
            newOptions.put("messages", "False");
        }
        newOptions.put("street_id", streetBox.getValue());
        newOptions.put("graph_info", infoOptsBox.getValue());
        return newOptions;
    }
    
    public void loadPreference() {
        String selected = parentController.selectPreference(PreferenceType.ROAD);
        setPreference(selected);
    }
    
    public void setPreference(String name) {
        Map<String, String> preference = parentController.getPreference(PreferenceType.ROAD, name);
        setPreference(preference);
    }
    
    public void setPreference(Map<String, String> preference) {
        if(preference != null) {
            xMinVal.setText(preference.get("xMin"));
            xMaxVal.setText(preference.get("xMax"));
            yMinVal.setText(preference.get("yMin"));
            yMaxVal.setText(preference.get("yMax"));
            sctDayDatePicker.setValue(LocalDate.parse(preference.get("date"), DateTimeFormatter.ISO_DATE));
            for(Toggle toggle : road.getToggles()) {
                if(((RadioButton) toggle).getText().equals(preference.get("selected"))) {
                    road.selectToggle(toggle);
                }
            }
            if(preference.get("message") != null) {
                if(preference.get("message").equals("True")) {
                    messagesCheckbox.setSelected(true);
                } else {
                    messagesCheckbox.setSelected(false);
                }
            }
            
            streetBox.setValue(preference.get("street_id"));
            infoOptsBox.setValue(preference.get("graph_info"));
            setGraphContent();
        }
    }
    
    public void savePreference() {
        parentController.savePreference(PreferenceType.ROAD, getOptions());
    }
}
