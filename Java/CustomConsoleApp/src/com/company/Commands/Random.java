package com.company.Commands;

import com.company.CustomConsoleLineModule.Command;
import com.company.SqrtDecompositions.SqrtDecompos;
import com.company.InputSystem.CustomInputSystim;
/**
 *@author Roman Korotaev
 *@since 1.0
 * Implements random array filling
 */
public class Random implements Command {
    public SqrtDecompos SqrtDecompos=null;

    /**
     *{@inheritDoc}
     * Which Implements random array filling. There can be two arguments. The first one is mandatory.
     * If there is one argument, then the second one will be equal to 0.
     * The function will automatically determine the maximum and minimum arguments and cause random filling of the array.
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
        SqrtDecompos.FillRandom(Math.min(n1,n2),Math.max(n1,n2));
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
        return "RANDOM";
    }

    /**
     *         {@inheritDoc}
     */
    @Override
    public String getDescription() {
        return "Implements random filling. There are two parameters. One is mandatory, the second is optional. The parameters carry out the random number selection distance. According to the standard, the second parameter is 0.";
    }
}
