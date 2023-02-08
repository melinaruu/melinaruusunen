package fi.tuni.comp_se_110.weatherapp;


/**
 * 
 * @author Melina Ruusunen
 */

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;

public class RoadConditionModel {
    private static HashMap<String, ArrayList<RoadConditionForecastPoint>> roadConditionData = new HashMap<>();
    private static HashMap<String, Integer> roadMaintenanceData = new HashMap<>();


    private static final ArrayList<String> precipitation = new ArrayList<>();
    private static final ArrayList<String> slipperiness = new ArrayList<>();
    private static final ArrayList<String> overallCondition = new ArrayList<>();
    private static final ArrayList<String> time = new ArrayList<>();  

    public static HashMap<String, ArrayList<RoadConditionForecastPoint>> fetchRoadConditionData(double xmin, double ymin, double xmax, double ymax) {
        return DigiTrafficApi.getRoadConditionsForecast(xmin, ymin, xmax, ymax);
    }
    
    public static HashMap<String, Integer> fetchRoadMaintenanceData(LocalDate starttime, double xmin, double ymin, double xmax, double ymax) {
        return DigiTrafficApi.getRoadMaintenanceTasks(starttime, xmin, ymin, xmax, ymax);
    }
    
    public static LineChart drawChart(double xmin, double ymin, double xmax, double ymax, String id, String info) {
        
        precipitation.clear();
        slipperiness.clear();
        overallCondition.clear();
        time.clear();
        roadConditionData.clear();
        
        roadConditionData = fetchRoadConditionData(xmin, ymin, xmax, ymax);
        
        for (Map.Entry<String, ArrayList<RoadConditionForecastPoint>> set :
             roadConditionData.entrySet()) {
            
            if (set.getKey().equals(id)) {
            
                for(var auto : set.getValue()) {
                    time.add(auto.getTime());
                    precipitation.add(auto.getPrecipitation());
                    slipperiness.add(auto.getSlipperiness());
                    overallCondition.add(auto.getOverallCondition());
                }    
            }       
        }
        
        CategoryAxis xAxis = new CategoryAxis();
        CategoryAxis yAxis = new CategoryAxis();
     
        LineChart lineChart = new LineChart(xAxis, yAxis);
                
        if(info.equals("Precipitation")) {
            XYChart.Series series = new XYChart.Series();
            for(int j = 0 ; j < precipitation.size() ; j++) {
                String withoutUnderscores = precipitation.get(j).replaceAll("_", " ");
                series.getData().add(new XYChart.Data(time.get(j),withoutUnderscores));
            }
            lineChart.getData().add(series);
            xAxis.setLabel("time");
            yAxis.setLabel("precipitation");

        } else if (info.equals("Slipperiness")) {
            XYChart.Series series = new XYChart.Series();
            for(int j = 0 ; j < slipperiness.size() ; j++) {
                
                if(slipperiness.get(j)==null) {
                    series.getData().add(new XYChart.Data(time.get(j),"NOT SLIPPERY"));
                } else {
                    String withoutUnderscores = slipperiness.get(j).replaceAll("_", " ");
                    series.getData().add(new XYChart.Data(time.get(j),withoutUnderscores));
                }
            }
            lineChart.getData().addAll(series);
            xAxis.setLabel("time");
            yAxis.setLabel("slipperiness");
        } else if (info.equals("Overall road condition")) {
            XYChart.Series series = new XYChart.Series();
            for(int j = 0 ; j < overallCondition.size() ; j++) {
                String withoutUnderscores = overallCondition.get(j).replaceAll("_", " ");
                series.getData().add(new XYChart.Data(time.get(j),withoutUnderscores));
            }
            lineChart.getData().addAll(series);
            xAxis.setLabel("time");
            yAxis.setLabel("overall road condition");
        }
        lineChart.setMaxSize(480, 353);
        lineChart.setLegendVisible(false);
             
        return lineChart;   
    }
    
    public static BarChart drawBars(LocalDate starttime, double xmin, double ymin, double xmax, double ymax) {
        
        roadMaintenanceData.clear();
        
        roadMaintenanceData = fetchRoadMaintenanceData(starttime, xmin, ymin, xmax, ymax);
        
        NumberAxis xAxis = new NumberAxis();
        CategoryAxis yAxis = new CategoryAxis();
     
        BarChart barChart = new BarChart(xAxis, yAxis);    
        
        XYChart.Series series = new XYChart.Series();
        
        if(roadMaintenanceData.isEmpty()) {
            barChart = null;
            return barChart;
        }
        
        for (Map.Entry<String, Integer> set : roadMaintenanceData.entrySet()) {
            String withoutUnderscores = set.getKey().replaceAll("_", " ");
            series.getData().add(new XYChart.Data(set.getValue(),withoutUnderscores));
        }
        
        barChart.getData().add(series);
        xAxis.setLabel("amount");
        yAxis.setLabel("task");

        barChart.setMaxSize(480, 353);
        barChart.setLegendVisible(false);
                
        return barChart;
    }
    
    public static String getTrafficMsgInfo(double xmin, double ymin, double xmax, double ymax) {
        String messageToDisplay = "";
        
        int messageAmount = DigiTrafficApi.getTrafficMessageAmount(xmin, ymin, xmax, ymax);
        
        messageToDisplay = "Amount of traffic messages in your area: " + messageAmount;
                
        return messageToDisplay;
    }
}
