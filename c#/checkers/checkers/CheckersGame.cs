using System.Data;
using Newtonsoft.Json.Bson;

namespace checkers;

public class CheckersGame
{
    private CheckersBoard _board;

    public CheckersGame()
    {
        _board = new CheckersBoard();
    }

    private void NewGame()
    {
        while (!_board.FindWinner)
        {
            DrawScene();
            var input = Console.ReadLine()?.Split();

            switch (input)
            {
                case { Length: 1 }:
                    switch (input[0])
                    {
                        case "0":
                            SaveGame();
                            return;
                    }

                    break;
                case { Length: 2 }:
                    if (input[0].Length == 2 && input[1].Length == 2)
                        Update(input[0], input[1]);
                    break;
            }
        }
        
        Console.WriteLine(_board.Winner);
        SaveGame();
    }

    protected internal bool LoadGame()
    {
        Console.Clear();
        
        var data = "";
        Console.Write("Input filename: ");
        var fileName = Console.ReadLine();

        if (!FileWorker.LoadData(fileName, ref data))
            return false;

        var history = data.Split("\n");
        DrawScene();
        
        foreach (var step in history[..^1])
        {
            var move = step.Split();
            Update(move[0], move[1]);
            var s = Console.ReadLine();
            if (s == "0")
                break;
            DrawScene();
        }
        return true;
    }

    private void SaveGame()
    {
        Console.Clear();
        
        Console.Write("Do you want to save game? yes/no: ");
        string? answer;
        while ((answer = Console.ReadLine()) != "yes" && answer != "no")
        {
            Console.Clear();
            Console.WriteLine("Do you want to save game? yes/no: ");
        }
        
        if (answer == "yes")
        {
            Console.Write("Input filename: ");
            var fileName = Console.ReadLine();
            if (fileName != null) FileWorker.SaveData(fileName, _board.GetHistory());
        }
    }

    private void DrawScene()
    {
        Console.Clear();
        _board.DrawBoard();
    }

    private void Update(string coord1, string coord2)
    {
        coord1 = coord1.ToUpper();
        coord2 = coord2.ToUpper();
        var x1 = coord1[0] - 'A';
        var y1 = coord1[1] - '1';
        var x2 = coord2[0] - 'A';
        var y2 = coord2[1] - '1';
        if (y1 is >= 0 and < 8 && x1 is >= 0 and < 8 && y2 is >= 0 and < 8 && x2 is >= 0 and < 8)
            _board.MakeMove(x1, y1, x2, y2);
    }

    public void Start()
    {
        while (true)
        {
            Console.Clear();
            Console.WriteLine("                  Main Menu                  \n" +
                              "---- 1. New game                             \n" +
                              "---- 2. Load Game                            \n" +
                              "---- 3. Exit                                 \n");
            switch (Console.ReadLine())
            {
                case "1":
                    Console.Clear();
                    NewGame();
                    break;
                case "2":
                    LoadGame();
                    break;
                case "3":
                    return;
                default:
                    Console.Clear();
                    break;
            }
        }
    }
}