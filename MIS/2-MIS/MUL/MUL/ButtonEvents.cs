using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Media;

namespace MUL
{
    public partial class MainWindow : Window
    {
        private void OpenClicked(object sender, RoutedEventArgs e)
        {
            var ofd = new Microsoft.Win32.OpenFileDialog() { Filter = "MP3 Files (*.mp3)|*.mp3", Multiselect = true };
            var result = ofd.ShowDialog();
            if (result == false) return;
            foreach (string f in ofd.FileNames)
            {
                TrackList.AddTrack(f);
            }
            LbxTrackList.UpdateLayout();
        }

        private void SaveClicked(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Save clicked");
            if(TrackList.SelectedCount == 1)
            {
                TrackList.FirstSelected().Tags.Title = ParseRegexp(TbTitle.Text, TrackList.FirstSelected().Tags.Title);
                TrackList.FirstSelected().Tags.Album = ParseRegexp(TbAlbum.Text, TrackList.FirstSelected().Tags.Album);
                TrackList.FirstSelected().Tags.Artists = ParseRegexp(TbArtist.Text, TrackList.FirstSelected().Tags.Artists);
                Debug.WriteLine(ParseRegexp(TbArtist.Text, TrackList.FirstSelected().Tags.Artists));
                TrackList.FirstSelected().Tags.AlbumArtists = ParseRegexp(TbAlbumArtist.Text, TrackList.FirstSelected().Tags.AlbumArtists);
                TrackList.FirstSelected().Tags.Genres = ParseRegexp(TbGenres.Text, TrackList.FirstSelected().Tags.Genres);
                TrackList.FirstSelected().Tags.Composers = ParseRegexp(TbComposers.Text, TrackList.FirstSelected().Tags.Composers);
                TrackList.FirstSelected().Tags.Conductor = ParseRegexp(TbConductor.Text, TrackList.FirstSelected().Tags.Conductor);
                TrackList.FirstSelected().Tags.Copyright = ParseRegexp(TbCopyright.Text, TrackList.FirstSelected().Tags.Copyright);
                TrackList.FirstSelected().Tags.Grouping = ParseRegexp(TbGrouping.Text, TrackList.FirstSelected().Tags.Grouping);
                TrackList.FirstSelected().Tags.Lyrics = TblockLyrics.Text;
                TrackList.FirstSelected().Tags.Year = ConvertStringToUint(TbYear.Text);
                TrackList.FirstSelected().Tags.Track = ConvertStringToUint(TbTrack.Text);
                TrackList.FirstSelected().Tags.TrackCount = ConvertStringToUint(TbTrackCount.Text);
                TrackList.FirstSelected().Tags.Disc = ConvertStringToUint(TbDisc.Text);
                TrackList.FirstSelected().Tags.DiscCount = ConvertStringToUint(TbDiscCount.Text);
                try
                {
                    TrackList.FirstSelected().UpdateTags();
                }
                catch (Exception ex)
                {
                    System.Windows.MessageBox.Show(
                        "An error occured while trying to update media tags " + ex.Message,
                        "Error",
                        MessageBoxButton.OK,
                        MessageBoxImage.Error
                    );
                }
            }
            else if(TrackList.SelectedCount > 1)
            {
                foreach (Models.File f in TrackList.CurrentSelection)
                {
                    if (ChbAlbum.IsChecked == true)
                        f.Tags.Album = ParseRegexp(TbAlbum.Text, f.Tags.Album);
                    if (ChbArtist.IsChecked == true)
                        f.Tags.Artists = ParseRegexp(TbArtist.Text, f.Tags.Artists);
                    if (ChbAlbumArtist.IsChecked == true)
                        f.Tags.AlbumArtists = ParseRegexp(TbAlbumArtist.Text, f.Tags.AlbumArtists);
                    if (ChbGenres.IsChecked == true)
                        f.Tags.Genres = ParseRegexp(TbGenres.Text, f.Tags.Genres);
                    if (ChbComposers.IsChecked == true)
                        f.Tags.Composers = ParseRegexp(TbComposers.Text, f.Tags.Composers);
                    if (ChbConductor.IsChecked == true)
                        f.Tags.Conductor = ParseRegexp(TbConductor.Text, f.Tags.Conductor);
                    if (ChbCopyright.IsChecked == true)
                        f.Tags.Copyright = ParseRegexp(TbCopyright.Text, f.Tags.Copyright);
                    if (ChbGrouping.IsChecked == true)
                        f.Tags.Grouping = ParseRegexp(TbGrouping.Text, f.Tags.Grouping);
                    if (ChbYear.IsChecked == true)
                        f.Tags.Year = ConvertStringToUint(TbYear.Text);
                    if (ChbTrack.IsChecked == true)
                    {
                        f.Tags.Track = ConvertStringToUint(TbTrack.Text);
                        f.Tags.TrackCount = ConvertStringToUint(TbTrackCount.Text);
                    }
                    if (ChbDisc.IsChecked == true)
                    {
                        f.Tags.Disc = ConvertStringToUint(TbDisc.Text);
                        f.Tags.DiscCount = ConvertStringToUint(TbDiscCount.Text);
                    }
                    try
                    {
                        f.UpdateTags();
                    }
                    catch(Exception ex)
                    {
                        System.Windows.MessageBox.Show(
                            "An error occured while trying to update media tags " + ex.Message,
                            "Error",
                            MessageBoxButton.OK,
                            MessageBoxImage.Error
                        );
                    }
                }
            }
            SetEditorControls();
        }

        private void PlayClicked(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Play clicked");
            Play();
        }

        private void PauseClicked(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Pause clicked");
            Pause();
        }

        private void StopClicked(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Stop clicked");
            Stop();
        }

        private void NextClicked(object sender, RoutedEventArgs e)
        {
            if (TrackList.HasCurrent)
            {
                int idx = LbxTrackList.Items.IndexOf(TrackList.CurrentTrack);
                Debug.WriteLine("Current: {0}", idx);
                if (LbxTrackList.Items.Count > idx + 1)
                {
                    TrackList.QueuedTrack = (Models.File)LbxTrackList.Items[idx + 1];
                    Debug.WriteLine("Queued {0}", TrackList.QueuedTrack);
                    Play();
                }
                else
                {
                    Debug.WriteLine("Currently playing last track");
                }
            }
        }

        private void PrevClicked(object sender, RoutedEventArgs e)
        {
            if (TrackList.HasCurrent)
            {
                int idx = LbxTrackList.Items.IndexOf(TrackList.CurrentTrack);
                Debug.WriteLine("Current: {0}", idx);
                if (idx - 1 >= 0)
                {
                    TrackList.QueuedTrack = (Models.File)LbxTrackList.Items[idx - 1];
                    Debug.WriteLine("Queued {0}", TrackList.QueuedTrack);
                    Play();
                }
                else
                {
                    Debug.WriteLine("Currently playing first track");
                }
            }
        }

        private void Play()
        {
            if (TrackList.HasCurrent && TrackList.HasQueued)
            {
                Stop();
                TrackList.CurrentTrack = TrackList.QueuedTrack;
            }
            if (!TrackList.HasCurrent && TrackList.HasSelection)
            {
                TrackList.CurrentTrack = TrackList.CurrentSelection[0];
            }
            if(TrackList.HasCurrent && !TrackList.HasQueued && TrackList.HasSelection &&
            TrackList.CurrentSelection[0] != TrackList.CurrentTrack)
            {
                Stop();
                TrackList.CurrentTrack = TrackList.CurrentSelection[0];
            }
            TrackList.QueuedTrack = null; // Remove any queued track
            if (TrackList.HasCurrent)
            {
                if (!TrackList.CurrentTrack.IsPaused)
                    Stop();
                TrackList.CurrentTrack.PlaybackFinished += CurrentTrackPlaybackFinished;
                int idx = LbxTrackList.Items.IndexOf(TrackList.CurrentTrack);
                ListBoxItem item = (ListBoxItem)LbxTrackList.Items[idx];
                item.FontWeight = FontWeights.Bold;
                TrackList.CurrentTrack.Play();
                TrackList.CurrentTrack.SetVolume((float)(SldVolume.Value / 100));
                //Debug.WriteLine("Playing: {0}", TrackList.CurrentTrack.IsPlaying);
                //Debug.WriteLine("Paused: {0}", TrackList.CurrentTrack.IsPaused);
                Timer.Start();
                SldDuration.Maximum = TrackList.CurrentTrack.Duration.TotalSeconds;
                UpdatePosition();
            }
        }

        private void Pause()
        {
            if (TrackList.HasCurrent)
            {
                TrackList.CurrentTrack.Pause();
                //Debug.WriteLine("Playing: {0}", TrackList.CurrentTrack.IsPlaying);
                //Debug.WriteLine("Paused: {0}", TrackList.CurrentTrack.IsPaused);
                if (TrackList.CurrentTrack.IsPaused)
                    Timer.Stop();
                else
                    Timer.Start();
                UpdatePosition();
            }
        }

        private void Stop()
        {
            if (TrackList.HasCurrent)
            {
                TrackList.CurrentTrack.Stop();
                TrackList.CurrentTrack.PlaybackFinished -= CurrentTrackPlaybackFinished;
                //Debug.WriteLine("Playing: {0}", TrackList.CurrentTrack.IsPlaying);
                //Debug.WriteLine("Paused: {0}", TrackList.CurrentTrack.IsPaused);
                Timer.Stop();
                UpdatePosition();
                SldDuration.Maximum = 0;
                foreach (ListBoxItem i in LbxTrackList.Items)
                    i.FontWeight = FontWeights.Normal;
            }
        }

        private void CurrentTrackPlaybackFinished(object sender, EventArgs e)
        {
            if (TrackList.HasCurrent)
            {
                int idx = LbxTrackList.Items.IndexOf(TrackList.CurrentTrack);
                Debug.WriteLine("Current: {0}", idx);
                if (LbxTrackList.Items.Count > idx + 1)
                {
                    TrackList.QueuedTrack = (Models.File)LbxTrackList.Items[idx + 1];
                    Debug.WriteLine("Queued {0}", TrackList.QueuedTrack);
                    Play();
                }
                else
                {
                    foreach (ListBoxItem i in LbxTrackList.Items)
                        i.FontWeight = FontWeights.Normal;
                }
            }
        }
    }
}
