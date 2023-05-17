using System;
using System.Reactive;
using System.Threading.Tasks;
using App.Models;
using Avalonia.Controls;
using ReactiveUI;

namespace App.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
    private Task<Field> _field = Models.Field.BuildFieldAsync();

    public MainWindowViewModel()
    {
    }

    public async void NextGeneration()
    {
        await Task.Run(() => _field.Result.CalculateNextGenerationAsync());
    }
}