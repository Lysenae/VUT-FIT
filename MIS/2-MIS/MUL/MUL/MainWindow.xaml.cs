using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace MUL
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            Closing += OnClosing;
            TrackList = new Models.TrackList();
            Timer = new DispatcherTimer();
            Timer.Tick += new EventHandler(TimerTicked);
            AdjustingPosition = false;
            InitializeComponent();
            LbxTrackList.ItemsSource = TrackList.Tracks;
            DisableEditorControls();
        }

        private void ValidateNumTb(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private uint ConvertStringToUint(string s)
        {
            s = s.TrimStart('0');
            s = s.Length == 0 ? "0" : s;
            try
            {
                uint n = Convert.ToUInt32(s);
                return n;
            }
            catch
            {
                return 0;
            }
        }

        private string ParseRegexp(string s, string def)
        {
            if (s == null)
                return s;

            Match match = Regex.Match(s, @"sub\(%r{(.*?)}, ""(.*?)""\)");
            if (match.Success)
            {
                string rgx = match.Groups[1].Value;
                string val = match.Groups[2].Value;
                Debug.WriteLine("Substitution regexp found: " + rgx + ", with value: " + val);
                try
                {
                    Regex.Match("", rgx);
                }
                catch
                {
                    return def;
                }
                string rslt = Regex.Replace(def, rgx, val);
                Debug.WriteLine(rslt);
                return rslt;
            }
            else
            {
                return s;
            }
        }

        private void OnClosing(object sender, CancelEventArgs cancelEventArgs)
        {
            TrackList.Dispose();
        }
    }
}
