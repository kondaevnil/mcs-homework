using NUnit.Framework;
using static NUnit.Framework.Assert;
using static checkers.CheckersGame;

namespace checkers;

public class Tests
{
    [Test]
    public void LoadGameTest()
    {
        string str = String.Empty;
        That(LoadGame("../../../map.txt", ref str), Is.True);
        
        Console.WriteLine(str);
    }
}