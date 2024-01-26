package com.PracticalTask;

import javax.sound.sampled.AudioFormat;
import java.io.*;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

public class DataInFile {
    public int Search(ArrayList<String[]> arrayList,int column, String str){
        int count=0;
        try{
            ClassLoader classLoader = getClass().getClassLoader();
            InputStream resource = classLoader.getResourceAsStream("airports.dat");
/*            File file = new File(classLoader.getResource("airports.dat").getPath());
            FileReader fr = new FileReader(file);
            BufferedReader reader = new BufferedReader(fr);
            System.out.println(file);-вот здесь ошибка*/
            int code;
            String char_str="";
            while ((code= resource.read())!=-1) {
                if(code!=10){
                    char_str+= (char)code;
                }else{
                    String[] elements = char_str.split(",(?![\\s])");
                    if(elements[column].toLowerCase(Locale.ROOT).contains(str)){
                        arrayList.add(elements);
                        count++;
                    }
                    char_str="";
                }
            }
            resource.close();
        }catch (FileNotFoundException fileNotFoundException){
            System.err.println("Error not  file: " + fileNotFoundException.getMessage());
            return -1;
        }
        catch (Exception e){
            System.err.println("Error: " + e.getMessage());
            return -1;
        }
        Collections.sort(arrayList,new Comparator<String[]>(){
            @Override
            public int compare(String[] o1, String[] o2) {
                return o1[column].compareTo(o2[column]);
            }
        });
        return count;
    }
}
