using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.ExceptionServices;
using System.Text;

namespace checkers;

public class CheckersBoard
{
    EntityColor _currentColor = EntityColor.White; // current player color
    private readonly List<List<Cell>> _board;
    private bool _findWinner = false;
    private EntityColor? _winner = null;
    private List<((int, int), (int, int))> History = new List<((int, int), (int, int))>(); // game history data
    
    internal Dictionary<EntityColor, Player> _players;

    // diagonal directions for making move
    private List<(int, int)> _directions = new List<(int, int)>()
    {
        (1, 1),
        (1, -1),
        (-1, 1),
        (-1, -1)
    };
    
    protected internal struct Player
    {
        public int Score;
        // key = (int, int) -- coordinate of player's checker, that can eat enemy's checker now
        // value = Hashset<(int, int)> -- coordinates of checker, which can be eaten by 'key' checker
        public Dictionary<(int, int), HashSet<(int, int)>> EatingList;

        public Player()
        {
            Score = 0;
            EatingList = new Dictionary<(int, int), HashSet<(int, int)>>();
        }
    }

    public bool FindWinner => _findWinner;
    internal EntityColor? Winner => _winner;

    private string _field =
        "0-0-0-0-\n" +
        "-0-0-0-0\n" +
        "0-0-0-0-\n" +
        "--------\n" +
        "--------\n" +
        "-1-1-1-1\n" +
        "1-1-1-1-\n" +
        "-1-1-1-1";

    internal enum EntityColor
    {
        White,
        Black
    }

    private struct Cell
    {
        public Cell(Checker? checker = null, bool filled = false)
        {
            Check = checker;
            IsFilled = filled;
        }

        public bool IsFilled { get; }

        public Checker? Check { get; }
    }

    private struct Checker
    {
        public Checker(EntityColor color)
        {
            Color = color;
        }

        public EntityColor Color { get; }

        public bool IsQueen { get; set; } = false;
    }

    public CheckersBoard()
    {
        _board = new List<List<Cell>>();
        _players = new Dictionary<EntityColor, Player>
        {
            [EntityColor.Black] = new Player(),
            [EntityColor.White] = new Player()
        };
        
        var cellCounter = 0;
        
        foreach (var row in _field.Split('\n'))
        {
            _board.Add(new List<Cell>());
            foreach (var cell in row)
            {
                switch (cell)
                {
                    case '0':
                        _board[cellCounter / 8].Add(new Cell(new Checker(EntityColor.White), true));
                        break;
                    case '1':
                        _board[cellCounter / 8].Add(new Cell(new Checker(EntityColor.Black), true));
                        break;
                    default:
                        _board[cellCounter / 8].Add(new Cell());
                        break;
                }

                cellCounter++;
            }
        }
    }
    
    // Constructor for testing purpose
    public CheckersBoard(string field)
    {
        _board = new List<List<Cell>>();
        _players = new Dictionary<EntityColor, Player>
        {
            [EntityColor.Black] = new Player(),
            [EntityColor.White] = new Player()
        };
        
        var cellCounter = 0;
        
        foreach (var row in field.Split('\n'))
        {
            _board.Add(new List<Cell>());
            foreach (var cell in row)
            {
                switch (cell)
                {
                    case '0':
                        _board[cellCounter / 8].Add(new Cell(new Checker(EntityColor.White), true));
                        break;
                    case '1':
                        _board[cellCounter / 8].Add(new Cell(new Checker(EntityColor.Black), true));
                        break;
                    default:
                        _board[cellCounter / 8].Add(new Cell());
                        break;
                }

                cellCounter++;
            }
        }
    }

    public void DrawBoard()
    {
        Display.WriteAt($"{_currentColor}", 0, 0);

        Display.WriteAt($"{_players[EntityColor.Black].EatingList.Count} -- Black", 0, 1);
        Display.WriteAt($"{_players[EntityColor.White].EatingList.Count} -- White", 0, 2);
        
        Display.WriteAt("Score", Console.BufferWidth - 10, 0);
        Display.WriteAt($"Black: {_players[EntityColor.Black].Score}", Console.BufferWidth - 10, 1);
        Display.WriteAt($"White {_players[EntityColor.White].Score}", Console.BufferWidth - 10, 2);
        
        Display.WriteAt("exit - 0", (Console.BufferWidth - 20) / 2, 0);
        
        Display.WriteAt("   A   B   C   D   E   F   G   H  \n", (Console.BufferWidth - 33) / 2, (Console.BufferHeight - 10) / 2);
        Display.WriteAt(" +===============================+\n", (Console.BufferWidth - 33) / 2, (Console.BufferHeight - 10) / 2 + 1);

        for (var i = 0; i < 8; i++)
        {
            var s = $"{1 + i}|";
            for (var j = 0; j < 8; j++)
            {
                s += $" {(!_board[i][j].IsFilled ? " " : _board[i][j].Check.Value.Color == EntityColor.White ? _board[i][j].Check.Value.IsQueen ?  "W" :  "☻" : _board[i][j].Check.Value.IsQueen ? "B" : "☺")} |";
            }

            Display.WriteAt(s + "\n", (Console.BufferWidth - 33) / 2, (Console.BufferHeight - 10) / 2 + i + 2);
        }

        Display.WriteAt(" +===============================+\n", (Console.BufferWidth - 33) / 2, (Console.BufferHeight - 10) / 2 + 10);
    }

    public string GetHistory()
    {
        var sb = new StringBuilder();
        foreach (var (pair1, pair2) in History)
            sb.AppendLine($"{Convert.ToChar('A' + pair1.Item1)}{pair1.Item2 + 1} {Convert.ToChar('A' + pair2.Item1)}{pair2.Item2 + 1}");

        return sb.ToString();
    }

    // Game logic
    
    /* OnBoard
     *  ---- Checks if given coordinate is correct  
     *  ---- int x, int y -- coord, that should be checked
     */
    private bool OnBoard(int x, int y)
    {
        return x is >= 0 and < 8 && y is >= 0 and < 8;
    }

    /* CanBeEaten
     *  ---- checks if new checker's position can be attacked by enemy's checkers 
     *  ---- int x, int y -- coord of checker
     */
    protected internal void CanBeEaten(int x, int y)
    {
        foreach (var dir in _directions)
        {
            CanEat(x + dir.Item1, y + dir.Item2, x, y);

            for (var i = 1; i < 7; i++)
            {
                var dx = x + dir.Item1 * i;
                var dy = y + dir.Item2 * i;
                QueenCanEat(dx, dy);
            }
        }
    }

    /* CanEat
     *  ---- Checks if checker with (x1, y1) coordinate can eat checker wth (x2, y2) coordinate
     *  ---- int x1, int y1 -- coordinate of eating checker
     *       int x2, int y2 -- coordinate of eaten checker
     */
    protected internal void CanEat(int x1, int y1, int x2, int y2)
    {
        // direction vector between (x1, y1) -> (x2, y2)
        var difx = x2 - x1;
        var dify = y2 - y1;

        // check if eaten cell is taken, current cell isn't out of board, 
        // next cell after eaten is free and colors
        if (OnBoard(x2, y2) && _board[y2][x2].IsFilled && OnBoard(x1, y1) && _board[y1][x1].IsFilled &&
            OnBoard(x2 + difx, y2 + dify) && !_board[y2 + dify][x2 + difx].IsFilled && !_board[y1][x1].Check.Value.IsQueen &&
            _board[y2][x2].Check.Value.Color != _board[y1][x1].Check.Value.Color)
        {
            if (_players[_board[y1][x1].Check.Value.Color].EatingList.ContainsKey((x1, y1))) 
                _players[_board[y1][x1].Check.Value.Color].EatingList[(x1, y1)].Add((x2, y2));
            else
                _players[_board[y1][x1].Check.Value.Color].EatingList[(x1, y1)] = new HashSet<(int, int)> { (x2, y2) };
        }
    }

    /* DeleteFromEating
     *  ---- Delete checker with (x2, y2) from eating list of checker with (x1, y1) 
     *  ---- int x1, int y1 -- eating checker coord
     *       int x2, int y2 -- deleting checker coord
     */
    private void DeleteFromEating(int x1, int y1, int x2, int y2)
    {
        if (OnBoard(x1, y1) && _board[y1][x1].IsFilled &&
            _players[_board[y1][x1].Check.Value.Color].EatingList.ContainsKey((x1, y1)))
        {
            _players[_board[y1][x1].Check.Value.Color].EatingList[(x1, y1)].Remove((x2, y2));
            if (_players[_board[y1][x1].Check.Value.Color].EatingList[(x1, y1)].Count == 0)
                _players[_board[y1][x1].Check.Value.Color].EatingList.Remove((x1, y1));
        }
    }

    /* TemporaryDeleteFunction
     *  ---- Deletes checker with (x, y) from all eating lists 
     *  ---- int x, int y -- deleting checker coord  
     */
    protected internal void TemporaryDeleteFunction(int x, int y)
    {
        foreach (var dir in _directions)
        {
            DeleteFromEating(x + dir.Item1, y + dir.Item2, x, y);
            QueenDeleteFromEating(dir, x, y);
        }
        
    }

    /* BlocksEating
     *  ---- Checks if checker with (x, y) blocks other checkers' attacks 
     *  ---- int x, int y -- blocking checker coord   
     */
    protected internal void BlocksEating(int x, int y)
    {
        foreach (var dir in _directions)
        {
            var xDir1 = x + dir.Item1;
            var yDir1 = y + dir.Item2;
            var xDir2 = x + dir.Item1 * 2;
            var yDir2 = y + dir.Item2 * 2;
            if (OnBoard(xDir2, yDir2) &&
                _board[yDir2][xDir2].IsFilled && _board[yDir1][xDir1].IsFilled &&
                _board[yDir2][xDir2].Check.Value.Color != _board[yDir1][xDir1].Check.Value.Color)
            {
                _players[_board[yDir2][xDir2].Check.Value.Color].EatingList[(xDir2, yDir2)].Remove((xDir1, yDir1));
                if (_players[_board[yDir2][xDir2].Check.Value.Color].EatingList[(xDir2, yDir2)].Count == 0)
                    _players[_board[yDir2][xDir2].Check.Value.Color].EatingList.Remove((xDir2, yDir2));
            }
        }
        QueenBlocksEating(x, y);
    }

    /* MakeMove
     *  ---- makes move from (x1, y1) to (x2, y2) 
     *  ---- int x1, int y1 -- from
     *       int x2, int y2 -- to
     */
    public bool MakeMove(int x1, int y1, int x2, int y2)
    {
        var cell1 = _board[y1][x1];
        var cell2 = _board[y2][x2];
        
        // check if current player color and checker color are similar
        if (!cell1.IsFilled || cell1.Check.Value.Color != _currentColor)
            return false;
        
        if (_players[_currentColor].EatingList.Count == 0) // check if any checkers should be eaten
        {
            if (cell2.IsFilled || x2 == x1 || y2 == y1)
                return false;

            if (cell1.Check.Value.IsQueen && !QueenCheckMove(x1, y1, x2, y2)
                || !cell1.Check.Value.IsQueen && Math.Abs(x2 - x1) != 1)
                return false;
            
            if (!cell1.Check.Value.IsQueen && 
                (_currentColor == EntityColor.Black && y2 - y1 > 0 || 
                 _currentColor == EntityColor.White && y2 - y1 < 0 || Math.Abs(y2 - y1) > 1))
                return false;
            
            // swap cells
            TemporaryDeleteFunction(x1, y1);
            var tmpCheck = cell1.Check.Value;
            if (!cell1.Check.Value.IsQueen && (_currentColor == EntityColor.Black && y2 == 0 ||
                                               _currentColor == EntityColor.White && y2 == 7))
                tmpCheck.IsQueen = true;

            _board[y2][x2] = new Cell(tmpCheck, true);
            _board[y1][x1] = new Cell();
            
            // checks if checker can eat enemy's checkers
            foreach (var dir in _directions)
                CanEat(x2, y2, x2 + dir.Item1, y2 + dir.Item2);
            QueenCanEat(x2, y2);
            
            // delete old eating list
            if (_players[_currentColor].EatingList.ContainsKey((x1, y1)))
                _players[_currentColor].EatingList.Remove((x1, y1));

            // change player color
            _currentColor = _currentColor == EntityColor.Black ? EntityColor.White : EntityColor.Black;
        }
        else
        {
            if (!_players[_currentColor].EatingList.ContainsKey((x1, y1)) || !cell1.Check.Value.IsQueen &&
                (!_players[_currentColor].EatingList[(x1, y1)].Contains(((x1 + x2) / 2, (y1 + y2) / 2)) ||
                Math.Abs(x1 - x2) != 2 || Math.Abs(y1 - y2) != 2))
                return false;

            int x, y;
            if (!cell1.Check.Value.IsQueen)
            {
                x = (x1 + x2) / 2;
                y = (y1 + y2) / 2; 
            }
            else
            {
                var res = QueenEatChecker(x1, y1, x2, y2);
                if (!res.Item1)
                    return false;
                x = res.Item2.Item1;
                y = res.Item2.Item2;
            }
            
            TemporaryDeleteFunction(x, y); // delete eaten checker from eaten list

            if (_players[_board[y][x].Check.Value.Color].EatingList.ContainsKey((x, y)))
                _players[_board[y][x].Check.Value.Color].EatingList.Remove((x, y));
            _board[y][x] = new Cell();
            
            var tmpCheck = cell1.Check.Value;
            if (!cell1.Check.Value.IsQueen && (_currentColor == EntityColor.Black && y2 == 0 ||
                                               _currentColor == EntityColor.White && y2 == 7))
                tmpCheck.IsQueen = true;
            
            TemporaryDeleteFunction(x1, y1);
            _board[y2][x2] = new Cell(tmpCheck, true);
            _board[y1][x1] = new Cell();

            // increase player's score after eating
            var tmp = _players[_currentColor];
            tmp.Score += 1;
            _players[_currentColor] = tmp;

            // checks if checker can eat enemy's checkers
            foreach (var dir in _directions)
                CanEat(x2, y2, x2 + dir.Item1, y2 + dir.Item2);
            QueenCanEat(x2, y2);
            
            // delete old eating list
            if (_players[_currentColor].EatingList.ContainsKey((x1, y1)))
                _players[_currentColor].EatingList.Remove((x1, y1));

            if (!_players[_board[y2][x2].Check.Value.Color].EatingList.ContainsKey((x2, y2)))
                _currentColor = _currentColor == EntityColor.Black ? EntityColor.White : EntityColor.Black;
        }
        
        BlocksEating(x2, y2);
        CanBeEaten(x2, y2);
        foreach (var dir in _directions)
            CanBeEaten(x1 + dir.Item1, y1 + dir.Item2);

        if (_players[EntityColor.Black].Score == 12)
        {
            _findWinner = true;
            _winner = EntityColor.Black;
        }
        else if (_players[EntityColor.White].Score == 12)
        {
            _findWinner = true;
            _winner = EntityColor.White;
        }
        
        History.Add(((x1, y1), (x2, y2)));
        return true;
    }
    
    /* * *
     * Block for queen's functions.
     * It's not a good idea to separate queen's and common checker logic, because
     * similar code will be written two times.
    */
    
    /* QueenCanEat
     *  ---- find all checkers and add them in her eatingList, that queen can eat
     *  ---- int x, int y -- coords of queen checker
     */
    private void QueenCanEat(int x, int y)
    {
        if (!OnBoard(x, y) || !_board[y][x].IsFilled || !_board[y][x].Check.Value.IsQueen)
            return;

        var eating = _board[y][x].Check;
        
        foreach (var dir in _directions)
        {
            var findEaten = false;
            var eatenCoord = (0, 0);
            var findSpace = false;
            for (var i = 1; i < 7; i++)
            {
                var dx = x + dir.Item1 * i;
                var dy = y + dir.Item2 * i;
                
                if (!OnBoard(dx, dy) ||
                    _board[dy][dx].IsFilled && _board[dy][dx].Check.Value.Color == eating.Value.Color)
                    break;
                
                if (findEaten && _board[dy][dx].IsFilled)
                    break;
                
                if (findEaten)
                {
                    findSpace = true;
                    break;
                }

                if (_board[dy][dx].IsFilled && _board[dy][dx].Check.Value.Color != eating.Value.Color)
                {
                    findEaten = true;
                    eatenCoord = (dx, dy);
                }
            }

            if (!findEaten || !findSpace)
                continue;
            
            if (_players[eating.Value.Color].EatingList.ContainsKey((x, y)) && !_players[eating.Value.Color].EatingList[(x, y)].Contains(eatenCoord))
                _players[eating.Value.Color].EatingList[(x, y)].Add(eatenCoord);
            else
                _players[eating.Value.Color].EatingList[(x, y)] = new HashSet<(int, int)> { eatenCoord };

        }
    }

    /* QueenDeleteFromEating
     *  ---- Delete checker in (x, y) coord from all queen diagonal eating lists 
     *  ---- (int, int) dir -- diagonal direction
     *       int x, int y -- coord of checker, that should be deleted
     */
    private void QueenDeleteFromEating((int, int) dir, int x, int y)
    {
        if (!OnBoard(x, y) || !_board[y][x].IsFilled)
            return;
        
        for (var i = 1; i < 7; i++)
        {
            var dx = x + dir.Item1 * i;
            var dy = y + dir.Item2 * i;

            if (!OnBoard(dx, dy) ||
                _board[dy][dx].IsFilled && (_board[dy][dx].Check.Value.Color == _board[y][x].Check.Value.Color ||
                !_board[dy][dx].Check.Value.IsQueen))
                break;
            
            if (_board[dy][dx].IsFilled && _players[_board[dy][dx].Check.Value.Color].EatingList.ContainsKey((dx, dy)))
            {
                _players[_board[dy][dx].Check.Value.Color].EatingList[(dx, dy)].Remove((x, y));
                if (_players[_board[dy][dx].Check.Value.Color].EatingList[(dx, dy)].Count == 0)
                    _players[_board[dy][dx].Check.Value.Color].EatingList.Remove((dx, dy));
                break;
            }
        }
    }

    /* QueenBlocksEating
     *  ---- Checks if the checker in (x, y) blocks queen's attack
     *  ---- int x, int y -- coords of blocking checker 
     */
    private void QueenBlocksEating(int x, int y)
    {
        foreach (var dir in _directions)
        {
            var xDir1 = x + dir.Item1;
            var yDir1 = y + dir.Item2;
            
            if (!OnBoard(xDir1, yDir1) || !_board[yDir1][xDir1].IsFilled)
                continue;

            for (var i = 2; i < 7; i++)
            {
                var xDir2 = x + dir.Item1 * i;
                var yDir2 = y + dir.Item2 * i;

                if (!OnBoard(xDir2, yDir2) || 
                    _board[yDir2][xDir2].IsFilled &&
                    _board[yDir2][xDir2].Check.Value.Color == _board[yDir1][xDir1].Check.Value.Color ||
                    _board[yDir2][xDir2].IsFilled && !_board[yDir2][xDir2].Check.Value.IsQueen)
                    break;

                if (_board[yDir2][xDir2].IsFilled)
                {
                    _players[_board[yDir2][xDir2].Check.Value.Color].EatingList[(xDir2, yDir2)].Remove((xDir1, yDir1));
                    if (_players[_board[yDir2][xDir2].Check.Value.Color].EatingList[(xDir2, yDir2)].Count == 0)
                        _players[_board[yDir2][xDir2].Check.Value.Color].EatingList.Remove((xDir2, yDir2));
                    break;
                }
            }
        }
    }

    /* QueenCheckMove
     *  ---- Checks if the queen can make move from (x1, y1) to (x2, y2)
     *  ---- int x1, int y1 -- from
     *       int x2, y2 -- to
     */
    private bool QueenCheckMove(int x1, int y1, int x2, int y2)
    {
        var xDir = x2 - x1 > 0 ? 1 : -1;
        var yDir = y2 - y1 > 0 ? 1 : -1;
        var i = 1;
        int x = x1 + xDir, y = y1 + yDir;

        while (x != x2 && y != y2)
        {
            if (_board[y][x].IsFilled)
                return false;
            
            i++;
            x = x1 + xDir * i;
            y = y1 + yDir * i;
        }

        return x == x2 && y == y2;
    }

    /* QueenEatChecker
     *  ---- Checks if the queen can eat checker
     *  ---- int x1, int y1 -- queen's coord
     *       int x2, y2 -- eaten checker coord
     */
    private (bool, (int, int)) QueenEatChecker(int x1, int y1, int x2, int y2)
    {
        var xDir = x2 - x1 > 0 ? 1 : -1;
        var yDir = y2 - y1 > 0 ? 1 : -1;
        var queen = _board[y1][x1].Check;
        var i = 1;
        int x = x1 + xDir, y = y1 + yDir;
        var find = false;

        while (!find && x != x2 && y != y2)
        {
            if (_players[queen.Value.Color].EatingList[(x1, y1)].Contains((x, y)))
            {
                find = true;
                break;
            }
            i++;
            x = x1 + xDir * i;
            y = y1 + yDir * i;
        }

        if (!find || !QueenCheckMove(x, y, x2, y2))
            return (false, (-1, -1));
        return (true, (x, y));
    }
}