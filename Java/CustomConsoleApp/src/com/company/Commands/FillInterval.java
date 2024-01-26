package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;
/**
 *@author Roman Korotaev
 *@since 1.0
 * Implements array filling on the interval
 */
public class FillInterval implements Command {
    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * which implements the filling of the array on the interval. There can be two parameters, one is required, the second is not.
     *      If the parameter is one, then the filling is at the point.
     *      If two, then filling in on the interval.
     * The function determines whether the parameters are entered correctly, if not, then there will be an attempt to correct the parameters.
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
        if(args.length==1){
            SqrtDecompos.Input(right,CustomInputSystim.DoubleInput("Enter the number to change at the point"+right,"You didn't enter a number"));
        }else{
            for (int index=left;index<=right;index++){
                SqrtDecompos.Input(index,CustomInputSystim.DoubleInput("Enter the number to change at the point"+index,"You didn't enter a number"));
            }
        }
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
        return "FILLINTERVAL";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "Implements array modification. There should be one or two parameters.\n" +
                " If one finds at a certain point\n" +
                " If two finds in the interval..";
    }
}
