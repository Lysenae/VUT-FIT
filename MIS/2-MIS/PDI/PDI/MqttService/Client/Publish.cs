using MqttService.Logging;
using MQTTnet.Core;

namespace MqttService.Client
{
    /// <summary>
    /// Part of the Client class that handles MQTT publishing.
    /// </summary>
    public partial class Client
    {
        private async void Publish(string topic, string payload)
        {
            if (!this.IsConnected)
            {
                Logger.Error("Client is not connected");
                return;
            }

            var message = new MqttApplicationMessageBuilder()
                .WithTopic(topic)
                .WithPayload(payload)
                .WithExactlyOnceQoS()
                .WithRetainFlag(false)
                .Build();
            await this._Client.PublishAsync(message);
        }

        /// <summary>
        /// Commands power strip to power on/off. If value is not 1 or 0 does nothing.
        /// Does nothing if no power strip is in the database.
        /// </summary>
        /// <param name="value"></param>
        public void PublishStripPowerCommand(string devId, int value)
        {
            if(this._Repository.PowerStrips.Count() == 0 ||
            value == 0 || value == 1)
                Publish(Topic.StripPowerCommand(devId), value.ToString());
        }

        /// <summary>
        /// Commands MCU to power on/off. If value is not 1 or 0 does nothing.
        /// Does nothing if no power strip is in the database.
        /// </summary>
        /// <param name="deviceId"></param>
        /// <param name="value"></param>
        public void PublishMcuPowerCommand(string deviceId, int value)
        {
            if (value == 0 || value == 1)
            {
                Publish(Topic.McuState(deviceId), value.ToString());
            }
        }

        /// <summary>
        /// Issues hard shutdown command for given device.
        /// </summary>
        /// <param name="deviceId"></param>
        public void PublishMcuHardShutdownCommand(string deviceId)
        {
            Publish(Topic.McuHardShutdown(deviceId), 1.ToString());
        }

        /// <summary>
        /// Issues reset command for given device.
        /// </summary>
        /// <param name="deviceId"></param>
        public void PublishMcuResetCommand(string deviceId)
        {
            Publish(Topic.McuReset(deviceId), 1.ToString());
        }
    }
}
