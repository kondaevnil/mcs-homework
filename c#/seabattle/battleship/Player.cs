namespace battleship;

internal partial class Game
{
    private enum PlayerN
    {
        Player1,
        Player2
    }
    
    private struct Player
    {
        internal readonly Field Field;

        public Player(List<List<Cell>> field, uint fieldSize)
        {
            Field = new Field(field, fieldSize);
        }
    }
}