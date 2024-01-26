package com.PracticalTask;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Locale;
import java.util.Scanner;


public class Main {
    private static int column=0;
    private static String ERROR_RANGE="Error. The entered number is out of range. Enter it again";
    private static int VALUE_MIN=0;
    private static int VALUE_MAX=13;
    private static Scanner in = new Scanner(System.in);
    public static void main(String[] args) {
        if (args.length>0){
            column=isNumber(args[0],String.format("Error. This %s is not an integer",args[0]));
        }else{
            Settings settings=new Settings();
            ClassLoader loader =Settings.class.getClassLoader();
            try (InputStream io = loader.getResourceAsStream("app.properties")){
                settings.load(io);
                String value = settings.getValue("column");
                column = Integer.parseInt(value);
            }catch (Exception e){
                System.err.println("Software error.");
                column = -1;
            }
        }
        if(((column < VALUE_MIN) || (column > VALUE_MAX))){
            column=GetNumber(column);
        }
        System.out.print("Enter a comparison string: ");
        String str = in.next().toLowerCase(Locale.ROOT);
        DataInFile loadData=new DataInFile();
        long start = System.currentTimeMillis();
        ArrayList<String[]> arrayList = new ArrayList<>();
        int count =loadData.Search(arrayList,column,str);
        if(count==-1){
            return;
        }
        if (count==0){
            System.out.println("No matches found");
        }else {
             for (String[] item:arrayList) {
                    System.out.println( String.join(",",item));
                }
                System.out.println("Number of entries: " + count);
        }
        long finish = System.currentTimeMillis() - start;
        System.out.println("Time has passed, ms: " +finish);
    }

    private static int GetNumber(int column) {
        do {
            column= InputIntNumber(String.format("Enter a number in the range from %s to %s ",VALUE_MIN,VALUE_MAX),"You didn't enter a number");
        }while (!CheckNumberInRange(column,VALUE_MAX,VALUE_MIN));
        return column;
    }
    private static int InputIntNumber(String print, String error){
        int value=0;
        while (true){
            try {
                System.out.print(print);
                System.out.println("");
                value=in.nextInt();
                break;
            }catch (InputMismatchException f){
                in.next();
                System.err.println(error);
            }
        }
        return value;
    }
    private static boolean CheckNumberInRange(int number, int max, int min){
        if((number >= min) && (number <= max)){
            return true;
        }
        System.err.println("Error. The entered number is out of range. Enter it again");
        return false;

    }
    private static int isNumber(String value, String error){
        int number=-1;
        try {
            number = Integer.parseInt(value);
        }catch (NumberFormatException f){
            System.err.println(error);
            number=-1;
        }
        return number;
    }

}
