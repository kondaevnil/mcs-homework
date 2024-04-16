using System;


namespace checkers
{
    public static class App
    {
        private static void Main(string[] args)
        {
            var game = new CheckersGame();
            
            Console.WriteLine("## Welcome to the Checkers game ##");

            /*var tm = new TimerCallback(game.DrawScene);
            var timer = new Timer(tm, null, 0, 200);*/
            game.Start();
        }
    }
}

