package com.company;

import com.company.Commands.FillInterval;
import com.company.Commands.Filling;
import com.company.Commands.Initialization;
import com.company.Commands.Print;
import com.company.Commands.Random;
import com.company.Commands.SearchMin;
import com.company.Commands.SumInterval;
import com.company.CustomConsoleLineModule.Command;
import com.company.CustomConsoleLineModule.CommandProcessor;

import java.util.Map;

public class Main {
    private Map<String, Command> commands;
    private static CommandProcessor cp = null;
    private static com.company.SqrtDecompositions.SqrtDecompos SqrtDec = null;

    public Main() {
    }

    public static void main(String[] args) {
        InitCommandLine();
        cp.execute();
    }

    private static void InitCommandLine() {
        cp = new CommandProcessor("Cp1251");
        SqrtDec = new com.company.SqrtDecompositions.SqrtDecompos();
        Initialization Init = new Initialization();
        Filling Fill = new Filling();
        Print Print = new Print();
        Random Random = new Random();
        SearchMin Search = new SearchMin();
        FillInterval FillIn = new FillInterval();
        SumInterval SumIn = new SumInterval();
        Init.SqrtDecompos = SqrtDec;
        Fill.SqrtDecompos = SqrtDec;
        Print.SqrtDecompos = SqrtDec;
        Random.SqrtDecompos = SqrtDec;
        Search.SqrtDecompos = SqrtDec;
        FillIn.SqrtDecompos = SqrtDec;
        SumIn.SqrtDecompos = SqrtDec;
        cp.commands.put(Init.getName(), Init);
        cp.commands.put(Fill.getName(), Fill);
        cp.commands.put(Print.getName(), Print);
        cp.commands.put(Random.getName(), Random);
        cp.commands.put(Search.getName(), Search);
        cp.commands.put(FillIn.getName(), FillIn);
        cp.commands.put(SumIn.getName(), SumIn);
    }
}
