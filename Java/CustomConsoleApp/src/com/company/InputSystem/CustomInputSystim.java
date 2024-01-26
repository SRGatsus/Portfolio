package com.company.InputSystem;

import java.util.InputMismatchException;
import java.util.Scanner;

public class CustomInputSystim {

    public static int IntInput(String val,String error){
        Scanner in = new Scanner(System.in);
        int value=0;
        while (true){
            try {
                System.out.print(val);
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
    public static double DoubleInput(String val, String error){
        Scanner in = new Scanner(System.in);
        double value=0;
        while (true){
            try {
                System.out.print(val);
                System.out.println("");
                value=in.nextDouble();
                break;
            }catch (InputMismatchException f){
                in.next();
                System.err.println(error);
            }
        }
        return value;
    }
    public  static int CheckNumber(int value, int min, int max,String printInput,String Error){
        int number=value;
        while (true){
            if(number>=min && number<=max){
                return number;
            }else{
                System.err.println(Error);
            }
            number=IntInput(printInput,Error);
        }
    }
}
