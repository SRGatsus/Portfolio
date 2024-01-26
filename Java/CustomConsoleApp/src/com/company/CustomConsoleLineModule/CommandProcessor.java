package com.company.CustomConsoleLineModule;
import java.util.Map;
import java.util.Scanner;
import java.util.TreeMap;


/**
 * @author Korotaev Roman
 * @since 1.0
 * The class implements the command line
 * when creating an object, pass the encoding and add your own commands and implementations to the dictionary
 * Help and Exit are implemented, you only need to implement your methods
 * To implement, you need to create a class that implements the interface {@linkplain
 * Command}. You can rewrite help and Exit. By contacting the HELP/EXIT key.
 */
public class CommandProcessor {

    /**
     * Error string if the command is not found
     */
    private static final String MSG_COMMAND_NOT_FOUND = "Command not found";
    private static final String MSG_DELIM = "==========================================";

    /**
     * Dictionary of commands: the key is the name of the command, the value is a reference to the class implementing this command
     */
    public Map<String, Command> commands;

    /**
     * Command line encoding
     */
    private String consoleEncoding;

    /**
     *Initializes the command line using {@linkplain TreeMap}. Also adds the standard Exit and Help methods.
     @param consoleEncoding-used to determine the input language
     */
    public CommandProcessor(String consoleEncoding) {
        commands = new TreeMap<>();
        Command cmd = new HelpCommand();
        commands.put(cmd.getName(), cmd);
        cmd = new ExitCommand();
        commands.put(cmd.getName(), cmd);
        this.consoleEncoding = consoleEncoding;
    }

    /**
     *Implements an infinite loop with command inputs and its search among available commands. After defining the command, it transfers control to it by calling the execute function.
     * The command is defined by means of the {@linkplain ParsedCommand} class
     */
    public void execute() {
        boolean result = true;
        Scanner scanner = new Scanner(System.in, consoleEncoding);
        do {
            System.out.print("> ");
            String fullCommand = scanner.nextLine();
            ParsedCommand pc = new ParsedCommand(fullCommand);
            if (pc.command == null || "".equals(pc.command)) {
                continue;
            }
            Command cmd = commands.get(pc.command.toUpperCase());
            if (cmd == null) {
                System.out.println(MSG_COMMAND_NOT_FOUND);
                continue;
            }
            result = cmd.execute(pc.args);
        } while (result);
        scanner.close();
    }


    /**
     * @author Roman Korotaev
     * @since 1.0
     * The class implements string parsing through spaces, and writes to the parts list
     * If part is empty, nothing happens
     * If the length of parts is one, then it means that there is only one command, without arguments. And it is written to the command variable
     * * Otherwise, the command and arguments are recorded.
     */
    class ParsedCommand {
        String command;
        String[] args;

        /**
         * The constructor implements string parsing through spaces, and writes to the parts list
         * If part is empty, nothing happens
         * If the length of parts is one, then it means that there is only one command, without arguments. And it is written to the command variable
         * Otherwise, the command and arguments are recorded.
         * @param line- a line that is parsed
         */
        public ParsedCommand(String line) {
            String parts[] = line.split(" ");
            if (parts != null) {
                command = parts[0];
                if (parts.length > 1) {
                    args = new String[parts.length - 1];
                    System.arraycopy(parts, 1, args, 0, args.length);
                }
            }
        }
    }


    /**
     *@author Roman Korotaev
     *@since 1.0
     * Implements the output of information about available commands
     */
    class HelpCommand implements Command {

        /**
         *{@inheritDoc}
         * if there are no arguments, then all available commands and their description are displayed
         * @param args array of arguments
         * @return returns a boolean value equal to true
         */
        @Override
        public boolean execute( String... args) {
            if (args == null) {
                System.out.println("Avaliable commands:\n" + MSG_DELIM);
                for (Command cmd : commands.values()) {
                    System.out.println(cmd.getName() + ": " + cmd.getDescription());
                }
                System.out.println(MSG_DELIM);
            } else {
                for (String cmd : args) {
                    System.out.println("Help for command " + cmd + ":\n" + MSG_DELIM);
                    Command command = commands.get(cmd.toUpperCase());
                    if (command == null) {
                        System.out.println(MSG_COMMAND_NOT_FOUND);
                    } else {
                        command.printHelp();
                    }
                    System.out.println(MSG_DELIM);
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
            return "HELP";
        }

        /**
         *         {@inheritDoc}
         */
        @Override
        public String getDescription() {
            return "Prints list of available commands";
        }
    }


    /**
     *@author Roman Korotaev
     *@since 1.0
     * Implements command line termination
     */
    class ExitCommand implements Command {
        /**
         *{@inheritDoc}
         * does not depend on arguments, just outputs the end of the work
         * @param args array of arguments
         * @return returns boolean value equal to false
         */
        @Override
        public boolean execute(String... args) {
            System.out.println("Finishing command processor... done.");
            return false;
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
            return "EXIT";
        }

        /**
         *         {@inheritDoc}
         */
        @Override
        public String getDescription() {
            return "Exits from command processor";
        }
    }
}