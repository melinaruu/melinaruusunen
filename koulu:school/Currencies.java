import java.util.Map;
import java.util.TreeMap;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Currencies {
    
    private static String readcommand(BufferedReader input) throws IOException {
        System.out.print("Enter command: ");
        return input.readLine();
    }

    public static void main(String[] args) throws IOException {
        Map<String, Double> rates = new TreeMap<String, Double>();
        String input;
        BufferedReader user_input = 
                new BufferedReader(new InputStreamReader(System.in));

        while(!"quit".equals(input = readcommand(user_input)) && input != null) {
            System.out.println(input);

            String[] cmd_parts = input.split(" ");
            
            if(cmd_parts.length == 3 && "rate".equals(cmd_parts[0]) && 
                    cmd_parts[1].length() ==3) {
                String currency = cmd_parts[1].toUpperCase();
                double rate = Double.parseDouble(cmd_parts[2]);
                rates.put(currency, rate);
                
                System.out.format("Stored the rate 1 EUR = %.3f %s%n", rate, 
                        currency);                    
            }
            else if (cmd_parts.length == 3 && "convert".equals(cmd_parts[0]) &&
                    cmd_parts[2].length() == 3) {
                String currency = cmd_parts[2].toUpperCase();
                Double rate = rates.get(currency);
                double money = Double.parseDouble(cmd_parts[1]);
                
                if(rate != null) {
                    System.out.format("%.3f %s = %.3f EUR%n", money, currency, 
                            money/rate); 
                }
                else {
                    System.out.format("No rate for %s has been stored!%n", 
                            currency);
                }
            }
            else if (cmd_parts.length == 1 && "rates".equals(cmd_parts[0])) {
                System.out.println("Stored euro rates:");
                
                for(String currency : rates.keySet()){
                    System.out.format("  %s %.3f%n", currency, rates.get(currency));
                }
            }
        }
        
        System.out.println(input);
        System.out.println("Quit-command received, exiting...");
    }
}
