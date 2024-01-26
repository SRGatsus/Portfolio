package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;
/**
 *@author Roman Korotaev
 *@since 1.0
 *Implements the sum of the numbers on the interval
 */
public class SumInterval implements Command {
    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * which implements the sum of the numbers on the interval. There are two parameters.
     *      If there is one parameter, then the number from the dot is output.
     *      Otherwise, the sum of the numbers in the interval is output.
     * The program itself will determine whether the arguments are passed correctly, if not, it will ask you to enter them again
     * @param args array of arguments
     * @return returns a boolean value equal to true
     */
    @Override
    public boolean execute(String... args) {
        if(args==null||SqrtDecompos==null){
            System.err.println("Error");
            return true;
        }
        int n1,n2=0;
        try {
            n1=Integer.parseInt(args[0]);
            if (args.length!=1){
                n2=Integer.parseInt(args[1]);
            }
        }catch (NumberFormatException e){
            System.err.println("Input error");
            return true;
        }
        n1= CustomInputSystim.CheckNumber(n1,0,SqrtDecompos.GetLengArray()-1,"Enter the number","The left border is entered incorrectly");
        n2=CustomInputSystim.CheckNumber(n2,0,SqrtDecompos.GetLengArray()-1,"Enter the number","The right border was entered incorrectly");
        int left=Math.min(n1,n2);
        int right=Math.max(n1,n2);
        double sum=0;
        if(args.length==1){
            sum=SqrtDecompos.Output(right);
        }else{
            for (int index=left;index<=right;index++){
                sum+=SqrtDecompos.Output(index);
            }
        }
        System.out.println("The sum is equal to: "+sum);
        return true;
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public void printHelp() {
        System.out.println(getDescription());
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getName() {
        return "SUMINTERVAL";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "Implements the sum of the numbers in the array. There should be one or two parameters.\n" +
                " If one finds at a certain point\n" +
                " If two finds in the interval.";
    }
}
