using System.Text;

namespace battleship;

internal partial class Game
{
    static readonly List<(int, int)> Directions = new List<(int, int)>()
    {
        (1, 1),
        (1, -1),
        (-1, 1),
        (-1, -1),
        (1, 0),
        (-1, 0),
        (0, 1),
        (0, -1),
    };
    
    private struct Cell
    {
        public readonly bool Fired;
        public readonly int? ShipName = null;

        public Cell(int? shipName, bool fired = false)
        {
            ShipName = shipName;
            Fired = fired;
        }

        public Cell()
        {
            Fired = false;
        }
    }
    private class Field
    {
        private readonly uint _fieldSize;
        private List<List<Cell>> _field;
        private readonly Dictionary<int, List<(int, int)>> _ships;
        private readonly Dictionary<int, List<(int, int)>> _tmpShips;
        private (int, int) _cursorPos = (0, 0);
        public (int, int) HighlightPos = (0, 0); 
        public int ShipsCount;
        public Field(List<List<Cell>> field, uint fieldSize = 10)
        {
            _field = field;
            _fieldSize = fieldSize;
            _ships = new Dictionary<int, List<(int, int)>>();
            var y = 0;

            foreach (var row in _field)
            {
                var x = 0;
                foreach (var cell in row)
                {
                    if (cell.ShipName.HasValue)
                    {
                        if (_ships.ContainsKey(cell.ShipName.Value))
                            _ships[cell.ShipName.Value].Add((x, y));
                        else
                        {
                            _ships[cell.ShipName.Value] = new List<(int, int)> { (x, y) };
                            ShipsCount++;
                        }
                    }
                    x++;
                }
                y++;
            }
            _tmpShips = _ships.ToDictionary(entry => entry.Key,
                entry => new List<(int, int)>(entry.Value));
        }

        public override string ToString()
        {
            var y = 0;
            var sb = new StringBuilder();
            var sep = new StringBuilder("     ");

            for (var i = 0; i < _fieldSize; i++)
                sep.Append("+----");
            sep.Append('+');
            
            var sep1 = sep.ToString();
            sep.Clear();

            for (var i = 0; i < _fieldSize; i++)
            {

                var x = 0;
                sb.AppendLine(sep1);
                y++;
                sb.Append("     ");
                x += 5;
                for (var j = 0; j < _fieldSize; j++)
                {
                    if (i == _cursorPos.Item2 && j == _cursorPos.Item1)
                    {
                        x += 2;
                        HighlightPos = (x, y);
                    }
                    sb.Append($"| {(!_field[i][j].Fired ? "  " : _field[i][j].ShipName.HasValue ? "[]" : "øø")} ");
                    x += 5;
                    
                }
                sb.AppendLine("|");
                y++;
            }
            sb.AppendLine(sep1);
            return sb.ToString();
        }

        internal void ChangeCursorPos(int x, int y)
        {
            if (OnField(_cursorPos.Item1 + x, _cursorPos.Item2 + y))
                _cursorPos = (_cursorPos.Item1 + x, _cursorPos.Item2 + y);
        }

        internal Cell GetCursorCell()
        {
            return _field[_cursorPos.Item2][_cursorPos.Item1];
        }

        internal bool Fire()
        {
            var cell = _field[_cursorPos.Item2][_cursorPos.Item1];

            _field[_cursorPos.Item2][_cursorPos.Item1] = new Cell(cell.ShipName, true);

            if (!cell.ShipName.HasValue)
                return false;
            
            _tmpShips[cell.ShipName.Value].Remove(_cursorPos);
            if (_tmpShips[cell.ShipName.Value].Count != 0)
                return true;
            
            _tmpShips.Remove(cell.ShipName.Value);
            ShipsCount--;

            foreach (var shipCoord in _ships[cell.ShipName.Value])
                FireNear(shipCoord, cell.ShipName.Value);

            return true;
        }

        internal bool CheckFire()
        {
            return _field[_cursorPos.Item2][_cursorPos.Item1].Fired;
        }

        private void FireNear((int, int) coord, int shipName)
        {
            foreach (var dir in Directions)
            {
                var x = coord.Item1 + dir.Item1;
                var y = coord.Item2 + dir.Item2;

                if (OnField(x, y) && !_ships[shipName].Contains((x, y)))
                    _field[y][x] = new Cell(null, true);
            }
        }

        private bool OnField(int x, int y)
        {
            return x >= 0 && y >= 0 && x < _fieldSize && y < _fieldSize;
        }
    }
}