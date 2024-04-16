namespace checkers;

public class Display
{
    private static int origRow = Console.CursorTop;
    private static int origCol = Console.CursorLeft;
    
    public static void WriteAt(string s, int x, int y)
    {
        try
        {
            Console.SetCursorPosition(origCol+x, origRow+y);
            Console.Write(s);
        }
        catch (ArgumentOutOfRangeException e)
        {
            Console.Clear();
            Console.WriteLine(e.Message);
        }
    }
}