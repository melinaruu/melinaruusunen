
import java.util.ArrayList;
import java.util.HashMap;
import java.util.TreeMap;


public class StudentRegister {
    
    public static final String NAME_ORDER = "by name";
    public static final String CODE_ORDER = "by code";
    
    private HashMap<String, Student> students = new HashMap<>();
    private HashMap<String, Course> courses = new HashMap<>();
    private TreeMap<String, ArrayList<Attainment>> atts = new TreeMap<>();

    public StudentRegister() {
    }
    
    ArrayList<Student> getStudents() {
        ArrayList<Student> sorted = new ArrayList<>(students.values());
        
        sorted.sort((a,b) -> {
            return a.getName().compareTo(b.getName());
        });
        return sorted;
    }
        
    ArrayList<Course> getCourses() {
        ArrayList<Course> sorted = new ArrayList<>(courses.values());
        
        sorted.sort((a,b) -> {
            return a.getName().compareTo(b.getName());
        });
        
        return sorted;
    }
    
    public void addStudent(Student student) {
        String numero = student.getStudentNumber();
        students.put(numero, student);
    }
    
    public void addCourse(Course course) {
        String numero = course.getCode();
        courses.put(numero, course);
    }
    
    public void addAttainment(Attainment att) {
        String numero = att.getStudentNumber();
        
        if(atts.get(numero) == null) {
            atts.put(numero, new ArrayList<Attainment>());
        }
        atts.get(numero).add(att);
    }
    
    public void printStudentAttainments(String studentNumber, String order) {
        ArrayList<String> numbers = new ArrayList<>(students.keySet());
        
        if(!numbers.contains(studentNumber)) {
            System.out.println("Unknown student number: "+studentNumber);
        }
        else {
            ArrayList<Attainment> suoritukset = new ArrayList<>();
            ArrayList<String> numerot = new ArrayList<>(atts.keySet());
            
            for (String numero : numerot) {
                if (numero.equals(studentNumber)) {
                    for(Attainment att : atts.get(numero)){
                        suoritukset.add(att);
                    }
                }
            }
                        
            String nimi = students.get(studentNumber).getName();
            
            System.out.println(nimi+" ("+studentNumber+"):");
            if (order.equals(NAME_ORDER)) {
                
                suoritukset.sort((a,b) -> {
                    String kurssiA = courses.get(a.getCourseCode()).getName();
                    String kurssiB = courses.get(b.getCourseCode()).getName();
                    return kurssiA.compareTo(kurssiB);
                });
                
                for(Attainment att : suoritukset) {
                    System.out.println("  "+att.getCourseCode()+" "+ 
                            courses.get(att.getCourseCode()).getName()+": "+
                            att.getGrade());
                }

            } else if (order.equals(CODE_ORDER)) {
                
                suoritukset.sort((a,b) -> {
                    String kurssiA = a.getCourseCode();
                    String kurssiB = b.getCourseCode();
                    return kurssiA.compareTo(kurssiB);
                });
                
                for(Attainment att : suoritukset) {
                    System.out.println("  "+att.getCourseCode()+" "+ 
                            courses.get(att.getCourseCode()).getName()+": "+
                            att.getGrade());
                }
                
            } else {
                for(Attainment att : suoritukset) {
                    System.out.println("  "+att.getCourseCode()+" "+ 
                            courses.get(att.getCourseCode()).getName()+": "+
                            att.getGrade());
                }
            }
        }
    }
    
    public void printStudentAttainments(String studentNumber) {
        ArrayList<String> numbers = new ArrayList<>(students.keySet());
        
        if(!numbers.contains(studentNumber)) {
            System.out.println("Unknown student number: "+studentNumber);
        }
        else {
            ArrayList<Attainment> suoritukset = new ArrayList<>();
            ArrayList<String> numerot = new ArrayList<>(atts.keySet());
            
            for (String numero : numerot) {
                if (numero.equals(studentNumber)) {
                    for(Attainment att : atts.get(numero)){
                        suoritukset.add(att);
                    }
                }
            }
            
            String nimi = students.get(studentNumber).getName();
            
            System.out.println(nimi+" ("+studentNumber+"):");
            
            for(Attainment att : suoritukset) {
                System.out.println("  "+att.getCourseCode()+" "+ 
                        courses.get(att.getCourseCode()).getName()+": "+
                        att.getGrade());
            }    
        }
    }
}
