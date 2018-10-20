using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace MUL
{
    public partial class MainWindow : Window
    {
        private bool AdjustingPosition { get; set; }

        private void PositionChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (AdjustingPosition)
                TrackList.CurrentTrack.SetPosition(Convert.ToInt32(SldDuration.Value));
        }

        private void PositionAdjustStart(object sender, MouseEventArgs e)
        {
            Timer.Stop();
            AdjustingPosition = true;
        }

        private void PositionAdjustEnd(object sender, MouseEventArgs e)
        {
            Timer.Start();
            AdjustingPosition = false;
        }

        private void UpdatePosition()
        {
            if (TrackList.CurrentTrack.IsPlaying)
            {
                var ts = TrackList.CurrentTrack.Position;
                string s = String.Format("{0}:{1}", ts.Minutes.ToString().PadLeft(2, '0'), ts.Seconds.ToString().PadLeft(2, '0'));
                if (ts.Hours > 0)
                    s = String.Format("{0}:", ts.Hours.ToString().PadLeft(2, '0')) + s;
                LblTime.Content = s;
                SldDuration.Value = ts.TotalSeconds;
            }
            else
            {
                LblTime.Content = "00:00";
                SldDuration.Value = 0;
            }
        }
    }
}
