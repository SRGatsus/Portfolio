using System;

namespace Laba4
{
    class Program
    {
        static void GetNumber(string value, out int Number)
        {
            bool isNumber = false;
            string stringNumber = "";
            int Number2 = 0;

            while (!isNumber)
            {
                Console.WriteLine(value);
                bool r;
                stringNumber = Console.ReadLine();

                r = int.TryParse(stringNumber, out Number2);

                if (r)
                {
                    isNumber = true;
                }
                else
                {
                    Console.WriteLine("Error:Вы ввели не число(либо не целое)");
                }
            }
            Number = Number2;
        }
        static void InputData(out int[] Stocks, out int[] Needs, out int[,] Matrix)
        {
            int Destination;
            int DeparturePoint;
            GetNumber("Введите количество пунктов назначение", out Destination);
            GetNumber("Введите количество пунктов отправление", out DeparturePoint);
            Stocks = new int[Destination];
            Needs = new int[DeparturePoint];
            Matrix = new int[DeparturePoint, Destination];
            for (int i = 0; i < Matrix.GetLength(0); i++)
            {
                for (int j = 0; j < Matrix.GetLength(1); j++)
                {
                    GetNumber("Введите стоймость из пунтка A" + (i + 1) + " в пункт B" + (j + 1), out Matrix[i, j]);
                }
            }
            for (int i = 0; i < Needs.Length; i++)
            {
                GetNumber("Введите  сколько требуеться в пункт B" + (i + 1), out Needs[i]);
            }
            for (int i = 0; i < Stocks.Length; i++)
            {
                GetNumber("Введите сколько имееться в пункт A" + (i + 1), out Stocks[i]);
            }
        }
        static void OutputData(int[] Stocks, int[] Needs, int[,] Matrix)
        {
            for (int i = 0; i < Matrix.GetLength(0); i++)
            {
                for (int j = 0; j < Matrix.GetLength(1); j++)
                {
                    Console.Write(Matrix[i, j] + "   ");
                }
                Console.WriteLine(Stocks[i] + " ");
            }
            for (int i = 0; i < Needs.Length; i++)
            {
                Console.Write(Needs[i] + " ");
            }
        }
        static void Main(string[] args)
        {
            bool WriteUser = false;
            int[] Stocks = { 140, 160, 120 };
            int[] Needs = { 150, 90, 100, 80 };
            int[,] Matrix = { { 2, 3, 4, 2 }, { 8, 4, 1, 4 }, { 9, 7, 3, 6 } };
            int[,] Matrix2;

            if (WriteUser)
            {
                InputData(out Stocks, out Needs, out Matrix);
            }
            OutputData(Stocks, Needs, Matrix);
            Matrix2 = new int[Matrix.GetLength(0), Matrix.GetLength(1)];
            int PointInt = 0;
            int PointOut = 0;
            int Value = 0;

            while (PointInt != Matrix.GetLength(1) && PointOut != Matrix.GetLength(0))
            {
                Console.WriteLine();
                Needs[PointInt] = Needs[PointInt] - Stocks[PointOut];

                if (Needs[PointInt] <= 0)
                {
                    Console.WriteLine("Потребности в пункте B" + PointInt + " полностью удовлетворены");
                    Console.WriteLine("Переходим к следующему пункту назначения");
                    Value += Matrix[PointOut, PointInt] * (Stocks[PointOut] + Needs[PointInt]);
                    Matrix2[PointOut, PointInt] = Stocks[PointOut] + Needs[PointInt];
                    Stocks[PointOut] = -Needs[PointInt];
                    Needs[PointInt] = 0;
                    PointInt++;
                }
                else
                {
                    Console.WriteLine("Запасы в пункте А" + PointOut + " полноостью исчерпаны");
                    Console.WriteLine("Переходим к следующему пункту отправления");
                    Value += Matrix[PointOut, PointInt] * (Stocks[PointOut]);
                    Matrix2[PointOut, PointInt] = Stocks[PointOut];
                    Stocks[PointOut] = 0;
                    PointOut++;
                }
                Console.WriteLine("Стоймость: " + Value);
                OutputData(Stocks, Needs, Matrix);
                Console.WriteLine();
                Console.WriteLine("Матрица распределения");
                OutputData(Stocks, Needs, Matrix2);
            }
            Console.WriteLine();
            Console.WriteLine("Стоймость: " + Value);
            Console.ReadLine();
        }
        
    }
}
