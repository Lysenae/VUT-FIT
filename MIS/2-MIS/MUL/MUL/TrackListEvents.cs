using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace MUL
{
    public partial class MainWindow : Window
    {
        private Models.TrackList TrackList { get; set; }

        /// <summary>
        /// Handle changed selection of the tracks.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SelectedTrackChanged(object sender, SelectionChangedEventArgs e)
        {
            foreach (object unselected in e.RemovedItems)
                TrackList.Unselect((Models.File) unselected);
            foreach (object selected in e.AddedItems)
                TrackList.Select((Models.File) selected);
            Debug.WriteLine("Selected changed");
            SetEditorControls();
        }

        /// <summary>
        /// Play double clicked track.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TrackListDoubleClicked(object sender, MouseEventArgs e)
        {
            if (TrackList.HasSelection)
                Play();
        }

        private void TrackListKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Delete)
            {
                Debug.WriteLine("Delete");
                if (TrackList.IsSelectedPlayed())
                {
                    Stop();
                    TrackList.CurrentTrack = null;
                }
                TrackList.RemoveSelected();
                e.Handled = true;
            }
        }
    }
}
