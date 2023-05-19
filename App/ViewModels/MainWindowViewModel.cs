using System;
using System.Reactive;
using System.Threading.Tasks;
using App.Models;
using Avalonia.Controls;
using ReactiveUI;

namespace App.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
    public static readonly Task<Field> MyField = Field.GetInstanse();

    public MainWindowViewModel()
    {
    }
    
    public static event EventHandler? MyFunctionTriggered;

    public async void NextGeneration()
    {
        await Task.Run(() => MyField.Result.CalculateNextGenerationAsync());
        // TODO multiply calls doesn't work (server fails)
        
        MyFunctionTriggered?.Invoke(this, EventArgs.Empty);
    }
}