using NUnit.Framework;
using NUnit.Framework.Constraints;
using static NUnit.Framework.Assert;
using static checkers.CheckersGame;

namespace checkers;

public class Tests
{
    [Test]
    public void CanEatTest()
    {
        const string field = "0-0-0-0-\n" +
                             "-0-0-0-0\n" +
                             "0-0-----\n" +
                             "-1-0---0\n" +
                             "--1-----\n" +
                             "---1-1-1\n" +
                             "--1-1-1-\n" +
                             "-1-1-1-1";
        var board = new CheckersBoard(field);
        
        board.CanEat(2, 4, 3, 3);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((2, 4)), Is.True);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList[(2, 4)].Contains((3, 3)), Is.True);
        
        board.CanEat(1, 3, 2, 2);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((1, 3)), Is.False);
        
        board.CanEat(3, 3, 2, 4);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((3, 3)), Is.False);
        That(board._players[CheckersBoard.EntityColor.White].EatingList.ContainsKey((3, 3)), Is.True);
        
        board.CanEat(0, 0, 7, 7);
        That(board._players[CheckersBoard.EntityColor.White].EatingList.ContainsKey((0, 0)), Is.False);
    }
    
    [Test]
    public void CanBeEatenTest()
    {
        const string field = "0-0-0-0-\n" +
                             "-0-0-0-0\n" +
                             "0-0-----\n" +
                             "-1-0---0\n" +
                             "--1-----\n" +
                             "---1-1-1\n" +
                             "--1-1-1-\n" +
                             "-1-1-1-1";
        var board = new CheckersBoard(field);
        
        board.CanBeEaten(3, 3);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((2, 4)), Is.True);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList[(2, 4)].Contains((3, 3)), Is.True);
        
        board.CanBeEaten(2, 2);
        board.CanBeEaten(7, 7);
        board.CanBeEaten(0, 0);
        board.CanBeEaten(-1, -1);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((1, 3)), Is.False);

        foreach (var pair in board._players.SelectMany(player => player.Value.EatingList))
        {
            That(pair.Value.Contains((2, 2)), Is.False);
            That(pair.Value.Contains((7, 7)), Is.False);
            That(pair.Value.Contains((0, 0)), Is.False);
            That(pair.Value.Contains((-1, -1)), Is.False);
        }
        
        board.CanBeEaten( 2, 4);
        That(board._players[CheckersBoard.EntityColor.White].EatingList.ContainsKey((3, 3)), Is.True);
    }

    [Test]
    public void TemporaryDeleteFunctionTest()
    {
        const string field = "0-0-0-0-\n" +
                             "-0-0-0-0\n" +
                             "0-0-----\n" +
                             "-1-0---0\n" +
                             "--1---1-\n" +
                             "---1---1\n" +
                             "--1-1-1-\n" +
                             "-1-1-1-1";
        var board = new CheckersBoard(field);

        for (var i = 0; i < 8; i++)
        {
            for (var j = 0; j < 8; j++)
            {
                board.CanBeEaten(i, j);
                board.TemporaryDeleteFunction(i, j);
                foreach (var pair in board._players.SelectMany(player => player.Value.EatingList))
                {
                    That(pair.Value.Contains((i, j)), Is.False);
                }
            }
        }
    }

    [Test]
    public void BlocksEatingTest()
    {
        const string field = "0-0-0-0-\n" +
                             "-0-0-0-0\n" +
                             "0-0-----\n" +
                             "-1-0---0\n" +
                             "--1---1-\n" +
                             "---1---1\n" +
                             "--1-1-1-\n" +
                             "-1-1-1-1";
        var board = new CheckersBoard(field);
        
        board.CanBeEaten(3, 3);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((2, 4)), Is.True);
        // if we place checker on (4, 2), it will block (2, 4) attack on (3, 3)
        board.BlocksEating(4, 2);
        That(board._players[CheckersBoard.EntityColor.Black].EatingList.ContainsKey((2, 4)), Is.False);
        
        board.CanBeEaten(6, 4);
        That(board._players[CheckersBoard.EntityColor.White].EatingList.ContainsKey((7, 3)), Is.True);
        // if we place checker on (5, 5), it will block (7, 3) attack on (6, 4)
        board.BlocksEating(5, 5);
        That(board._players[CheckersBoard.EntityColor.White].EatingList.ContainsKey((7, 3)), Is.False);
    }

    private readonly TextReader _standartIn = Console.In;
    private readonly TextWriter _standartOut = Console.Out;
    private StringWriter _stringWriter = new StringWriter();

    [SetUp]
    public void Setup()
    {
        var stringWriter = new StringWriter();
        _stringWriter = stringWriter;
        Console.SetOut(_stringWriter);
    }
    
    [TearDown]
    public void TearDown()
    {
        Console.SetIn(_standartIn);
        Console.SetOut(_standartOut);
        _stringWriter.Close();
    }
    
    [Test]
    public void LoadGameTest()
    {
        var game = new CheckersGame();
        Console.SetIn(new StringReader("game.txt"));
        That(game.LoadGame(), Is.True);
    }
}