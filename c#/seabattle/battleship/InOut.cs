namespace battleship;

public static class InOut
{
    private static readonly int OrigRow = Console.CursorTop;
    private static readonly int OrigCol = Console.CursorLeft;
    
    public static void WriteAt(string s, int x, int y)
    {
        try
        {
            Console.SetCursorPosition(OrigCol+x, OrigRow+y);
            Console.Write(s);
        }
        catch (ArgumentOutOfRangeException e)
        {
            Console.Clear();
            Console.WriteLine(e.Message);
        }
    }
}