using NUnit.Framework;
using static NUnit.Framework.Assert;

namespace battleship;

public class Tests
{
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
    
    private void AssertOut(String expected)
    {
        That(_stringWriter.ToString().TrimEnd(Environment.NewLine.ToCharArray()), Is.EqualTo(expected));
    }
    
    [Test]
    public void InOutTest()
    {
        InOut.WriteAt("+-----+", 0, 0);
        InOut.WriteAt("+-----+", 0, 1);
        AssertOut("+-----++-----+");
    }
}