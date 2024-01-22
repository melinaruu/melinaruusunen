package fi.tuni.comp_se_110.weatherapp;

/**
 * 
 * @author Melina Ruusunen
 */

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;

public class WeatherModel {    
   
    private static ArrayList<WeatherDataPoint> datapoints = new ArrayList<>();
    private static final ArrayList<WeatherDataPoint> data = new ArrayList<>();
    private static ArrayList<ArrayList<WeatherAverageTemperaturePoint>> monthlyDatapoints = new ArrayList<>();

    private static final ArrayList<Float> temps = new ArrayList<>();
    private static final ArrayList<Float> winds = new ArrayList<>();
    private static final ArrayList<Float> clouds = new ArrayList<>();
    private static final ArrayList<String> dates = new ArrayList<>();   
    
    private static final DateTimeFormatter formatter1 = DateTimeFormatter.ofPattern("HH:mm");
    private static final DateTimeFormatter formatter2 = DateTimeFormatter.ofPattern("dd.MM");
    
    public static ArrayList<WeatherDataPoint> fetchWeatherData(String location, LocalDate date) throws Exception {
        return FmiApi.getWeatherData(location, date);
    }
    
    public static ArrayList<ArrayList<WeatherAverageTemperaturePoint>> fetchMonthlyData(String location, LocalDate date) throws Exception {
        ArrayList<WeatherAverageTemperaturePoint> tempData;
        ArrayList<ArrayList<WeatherAverageTemperaturePoint>> monthlyData = new ArrayList<>();
        
        tempData = FmiApi.getMonthlyTemperature(location, date);
        
        for (int i = 0 ; i < tempData.size() ; i += 6) {
            ArrayList<WeatherAverageTemperaturePoint> subList = new ArrayList<>();
            
            subList.add(tempData.get(i));
            subList.add(tempData.get(i+1));
            subList.add(tempData.get(i+2));
            subList.add(tempData.get(i+3));
            subList.add(tempData.get(i+4));
            subList.add(tempData.get(i+5));
            
            monthlyData.add(subList);
        }

        return monthlyData;
    }
    
    public static LineChart drawGraph(String radioButtonSelection, String location, LocalDate date) throws Exception{
        
        temps.clear();
        winds.clear();
        clouds.clear();
        dates.clear();
        datapoints.clear();
        data.clear();
        
        CategoryAxis xAxis = new CategoryAxis();
        NumberAxis yAxis = new NumberAxis();
     
        LineChart lineChart = new LineChart(xAxis, yAxis);
        
        if(radioButtonSelection.equals("temperature") || 
                radioButtonSelection.equals("wind") ||
                radioButtonSelection.equals("cloudiness")) {
            datapoints = fetchWeatherData(location, date);
            
            if(datapoints.isEmpty()) {
                lineChart = null;
                return lineChart;
            }
            
            int now_hour = LocalDateTime.now().getHour();
            int now_date = LocalDateTime.now().getDayOfMonth();

            for(int i = 0 ; i < datapoints.size() ; i++) {
                int datapoint_hour = datapoints.get(i).getDateTime().getHour();
                int datapoint_date = datapoints.get(i).getDateTime().getDayOfMonth();

                if (datapoint_hour >= now_hour || datapoint_date >= now_date) {
                    data.add(datapoints.get(i));
                }
            }
            
            for(int i = 0 ; i < 12 ; i++) {
                String timeAsString = data.get(i).getDateTime().format(formatter1);
                dates.add(timeAsString);
                temps.add(Float.parseFloat(data.get(i).getTemperature()));
                winds.add(Float.parseFloat(data.get(i).getWindSpeed()));
                clouds.add(Float.parseFloat(data.get(i).getTotalCloudCover()));
            }       

            if(radioButtonSelection.equals("temperature")) {
                XYChart.Series series = new XYChart.Series();
                for(int j = 0 ; j < temps.size() ; j++) {
                    series.getData().add(new XYChart.Data(dates.get(j),temps.get(j)));
                }
                lineChart.getData().add(series);
                xAxis.setLabel("time");
                yAxis.setLabel("temperature (°C)");

            } else if (radioButtonSelection.equals("wind")) {
                XYChart.Series series = new XYChart.Series();

                for(int j = 0 ; j < winds.size() ; j++) {
                    series.getData().add(new XYChart.Data(dates.get(j),winds.get(j)));
                }
                lineChart.getData().addAll(series);
                xAxis.setLabel("time");
                yAxis.setLabel("windspeed (m/s)");

            } else if (radioButtonSelection.equals("cloudiness")) {
                XYChart.Series series = new XYChart.Series();

                for(int j = 0 ; j < clouds.size() ; j++) {
                    series.getData().add(new XYChart.Data(dates.get(j),clouds.get(j)));
                }
                lineChart.getData().addAll(series);
                xAxis.setLabel("time");
                yAxis.setLabel("cloudiness (%)");
            }
            lineChart.setLegendVisible(false);
        }
        if(radioButtonSelection.equals("daily min and max") || 
                radioButtonSelection.equals("daily average")) {
            monthlyDatapoints = fetchMonthlyData(location, date);
            
            if(monthlyDatapoints.isEmpty()) {
                lineChart = null;
                return lineChart;
            }
            
            if (radioButtonSelection.equals("daily min and max")) {
                XYChart.Series series1 = new XYChart.Series();
                XYChart.Series series2 = new XYChart.Series();

                for(int j = 0 ; j < monthlyDatapoints.size() ; j++) {
                    ArrayList<WeatherAverageTemperaturePoint> datesTemperatures = monthlyDatapoints.get(j);
                    
                    Double min = Double.parseDouble(datesTemperatures.get(0).getTemperature());
                    Double max = Double.parseDouble(datesTemperatures.get(0).getTemperature());

                    for (int k = 0 ; k < datesTemperatures.size() ; k++) {
                        if (Double.parseDouble(datesTemperatures.get(k).getTemperature()) > max) {
                            max = Double.parseDouble(datesTemperatures.get(k).getTemperature());
                        }
                        if (Double.parseDouble(datesTemperatures.get(k).getTemperature()) < min) {
                            min = Double.parseDouble(datesTemperatures.get(k).getTemperature());
                        }
                    }
                    String dateAsString = datesTemperatures.get(0).getDate().format(formatter2);

                    series1.getData().add(new XYChart.Data(dateAsString, min));
                    series2.getData().add(new XYChart.Data(dateAsString, max));
                }
                series1.setName("Daily min temperature");
                series2.setName("Daily max temperature");
                lineChart.getData().addAll(series1, series2);
                xAxis.setLabel("date");
                yAxis.setLabel("temperature (°C)");

            } else if (radioButtonSelection.equals("daily average")) {
                XYChart.Series series = new XYChart.Series();

                for(int j = 0 ; j < monthlyDatapoints.size() ; j++) {
                    ArrayList<WeatherAverageTemperaturePoint> datesTemperatures = monthlyDatapoints.get(j);

                    Double sum = 0.0;

                    for (int k = 0 ; k < datesTemperatures.size() ; k++) {
                        sum += Double.parseDouble(datesTemperatures.get(k).getTemperature());
                    }

                    Double average = sum / 6;
                    String dateAsString = datesTemperatures.get(0).getDate().format(formatter2);

                    series.getData().add(new XYChart.Data(dateAsString, average));
                }
                lineChart.setLegendVisible(false);
                lineChart.getData().addAll(series);
                xAxis.setLabel("date");
                yAxis.setLabel("temperature (°C)");
            } 
        }
        
        lineChart.setMaxSize(470, 370);
        
        return lineChart;

    }
}