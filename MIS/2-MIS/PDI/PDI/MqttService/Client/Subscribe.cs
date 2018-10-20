using System;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using System.Globalization;
using MqttService.Models;
using MqttService.Logging;
using MQTTnet.Core;
using MQTTnet.Core.Client;
using Newtonsoft.Json.Linq;

namespace MqttService.Client
{
    /// <summary>
    /// Part of the Client class that handles MQTT subscriptions.
    /// </summary>
    public partial class Client
    {
        private async void Subscribe(string topic)
        {
            if (!this.IsConnected)
            {
                Logger.Error("Client is not connected");
                return;
            }

            Logger.Info("Subscribing to '{0}'", topic);
            if (!IsSubscribed(topic))
            {
                this.SubscribedTopics.Add(topic);
                await this._Client.SubscribeAsync(
                    new TopicFilterBuilder().WithTopic(topic).WithExactlyOnceQoS().Build()
                );
            }
        }

        private void Subscribe(string[] topics)
        {
            foreach (string t in topics)
                Subscribe(t);
        }

        private bool IsSubscribed(string topic)
        {
            return this.SubscribedTopics.Contains(topic);
        }

        private void SubscribeToSavedMcus()
        {
            if (!this.IsConnected)
            {
                Logger.Error("Client is not connected");
                return;
            }

            foreach (var m in this._Repository.Microcontrollers.All())
                SubscribeToMcu(m.DeviceId);
        }

        private void SubscribeToSavedPowerStrips()
        {
            if (!this.IsConnected)
            {
                Logger.Error("Client is not connected");
                return;
            }

            foreach (var m in this._Repository.PowerStrips.All())
                SubscribeToPowerStrip(m.DeviceId);
        }

        private void SubscribeToMcu(string devId)
        {
            if (!this.IsConnected)
            {
                Logger.Error("Client is not connected");
                return;
            }

            List<string> topics = new List<string>
            {
                Topic.McuState(devId),
                Topic.McuStatus(devId),
                Topic.McuTemperature(devId)
            };
            Subscribe(topics.ToArray());
        }

        private void SubscribeToPowerStrip(string devId)
        {
            List<string> topics = new List<string>
            {
                Topic.StripEnergy(devId),
                Topic.StripPowerStatus(devId)
            };
            Subscribe(topics.ToArray());
        }

        public async void Unsubscribe(string topic)
        {
            if (!this.IsConnected)
            {
                Logger.Error("Client is not connected");
                return;
            }

            if (IsSubscribed(topic))
            {
                this.SubscribedTopics.Remove(topic);
                await this._Client.UnsubscribeAsync(topic);
            }
        }

        public void Unsubscribe(string[] topics)
        {
            foreach (string t in topics)
                Unsubscribe(t);
        }

        /// <summary>
        /// Event handler that parses incoming messages.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMessageReceived(object sender, MqttApplicationMessageReceivedEventArgs e)
        {
            string topic = e.ApplicationMessage.Topic;
            string payload = Encoding.UTF8.GetString(e.ApplicationMessage.Payload);

            Logger.Info("Received message '{0}' in topic: '{1}'", payload, topic);
            if (topic == Topic.McuAnnounce())
            {
                ProcessMcuAnnounce(payload);
            }
            else if (Topic.IsPowerStripAnnounce(topic))
            {
                ProcessStripAnnounce(topic);
            }
            else if (Topic.IsPowerStripPowerStatus(topic))
            {
                ProcessStripPowerStatus(topic, payload);
            }
            else if(Topic.IsPowerStripEnergy(topic))
            {
                ProcessStripEnergyStatus(topic, payload);
            }
            else if (Topic.IsMcuStatus(topic))
            {
                ProcessMcuStatus(topic, payload);
            }
            else if (Topic.IsMcuTemp(topic))
            {
                ProcessMcuTemperature(topic, payload);
            }
        }

        /// <summary>
        /// Parses MCU's temperature and updates its value in the DB.
        /// Does nothing in case of error.
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="payload"></param>
        private void ProcessMcuTemperature(string topic, string payload)
        {
            string deviceId = Topic.McuIdFromTopic(topic);
            if (deviceId == null)
            {
                Logger.Error("Failed to get device ID from topic {0}", topic);
                return;
            }

            double temp = 0.0;
            try
            {
                temp = Double.Parse(payload.Trim(), CultureInfo.InvariantCulture);
            }
            catch
            {
                Logger.Error("Failed to convert MCU temperature value to double: '{0}'", payload);
                return;
            }
            Logger.Info("Updating MCU {0} temperature to '{1}'", deviceId, temp.ToString());
            this._Repository.Microcontrollers.Update(deviceId, temp);
        }

        /// <summary>
        /// Parses MCU's power state and updates its value in the DB.
        /// Does nothing in case of error.
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="payload"></param>
        private void ProcessMcuStatus(string topic, string payload)
        {
            string deviceId = Topic.McuIdFromTopic(topic);
            if (deviceId == null)
            {
                Logger.Error("Failed to get MCU ID from topic {0}", topic);
                return;
            }

            payload = payload.Trim();
            Logger.Info("Updating MCU {0} status to '{1}'", deviceId, payload.ToUpper());
            if (payload == "0")
            {
                this._Repository.Microcontrollers.Update(deviceId, false);
            }
            else if (payload == "1")
            {
                this._Repository.Microcontrollers.Update(deviceId, true);
            }
            else
            {
                Logger.Error("Invalid MCU state payload contents: '{0}'", payload);
            }
        }

        /// <summary>
        /// Parses power strip's energy status and updates its value and date in the DB.
        /// Does nothing in case of error.
        /// </summary>
        /// <param name="payload"></param>
        private void ProcessStripEnergyStatus(string topic, string payload)
        {
            string devId = Topic.PowerStripIdFromTopic(topic);
            if (devId == null)
            {
                Logger.Error("Failed to get power strip ID from topic {0}", payload);
                return;
            }

            JObject json = null;
            try
            {
                json = JObject.Parse(payload);
            }
            catch
            {
                Logger.Error("Power strip payload - Failed to parse JSON: '{0}'", payload);
                return;
            }
            var time   = json["Time"];
            var energy = json["Today"];
            if (time == null || energy == null)
            {
                Logger.Error("Invalid power strip energy status payload: '{0}'", payload);
                return;
            }
            else
            {
                double val = 0.0;
                try
                {
                    val = Double.Parse(energy.ToString(), CultureInfo.InvariantCulture);
                }
                catch
                {
                    Logger.Error("Failed to convert energy consumption value '{0}' to double", energy);
                    return;
                }
                Logger.Info("Updating power strip {0} energy status to {1} on {2}", devId, val.ToString(), time.ToString());
                _Repository.PowerStrips.Update(devId, val, time.ToString());
            }
        }

        /// <summary>
        /// Sets power strip's powered status to true/false if values in payload are on/off.
        /// Does nothing if values are something else.
        /// </summary>
        /// <param name="payload"></param>
        private void ProcessStripPowerStatus(string topic, string payload)
        {
            payload = payload.Trim().ToLower();
            string devId = Topic.PowerStripIdFromTopic(topic);
            if (devId == null)
            {
                Logger.Error("Failed to get power strip ID from topic {0}", payload);
                return;
            }
            Logger.Info("Updating power strip {0} status to '{1}'", devId, payload.ToUpper());
            if (payload == "on")
                _Repository.PowerStrips.Update(devId, true);
            else if(payload == "off")
                _Repository.PowerStrips.Update(devId, false);
        }

        /// <summary>
        /// Parses payload which is expected to be JSON with at least keys Module and Version.
        /// Does nothing if JSON parsing faile or keys are missing.
        /// Only one power strip may be in DB at the same time.
        /// </summary>
        /// <param name="payload"></param>
        private void ProcessStripAnnounce(string topic)
        {
            topic = topic.Trim().ToLower();
            string devId = Topic.PowerStripIdFromTopic(topic);
            if (devId == null)
            {
                Logger.Error("Failed to get power strip ID from topic {0}", topic);
                return;
            }
            if(_Repository.PowerStrips.Contains(devId))
            {
                Logger.Warn("Power strip {0} is already in the database", devId);
                return;
            }
            Logger.Info("Adding power strip '{0}' into the database", devId);
            this._Repository.PowerStrips.Add(new PowerStrip(devId));
            SubscribeToPowerStrip(devId);
        }

        /// <summary>
        /// Parses MCU's announce message and adds it in the DB.
        /// Does nothing in case of error.
        /// </summary>
        /// <param name="payload"></param>
        private void ProcessMcuAnnounce(string payload)
        {
            Logger.Info("Trying to add MCU '{0}'", payload);
            if (!_Repository.Microcontrollers.Contains(payload))
            {
                this._Repository.Microcontrollers.Add(new Microcontroller(payload));
                SubscribeToMcu(payload);
            }
            else
            {
                Logger.Warn("Device '{0}' is already in the database", payload);
            }
        }
    }
}
