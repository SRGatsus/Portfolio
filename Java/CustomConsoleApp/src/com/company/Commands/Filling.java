package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;

/**
 *@author Roman Korotaev
 *@since 1.0
 * Implements array filling
 */
public class Filling implements Command {
    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * which implements array filling. The number of arguments is determined by the user himself.
     *      If there are no arguments, then the filling starts from zero.
     *      If the number of arguments is less than the length of the array or equal to the length, then the number of arguments will be filled in, then there will be standard input (if the argument does not meet the requirement, then there will be a poptyk of input to replace this argument).
     *      If there are more arguments than necessary, the remaining arguments will be displayed.
     * @param args array of arguments
     * @return returns a boolean value equal to true
     */
    @Override
    public boolean execute(String... args) {
        if(SqrtDecompos==null || !SqrtDecompos.IsArray()){
            System.err.println("Error");
            return true;
        }
        CustomInputSystim CIS=new CustomInputSystim();
        if(args!=null){
            int indexArray=0;
            int indexArgs=0;
            double num=0;
            while (indexArgs<args.length && indexArray<SqrtDecompos.GetLengArray()){
                try {
                    System.out.println("Checking for a number in a string:"+args[indexArgs] );
                    num=Double.parseDouble(args[indexArgs]);
                }catch (NumberFormatException e){
                    System.err.println(args[indexArgs] +" is not a number. Lead anew");
                    num=CIS.DoubleInput("Enter the number","You entered incorrectly");
                }
                SqrtDecompos.Input(indexArray,num);
                indexArgs++;
                indexArray++;
            }
            for (int i=indexArgs;i<args.length;i++){
                System.out.println("Number "+args[i]+" not included in the array");
            }
            for (int i=indexArgs;i<SqrtDecompos.GetLengArray();i++){
                SqrtDecompos.Input(i,CIS.DoubleInput("Enter the number","You entered incorrectly"));
            }
            SqrtDecompos.FillMinArray();
        }else{
            SqrtDecompos.Fill();
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
        return "FILL";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "This command implements array filling. The array must be initialized. You can just write a command or a command and numbers, the command will fill in itself or ask you to fill in more..";
    }

}
