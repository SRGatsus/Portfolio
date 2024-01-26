package com.company.CustomConsoleLineModule;

/**
 * @author Roman Korotaev
 * @since 1.0
 * Represents a set of functions that need to be implemented for stable operation of the command line
 */
public interface Command {

    /**
     *  Function implementing the command.
     * @param args array of arguments passed with the command
     *@return returns a boolean value, if false, the command line stops working, otherwise it continues working
     */
    boolean execute( String... args);

    /**
     *Command description output function.
     */
    void printHelp();

    /**
     *The name of the function itself. It must be uppercase.
     */
    String getName();

    /**
     *Command description function. What and how it does.
     */
    String getDescription();
}