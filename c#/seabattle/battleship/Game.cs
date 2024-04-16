using System.Text;

namespace battleship;

internal partial class Game
{
    // Game instance
    private static Game? _instance;

    // Game data
    private readonly Dictionary<PlayerN, Player> _players;
    private const uint FieldSize = 10;
    private PlayerN _currentPlayer;
    private bool _findWinner;

    private Game()
    {
        _players = new Dictionary<PlayerN, Player>();
    }
 
    public static Game GetInstance()
    {
        return _instance ??= new Game();
    }

    public void Start()
    {
        while (true)
        {
            Console.Clear();
            Console.WriteLine("                  Main Menu                  \n" +
                              "---- 1. New game                             \n" +
                              "---- 2. Exit                                 \n");
            switch (Console.ReadLine())
            {
                case "1":
                    Console.Clear();
                    NewGame();
                    break;
                case "2":
                    return;
            }
        }
    }

    private void NewGame()
    {
        PlayerN? winner = null;
        _findWinner = false;
        _currentPlayer = PlayerN.Player1;
        
        foreach (PlayerN player in Enum.GetValues(typeof(PlayerN)))
            _players[player] = new Player(ShipPlacing(player), FieldSize);
        
        while (!_findWinner)
        {
            DrawGame();
            
            var opponent = _currentPlayer == PlayerN.Player1 ? PlayerN.Player2 : PlayerN.Player1;
            
            switch (Console.ReadKey().Key)
            {
                case ConsoleKey.Escape:
                    return;
                case ConsoleKey.LeftArrow:
                    _players[opponent].Field.ChangeCursorPos(-1, 0);
                    break;
                case ConsoleKey.RightArrow:
                    _players[opponent].Field.ChangeCursorPos(1, 0);
                    break;
                case ConsoleKey.UpArrow:
                    _players[opponent].Field.ChangeCursorPos(0, -1);
                    break;
                case ConsoleKey.DownArrow:
                    _players[opponent].Field.ChangeCursorPos(0, 1);
                    break;
                case ConsoleKey.Enter:
                    if (!_players[opponent].Field.CheckFire())
                    {
                        var isHit = _players[opponent].Field.Fire();
                        if (!isHit)
                            _currentPlayer = opponent;
                        else if (_players[opponent].Field.ShipsCount == 0)
                        {
                            _findWinner = true;
                            winner = _currentPlayer;
                        }
                    }
                    break;
            }
        }
        
        Console.SetCursorPosition(0, Console.BufferHeight);
        Console.WriteLine($"Winner is... {winner}");
        Console.WriteLine("Press any key to continue");
        Console.ReadKey();
    }

    private void DrawGame()
    {
        Console.Clear();
        var y = 0;
        
        // game information
        var gameInfo = "Press ←, →, ↑, ↓ -- cursor pos   enter -- fire   esc -- exit";
        InOut.WriteAt(gameInfo, (Console.BufferWidth - gameInfo.Length) / 2, y++);
        
        // score
        var shipsLeft1 = $"player 1: {_players[PlayerN.Player1].Field.ShipsCount}";
        var shipsLeft2 = $"player 2: {_players[PlayerN.Player2].Field.ShipsCount}";
        InOut.WriteAt(shipsLeft1, 0, y++);
        InOut.WriteAt(shipsLeft2, 0, y++);
        
        var tmpY = y;
        var offset = 0;
        int lineLen = 0;

        foreach (var playersValue in _players.Values)
        {
            y = tmpY;
            foreach (var line in playersValue.Field.ToString().Split("\n").SkipLast(1))
            {
                InOut.WriteAt(line, offset == 0 ? 0 : Console.BufferWidth - line.Length, y++);
                lineLen = line.Length;
            }
            Console.WriteLine();
            var tmp = playersValue.Field.GetCursorCell();
            Console.BackgroundColor = ConsoleColor.White;
            if (!tmp.Fired)
            {
                InOut.WriteAt("  ",offset == 0 ? 0 + playersValue.Field.HighlightPos.Item1 :
                        Console.BufferWidth - lineLen + playersValue.Field.HighlightPos.Item1,
                    playersValue.Field.HighlightPos.Item2 + tmpY);
            }
            else
            {
                InOut.WriteAt(tmp.ShipName.HasValue ? "[]" : "øø",offset == 0 ? 0 + playersValue.Field.HighlightPos.Item1 :
                        Console.BufferWidth - lineLen + playersValue.Field.HighlightPos.Item1,
                    playersValue.Field.HighlightPos.Item2 + tmpY);
            }
            Console.ResetColor();
            offset++;
        }
    }

    private List<List<Cell>> ShipPlacing(PlayerN player)
    {
        var placer = new Placer(FieldSize);

        while (placer.ShipsCount != 0)
        {
            placer.Draw();
            InOut.WriteAt($"{player} placing ships", 0, 0);
            InOut.WriteAt("Press 1, 2, 3, 4 for choosing ship size", 0, 1);
            switch (Console.ReadKey().Key)
            {
                case ConsoleKey.D1:
                    if (placer.CanAddShip(1))
                        placer.AddShip(1);
                    break;
                case ConsoleKey.D2:
                    if (placer.CanAddShip(2))
                        placer.AddShip(2);
                    break;
                case ConsoleKey.D3:
                    if (placer.CanAddShip(3))
                        placer.AddShip(3);
                    break;
                case ConsoleKey.D4:
                    if (placer.CanAddShip(4))
                        placer.AddShip(4);
                    break;
                case ConsoleKey.LeftArrow:
                    placer.MoveCursor(-1, 0);
                    break;
                case ConsoleKey.RightArrow:
                    placer.MoveCursor(1, 0);
                    break;
                case ConsoleKey.UpArrow:
                    placer.MoveCursor(0, -1);
                    break;
                case ConsoleKey.DownArrow:
                    placer.MoveCursor(0, 1);
                    break;
                case ConsoleKey.Enter:
                    // placer.ChooseShip();
                    break;
            }
        }

        var field = placer.GetMap();
        var cellField = new List<List<Cell>>();
        var y = 0;

        foreach (var row in field.Item1)
        {
            cellField.Add(new List<Cell>());
            var x = 0;
            foreach (var cell in row)
            {
                if (cell == 1)
                {
                    int? id = null;

                    foreach (var key in field.Item2.Keys.Where(key => field.Item2[key].Contains((x, y))))
                    {
                        id = key;
                        break;
                    }
                    
                    cellField[y].Add(new Cell(id));
                }
                else
                    cellField[y].Add(new Cell());

                x++;
            }

            y++;
        }

        return cellField;
    }

    private class Placer
    {
        private readonly List<List<int>> _map;
        private readonly Dictionary<int, int> _ships;
        public int ShipsCount;
        private readonly uint _fieldSize;
        private (int, int) _startPos = (-1, -1);
        private (int, int) _endPos = (-1, -1);
        private bool _placing;
        private int _rotatedPos;
        private readonly Dictionary<int, List<(int, int)>> _shipIds;
        private int _shipId;


        private (int, int) _cursorPos = (0, 0);

        public Placer(uint fieldSize)
        {
            _map = new List<List<int>>();
            _shipIds = new Dictionary<int, List<(int, int)>>();
            for (var y = 0; y < fieldSize; y++)
            {
                _map.Add(new List<int>());
                for (var x = 0; x < fieldSize; x++)
                    _map[y].Add(0);
            }
            
            _fieldSize = fieldSize;
            _ships = new Dictionary<int, int>()
            {
                { 1, 4 },
                { 2, 3 },
                { 3, 2 },
                { 4, 1 }
            };
            foreach (var ship in _ships.Values)
            {
                ShipsCount += ship;
            }
        }

        // Add on layer
        public void AddShip(int n)
        {
            _startPos = (0, 0);
            _endPos = (n - 1, 0);
            _placing = true;
            var placed = false;
            _rotatedPos = 0;

            while (!placed)
            {
                Draw();
                
                switch (Console.ReadKey().Key)
                {
                    case ConsoleKey.UpArrow:
                        MoveShip(0, -1);
                        break;
                    case ConsoleKey.DownArrow:
                        MoveShip(0, 1);
                        break;
                    case ConsoleKey.RightArrow:
                        MoveShip(1, 0);
                        break;
                    case ConsoleKey.LeftArrow:
                        MoveShip(-1, 0);
                        break;
                    case ConsoleKey.Enter:
                        placed = PlaceShip();
                        break;
                    case ConsoleKey.R:
                        RotateShip((_rotatedPos + 1) % 4);
                        break;
                }
            }
            _ships[n]--;
            ShipsCount--;
        }

        // Place on map
        private bool PlaceShip()
        {
            if (Intersects())
                return false;

            (int, int) start;
            (int, int) end;
            
            if (_startPos.Item1 < _endPos.Item1)
            {
                start.Item1 = _startPos.Item1;
                end.Item1 = _endPos.Item1;
            }
            else
            {
                start.Item1 = _endPos.Item1;
                end.Item1 = _startPos.Item1;
            }
            
            if (_startPos.Item2 < _endPos.Item2)
            {
                start.Item2 = _startPos.Item2;
                end.Item2 = _endPos.Item2;
            }
            else
            {
                start.Item2 = _endPos.Item2;
                end.Item2 = _startPos.Item2;
            }

            if (!_shipIds.ContainsKey(_shipId))
                _shipIds[_shipId] = new List<(int, int)>();
            for (var y = start.Item2; y <= end.Item2; y += 1)
                for (var x = start.Item1; x <= end.Item1; x += 1)
                {
                    _map[y][x] = 1;
                    _shipIds[_shipId].Add((x, y));
                }
            _shipId++;
            
            _placing = false;
            return true;
        }

        // Choose on map
        public void ChooseShip()
        {
            throw new NotImplementedException();
        }

        // Rotate on layer
        private void RotateShip(int pos)
        {
            switch (pos)
            {
                case 0:
                    var dif = _startPos.Item2 - _endPos.Item2;
                    if (OnMap(_startPos.Item1 + dif, _startPos.Item2))
                    {
                        _endPos.Item1 = _startPos.Item1 + dif;
                        _endPos.Item2 = _startPos.Item2;
                        _rotatedPos += 1;

                    }
                    break;
                case 1:
                    dif = _endPos.Item1 - _startPos.Item1;
                    if (OnMap(_startPos.Item1, _startPos.Item2 + dif))
                    {
                        _endPos.Item1 = _startPos.Item1;
                        _endPos.Item2 = _startPos.Item2 + dif;
                        _rotatedPos += 1;
                    }
                    break;
                case 2:
                    dif = _endPos.Item2 - _startPos.Item2;
                    if (OnMap(_startPos.Item1 - dif, _startPos.Item2))
                    {
                        _endPos.Item1 = _startPos.Item1 - dif;
                        _endPos.Item2 = _startPos.Item2;
                        _rotatedPos += 1;
                    }
                    break;
                case 3:
                    dif = _startPos.Item1 - _endPos.Item1;
                    if (OnMap(_startPos.Item1, _startPos.Item2 - dif))
                    {
                        _endPos.Item1 = _startPos.Item1;
                        _endPos.Item2 = _startPos.Item2 - dif;
                        _rotatedPos += 1;
                    }
                    break;
            }
        }

        // Move on layer
        private void MoveShip(int x, int y)
        {
            if (!OnMap(_startPos.Item1 + x, _startPos.Item2 + y) ||
                !OnMap(_endPos.Item1 + x, _endPos.Item2 + y)) return;
            
            _startPos = (_startPos.Item1 + x, _startPos.Item2 + y);
            _endPos = (_endPos.Item1 + x, _endPos.Item2 + y);
        }

        public void MoveCursor(int x, int y)
        {
            if (OnMap(x + _cursorPos.Item1, y + _cursorPos.Item2))
                _cursorPos = (_cursorPos.Item1 + x, _cursorPos.Item2 + y);
        }

        public void Draw()
        {
            Console.Clear();
            var i = 0;

            foreach (var line in DrawField().Split("\n").SkipLast(1))
                InOut.WriteAt(line, (Console.BufferWidth - line.Length) / 2, i++);
            Console.WriteLine();

            if (_placing)
            {
                InOut.WriteAt("Use ←, →, ↑, ↓ to move ship", 0, 0);
                InOut.WriteAt("Press R for rotating ship", 0, 1);
                InOut.WriteAt("Press Enter for placing ship", 0, 2);
                
                var shipCoords = new List<(int, int)>();
                
                (int, int) start;
                (int, int) end;
            
                if (_startPos.Item1 < _endPos.Item1)
                {
                    start.Item1 = _startPos.Item1;
                    end.Item1 = _endPos.Item1;
                }
                else
                {
                    start.Item1 = _endPos.Item1;
                    end.Item1 = _startPos.Item1;
                }
            
                if (_startPos.Item2 < _endPos.Item2)
                {
                    start.Item2 = _startPos.Item2;
                    end.Item2 = _endPos.Item2;
                }
                else
                {
                    start.Item2 = _endPos.Item2;
                    end.Item2 = _startPos.Item2;
                }

                for (var y = start.Item2; y <= end.Item2; y += 1)
                    for (var x = start.Item1; x <= end.Item1; x += 1)
                        shipCoords.Add((x, y));

                var sb = new StringBuilder();
                var sep = new StringBuilder("     ");

                for (i = 0; i < _fieldSize; i++)
                    sep.Append("+----");
                sep.Append('+');
            
                var sep1 = sep.ToString();
                sep.Clear();

                for (i = 0; i < _fieldSize; i++)
                {
                
                    sb.AppendLine(sep1);
                    sb.Append($"     ");
                    for (var j = 0; j < _fieldSize; j++)
                        sb.Append($"| {(shipCoords.Contains((j, i)) ? "[]" : "  ")} ");
                    sb.AppendLine("|");
                }
                sb.AppendLine(sep1);

                var tmpStr = sb.ToString().Split("\n").SkipLast(1).ToList();
                sb.Clear();

                Console.ForegroundColor = Intersects() ? ConsoleColor.Red : ConsoleColor.Green;
                for (i = 0; i < tmpStr.Count; i++)
                {
                    for (var j = 0; j < tmpStr[i].Length; j++)
                    {
                    
                        if (tmpStr[i][j] == '[')
                            InOut.WriteAt("[]", (Console.BufferWidth - tmpStr[i].Length) / 2 + j, i);
                    }
                }
                Console.ResetColor();
            }
        }
        
        public (IEnumerable<List<int>>, Dictionary<int, List<(int, int)>>) GetMap()
        {
            return (_map, _shipIds);
        }

        private bool OnMap(int x, int y)
        {
            return x < _fieldSize && x >= 0 && y >= 0 && y < _fieldSize;
        }

        private bool Intersects()
        {
            (int, int) start;
            (int, int) end;
            
            if (_startPos.Item1 < _endPos.Item1)
            {
                start.Item1 = _startPos.Item1;
                end.Item1 = _endPos.Item1;
            }
            else
            {
                start.Item1 = _endPos.Item1;
                end.Item1 = _startPos.Item1;
            }
            
            if (_startPos.Item2 < _endPos.Item2)
            {
                start.Item2 = _startPos.Item2;
                end.Item2 = _endPos.Item2;
            }
            else
            {
                start.Item2 = _endPos.Item2;
                end.Item2 = _startPos.Item2;
            }
            
            
            for (var y = start.Item2; y <= end.Item2; y += 1)
            {
                for (var x = start.Item1; x <= end.Item1; x += 1)
                {
                    if (_map[y][x] != 0)
                        return true;
                    
                    foreach (var dif in Directions)
                    {
                        var i = x + dif.Item1;
                        var j = y + dif.Item2;

                        if (OnMap(i, j) && _map[j][i] != 0)
                            return true;
                    }
                }
            }
            return false;
        }

        public bool CanAddShip(int n)
        {
            return _ships[n] > 0;
        }

        private string DrawField()
        {
            var sb = new StringBuilder();
            var sep = new StringBuilder("     ");

            for (var i = 0; i < _fieldSize; i++)
                sep.Append("+----");
            sep.Append('+');
            
            var sep1 = sep.ToString();
            sep.Clear();

            for (var i = 0; i < _fieldSize; i++)
            {
                
                sb.AppendLine(sep1);
                sb.Append($"     ");
                for (var j = 0; j < _fieldSize; j++)
                    sb.Append($"| {(_map[i][j] == 1 ? "[]" : "  ")} ");
                sb.AppendLine("|");
            }
            sb.AppendLine(sep1);
            return sb.ToString();
        }
    }
}