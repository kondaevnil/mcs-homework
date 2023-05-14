using Client.Models;

namespace Client.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
    public string Greeting => "Welcome to Avalonia!";

    public Field Field;

    public MainWindowViewModel()
    {
        Field = new Field();
    }
}