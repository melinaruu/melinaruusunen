
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import static java.lang.Integer.parseInt;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

public class Standings {
    
    private Set<Team> teams = new HashSet<>();
    private HashMap<String, Team> teamsWithNames = new HashMap<>();

    public static class Team {
        
        private String name;
        private int wins;
        private int ties;
        private int losses;
        private int scored;
        private int allowed;
        private int points;
        private int games;

        public Team(String name) {
            this.name = name;
        }

        public String getName() {
            return name;
        }

        public int getWins() {
            return wins;
        }

        public int getTies() {
            return ties;
        }

        public int getLosses() {
            return losses;
        }

        public int getScored() {
            return scored;
        }

        public int getAllowed() {
            return allowed;
        }

        public int getPoints() {
            return points;
        }
        
        private int getGames() {
            return games;
        }
        
    }

    public Standings() {
    }
    
    public Standings(String filename) throws IOException {
        readMatchData(filename);
    }
    
    public void readMatchData(String filename) throws IOException {
        BufferedReader file = new BufferedReader(new FileReader(filename));
        String row;
        while((row=file.readLine()) != null) {
            String[] parts = row.split("\\t");
            String[] scores = parts[1].split("-");
            String teamA = parts[0];
            String teamB = parts[2];
            int goalsA = parseInt(scores[0]);
            int goalsB = parseInt(scores[1]);
            
            addMatchResult(teamA, goalsA, goalsB, teamB);
        }
    }
    
    public void addMatchResult(String teamNameA, int goalsA, int goalsB, 
            String teamNameB) { 
        ArrayList<String> teamNames = new ArrayList<>(teamsWithNames.keySet());
        
        if(!teamNames.contains(teamNameA)) {
            Team A_new = new Team(teamNameA);
            teams.add(A_new);
            teamsWithNames.put(teamNameA, A_new);
        }
        if(!teamNames.contains(teamNameB)) {
            Team B_new = new Team(teamNameB);
            teams.add(B_new);
            teamsWithNames.put(teamNameB, B_new);
        }
        
        Team A = teamsWithNames.get(teamNameA);
        Team B = teamsWithNames.get(teamNameB);
        A.games++;
        B.games++;
        
        if (goalsA > goalsB) {
            A.wins++;
            B.losses++;
            A.points+=3;
            A.scored+=goalsA;
            A.allowed+=goalsB;
            B.scored+=goalsB;
            B.allowed+=goalsA;
        }
        else if (goalsA < goalsB) {
            A.losses++;
            B.wins++;
            B.points+=3;
            A.scored+=goalsA;
            A.allowed+=goalsB;
            B.scored+=goalsB;
            B.allowed+=goalsA;
        }
        else if (goalsA == goalsB) {
            A.ties++;
            B.ties++;
            A.points++;
            B.points++;
            A.scored+=goalsA;
            A.allowed+=goalsB;
            B.scored+=goalsB;
            B.allowed+=goalsA;
        }
    }
    
    ArrayList<Team> getTeams() {
        ArrayList<Team> teams = new ArrayList<>(teamsWithNames.values());
        
        teams.sort((a, b) -> {
            if(a.getPoints() == (b.getPoints())) {
                int maalieroA = a.getScored()-a.getAllowed();
                int maalieroB = b.getScored()-b.getAllowed();
                
                if(maalieroA == maalieroB) {
                    if(a.getScored() == b.getScored()) {
                        return a.getName().compareTo(b.getName());
                    }
                    return b.getScored() - a.getScored();
                }
                return maalieroB - maalieroA;
            }
            return b.getPoints() - a.getPoints();
        });
        return teams;
    }
        
        
    
    public void printStandings() {
        int max = 0;
        
        for(String name : teamsWithNames.keySet()) {
            if(name.length() > max) {
                max = name.length();
            }
        }
        
        for(Team team : getTeams()) {
            int valit = max-team.name.length();
            System.out.print(team.name);
            for (int i=0; i<valit; i++) {
                    System.out.print(" ");
            }
            
            String maalit = team.scored+"-"+team.allowed;
            
            System.out.format("%4d%4d%4d%4d%7s%4d%n", team.games,
                    team.wins, team.ties, team.losses, maalit, team.points);
        }
    }
}